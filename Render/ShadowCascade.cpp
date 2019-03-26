#include "Common.hpp"
#include <i0rRender/Frustum.hpp>
#include <i0rRender/Mesh.hpp>
#include <i0rRender/Lod.hpp>
#include "i0rPrograms/Shadow.hpp"
#include "ShadowCascade.hpp"

IFrameBuffer* ShadowCascade::sun         = nullptr;
IShader*      ShadowCascade::shaderDebug = nullptr;

f32 minDistance = 0.30f;
f32 maxDistance = 1.00f;

static struct frustum_csm_t {
	vec3f xyz;
	vec3f xyZ;
	vec3f xYz;
	vec3f xYZ;

	vec3f Xyz;
	vec3f XyZ;
	vec3f XYz;
	vec3f XYZ;

	vec3f &operator []( const i32 i ) {
		switch( i ) {
			case 0:
				return xyz;

			case 1:
				return xyZ;

			case 2:
				return xYz;

			case 3:
				return xYZ;

			case 4:
				return Xyz;

			case 5:
				return XyZ;

			case 6:
				return XYz;

			case 7:
				return XYZ;
		}

		return xyz;
	}
} m_Frustum = {};

f32 ShadowCascade::ComputeCSMSplitDistance( const i32 split ) {
	const f32 fLambda = 1.000f;
	const f32 clipRange = Instance.Configuration->Graphics.FarPlane - Instance.Configuration->Graphics.NearPlane;
	const f32 minZ = Instance.Configuration->Graphics.NearPlane + minDistance * clipRange;
	const f32 maxZ = Instance.Configuration->Graphics.NearPlane + maxDistance * clipRange;
	const f32 range = maxZ - minZ;
	const f32 ratio = maxZ / minZ;
	const f32 p = ( split + 1 ) / static_cast<f32>( SHADOW_DL_SPLIT_COUNT );
	const f32 log = minZ * std::pow( ratio, p );
	const f32 uniform = minZ + range * p;
	const f32 d = fLambda * ( log - uniform ) + uniform;

	return ( d - Instance.Configuration->Graphics.NearPlane ) / clipRange;
}

f32 ShadowCascade::GetSceneRadius() {
	vec3f lowBounds = vec3f( F32_MAX ), hiBounds = vec3f( F32_MIN );

	for( Mesh* m : Instance.SceneManager->GetMeshes() ) {
		lowBounds.x = min( lowBounds.x, m->m_WorldTranslation.x );
		lowBounds.y = min( lowBounds.y, m->m_WorldTranslation.y );
		lowBounds.z = min( lowBounds.z, m->m_WorldTranslation.z );

		hiBounds.x = max( hiBounds.x, m->m_WorldTranslation.x );
		hiBounds.y = max( hiBounds.y, m->m_WorldTranslation.y );
		hiBounds.z = max( hiBounds.z, m->m_WorldTranslation.z );
	}
	
	f32 distance = Distance3D( lowBounds, hiBounds ) / 40.0f;
	//Clamp( distance, 0.0f, 10.0f );
	return distance;
}

void ShadowCascade::ComputeMatrices() {
	Shadows.SunSlices[0].FarDistance = ComputeCSMSplitDistance( 0 );
	Shadows.SunSlices[1].FarDistance = ComputeCSMSplitDistance( 1 );
	Shadows.SunSlices[2].FarDistance = ComputeCSMSplitDistance( 2 );
	Shadows.SunSlices[3].FarDistance = ComputeCSMSplitDistance( 3 );

	for( i32 c = 0; c < SHADOW_DL_SPLIT_COUNT; ++c ) {
		f32 prevSplitDistance = c == 0 ? minDistance : Shadows.SunSlices[c - 1].FarDistance;
		f32 splitDistance     = Shadows.SunSlices[c].FarDistance;

		vec3f frustumCornersWS[8] = {
			vec3f( -1.0f,  1.0f, -1.0f ),
			vec3f(  1.0f,  1.0f, -1.0f ),
			vec3f(  1.0f, -1.0f, -1.0f ),
			vec3f( -1.0f, -1.0f, -1.0f ),
			vec3f( -1.0f,  1.0f,  1.0f ),
			vec3f(  1.0f,  1.0f,  1.0f ),
			vec3f(  1.0f, -1.0f,  1.0f ),
			vec3f( -1.0f, -1.0f,  1.0f ),
		};

		mat4x4f invViewProj = Inverse( Common.ViewProjection );
		for( u32 i = 0; i < 8; ++i ) {
			vec4f inversePoint = invViewProj * vec4f( frustumCornersWS[i], 1.0f );
			frustumCornersWS[i] = vec3f( inversePoint ) / inversePoint.w;
		}

		const f32 clipDist = Instance.Configuration->Graphics.FarPlane - Instance.Configuration->Graphics.NearPlane;

		for( u32 i = 0; i < 4; ++i ) {
			vec3f cornerRay         = frustumCornersWS[i + 4] - frustumCornersWS[i];
			vec3f nearCornerRay     = cornerRay * prevSplitDistance;
			vec3f farCornerRay      = cornerRay * splitDistance;
			frustumCornersWS[i + 4] = frustumCornersWS[i] + farCornerRay;
			frustumCornersWS[i]     = frustumCornersWS[i] + nearCornerRay;
		}

		vec3f frustumCenter = vec3f( 0.0f );
		for( u32 i = 0; i < 8; ++i ) {
			frustumCenter += frustumCornersWS[i];
		}

		//frustumCenter /= 8.0f;

		f32 ffar  = F32_MIN;
		f32 fnear = F32_MAX;

		f32 radius = 0.0f;
		for( u32 i = 0; i < 8; ++i ) {
			f32 distance = Length( frustumCornersWS[i] - frustumCenter );
			radius = max( radius, distance );
		}

		radius = std::ceil( radius ) * GetSceneRadius();

		vec3f maxOrtho = frustumCenter + vec3f( radius, radius, radius );
		vec3f minOrtho = frustumCenter - vec3f( radius, radius, radius );

		vec3f lightDirection = frustumCenter + ( Sky.SunShadowDir );
		mat4x4f lightViewMatrix = LookAt( lightDirection, frustumCenter, vec3f( 0.0f, 1.0f, 0.0f ) );

		maxOrtho = vec3f( lightViewMatrix * vec4f( maxOrtho, 1.0f ) * 8.0f );
		minOrtho = vec3f( lightViewMatrix * vec4f( minOrtho, 1.0f ) * 8.0f );

		ffar  = maxOrtho.z;
		fnear = minOrtho.z;

		mat4x4f lightOrthoMatrix = Ortho( minOrtho.x, maxOrtho.x, minOrtho.y, maxOrtho.y, fnear, ffar );

		mat4x4f shadowMatrix = lightOrthoMatrix * lightViewMatrix;
		vec4f shadowOrigin = vec4f( Common.ViewPosition, 1.0f );
		shadowOrigin = shadowMatrix * shadowOrigin;
		shadowOrigin = shadowOrigin * 2.0f / (f32)Instance.Configuration->Graphics.Shadows.Resolution;

		vec4f roundedOrigin = Round( shadowOrigin );
		vec4f roundOffset = roundedOrigin - shadowOrigin;
		roundOffset       = roundOffset * 2.0f / (f32)Instance.Configuration->Graphics.Shadows.Resolution;
		roundOffset.z     = 0.0f;
		roundOffset.w     = 0.0f;

		mat4x4f shadowProj = lightOrthoMatrix;
		shadowProj.r3 += roundOffset;
		lightOrthoMatrix = shadowProj;

		Shadows.SunSlices[c].FarDistance = Instance.Configuration->Graphics.NearPlane + splitDistance * clipDist;

		Shadows.SunSlices[c].ModelView = lightOrthoMatrix * lightViewMatrix;
	}

	Instance.GraphicsApiContext->UploadUBO( &Shadows, sizeof( Shadows ), uboShadows );
}

void ShadowCascade::Create() {
	if( !shaderDebug ) {
		shaderDebug = Instance.ShaderManager->GetShader( "CSMDebug", SHADER_STAGE_VERTEX | SHADER_STAGE_FRAGMENT );
		shaderDebug->Bind();
		shaderDebug->AddAttribute( "uIndex" );
		shaderDebug->AddAttribute( "uDiffuseTextureSampler" );
		shaderDebug->SetAttribute1i( "uDiffuseTextureSampler", 7 );

		Instance.GraphicsApiContext->UnbindShaderProgram();
	}

	sun = Instance.GraphicsApiContext->CreateFrameBuffer( Instance.Configuration->Graphics.Shadows.Resolution, 
								                                               Instance.Configuration->Graphics.Shadows.Resolution );

	sun->Bind( GENERIC_RENDER_FRAMEBUFFER_RW );
	sun->AddAttachementDepthArray2D( GENERIC_RENDER_DEPTH_COMPONENT16, GENERIC_RENDER_DEPTH_COMPONENT, GENERIC_RENDER_FLOAT,
								                          GENERIC_RENDER_CLAMP_TO_BORDER, SHADOW_DL_SPLIT_COUNT );
}

void ShadowCascade::ShadowPass() {
	Instance.GraphicsApiContext->SetFaceCulling( GENERIC_RENDER_FRONT );

	for( i32 c = 0; c < SHADOW_DL_SPLIT_COUNT; ++c ) {
		sun->BindDepthWrite( c );
		Instance.GraphicsApiContext->ClearBuffer( GENERIC_RENDER_DEPTH_BUFFER_BIT );
		Instance.Renderer->GetShadowProgram()->RenderScene( Shadows.SunSlices[c].ModelView );
	}

	Instance.GraphicsApiContext->SetFaceCulling( GENERIC_RENDER_BACK );
	sun->BindDepthMap( TEXTURE_BIND_SUN_CSM );
	Instance.GraphicsApiContext->UpdateViewport( 0, 0, Instance.Configuration->Display.WindowWidth, Instance.Configuration->Display.WindowHeight );
}

void ShadowCascade::Debug() {
	sun->BindDepthMap( TEXTURE_BIND_SUN_CSM );
	shaderDebug->Bind();

	for( i32 i = 0; i < SHADOW_DL_SPLIT_COUNT; ++i ) {
		shaderDebug->SetAttribute1i( "uIndex", i );
		Instance.GraphicsApiContext->UpdateViewport( ( 5 + 256 ) * i + 5, 5, 256, 256 );
		ScreenQuad::VAO->Bind();
		ScreenQuad::VAO->Draw();
	}

	ScreenQuad::VAO->Unbind();

	Instance.GraphicsApiContext->UnbindShaderProgram();
	Instance.GraphicsApiContext->UpdateViewport( 0, 0, Instance.Configuration->Display.WindowWidth, Instance.Configuration->Display.WindowHeight );
}
