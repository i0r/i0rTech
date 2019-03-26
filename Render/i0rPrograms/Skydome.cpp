#include "../Common.hpp"
#include <i0rCore/FSmallGeometry.hpp>
#include <i0rDebug/IDevMenu.hpp>
#include "../SmallGeometry.hpp"
#include "EnvironmentProbeRendering.hpp"
#include "ArHosekSkyModelData_RGB.h"
#include "Skydome.hpp"

#include <random>

Skydome::Skydome() : m_Dome( NULL ), m_Shader( NULL ), 
	m_ShaderEnvProbe( NULL ) {

}

Skydome::~Skydome() {
	SAFE_DELETE( m_Dome )
// SAFE_DELETE( m_Noise )
	m_Shader = NULL;
	m_ShaderEnvProbe = NULL;
}

void Skydome::ComputeNoise() {
	/*const i32 noiseWidth = 256, noiseHeight = 256;
	std::random_device randomDevice;
	std::mt19937 generator( randomDevice() );
	std::uniform_real_distribution<float> distribution( 0.0f, 1.0f );

	colorrgbf* texels = new colorrgbf[noiseWidth * noiseHeight];

	for( int32_t y = 0; y < noiseHeight; y++ ) {
		for( int32_t x = 0; x < noiseWidth; x++ ) {
			texels[x + y * noiseWidth].r = distribution( generator );
			texels[x + y * noiseWidth].b = distribution( generator );
		}
	}

	int32_t x2 = 0, y2 = 0;
	for( int32_t y = 0; y < noiseHeight; y++ ) {
		for( int32_t x = 0; x < noiseWidth; x++ ) {
			x2 = ( x - 37 ) & 255;
			y2 = ( y - 17 ) & 255;
			texels[x + y * noiseWidth].g = texels[x2 + y2 * noiseWidth].r;
		}
	}

	m_Noise = Instance.GraphicsApiContext->CreateTexture();
	m_Noise->Name = "Skydome Noise";
	m_Noise->IsManaged = false;
	m_Noise->CreateTexture2D( { GL_RGB16F, GL_RGB, GENERIC_RENDER_FLOAT,
		GENERIC_RENDER_CLAMP_TO_EDGE, 0, GENERIC_RENDER_LINEAR, GENERIC_RENDER_LINEAR }, noiseWidth, noiseHeight, texels );
	m_Noise->FixedSlot = TEXTURE_BIND_SKYDOME_NOISE;
	m_Noise->Bind( TEXTURE_BIND_SKYDOME_NOISE );

	delete[] texels;*/
}

bool Skydome::Initialize() {
	m_Shader = Instance.ShaderManager->GetShader( "Skydome", SHADER_STAGE_VERTEX | SHADER_STAGE_FRAGMENT );

	if( !m_Shader ) {
		return false;
	}

	m_Shader->Bind();

	m_Shader->AddUniformBlock( "ubo_common_t" );
	m_Shader->SetConstantUniformBufferIndex( "ubo_common_t", UBO_COMMON );

	m_Shader->AddUniformBlock( "ubo_sky_t" );
	m_Shader->SetConstantUniformBufferIndex( "ubo_sky_t", UBO_SKY );

	m_ShaderEnvProbe = Instance.ShaderManager->GetShader( "Skydome", SHADER_STAGE_VERTEX | SHADER_STAGE_FRAGMENT, 
								                                               { "NO_UNIFORM_BUFFER" } );

	if( !m_ShaderEnvProbe ) {
		return false;
	}

	m_ShaderEnvProbe->AddUniformBlock( "ubo_sky_t" );
	m_ShaderEnvProbe->SetConstantUniformBufferIndex( "ubo_sky_t", UBO_SKY );
	m_ShaderEnvProbe->AddAttribute( "uViewProjection" );
	m_ShaderEnvProbe->AddAttribute( "uViewPosition" );

	ComputeNoise();

	// this should be set depending on world parameters...
	m_SunTheta     = 30.0f;
	m_SunGamma     = 70.0f;
	m_SunPosition  = 0.10f;
	m_SkyTurbidity = 3.60f;
	m_SkyAlbedo    = 0.00f;
	
#ifdef FLAG_DEBUG
	Instance.DebugMenu->RegisterFloat( &m_SunTheta, 5.0f, 0.0f, 90.0f, "Sun Theta", "\\World\\Skydome", std::bind( &Skydome::UpdateDome, this ) );
	Instance.DebugMenu->RegisterFloat( &m_SunGamma, 5.0f, 0.0f, 90.0f, "Sun Gamma", "\\World\\Skydome", std::bind( &Skydome::UpdateDome, this ) );
	Instance.DebugMenu->RegisterFloat( &m_SunPosition, 0.01f, 0.0f, 16.0f, "Sun Position", "\\World\\Skydome", std::bind( &Skydome::UpdateDome, this ) );
	Instance.DebugMenu->RegisterFloat( &m_SkyTurbidity, 0.1f, 0.0f, 16.0f, "Sky Turbidity", "\\World\\Skydome", std::bind( &Skydome::UpdateDome, this ) );
	Instance.DebugMenu->RegisterFloat( &m_SkyAlbedo, 0.1f, 0.0f, 1.0f, "Sky Albedo", "\\World\\Skydome", std::bind( &Skydome::UpdateDome, this ) );
#endif

	UpdateDome();

	m_Dome = new smallgeometry_t();
		
	file_small_geometry_t* file = Instance.FileSystem->GetFileSmallGeometry( "Geometry/World/Skydome.sge" );
		
	if( !file ) {
		return false;
	}

	m_Dome->Build( file );

	return true;
}

void Skydome::UpdateDome() {
	ComputeRadiance( m_SunPosition, DegreesToRadians( m_SunTheta ), m_SkyTurbidity, m_SkyAlbedo );

	Sky.SunDirection = SphericalCoordinates( DegreesToRadians( m_SunTheta ), DegreesToRadians( m_SunGamma ) );
	Sky.SunShadowDir = SphericalCoordinates( 360.0f - DegreesToRadians( m_SunTheta ), DegreesToRadians( m_SunGamma ) );

	Instance.GraphicsApiContext->UploadUBO( &Sky, sizeof( Sky ), uboSky );
}

void Skydome::Render() {
	Instance.GraphicsApiContext->EnableBackBufferDepthWrite( GENERIC_RENDER_FALSE );

	m_Shader->Bind();
	m_Dome->Draw();
	
	Instance.GraphicsApiContext->EnableBackBufferDepthWrite( GENERIC_RENDER_TRUE );
}
	
void Skydome::ComputeRadiance( f32 sunPosition, f32 theta, f32 turbidity, f32 albedo ) {
	for( i32 i = 0; i < 3; ++i ) {
		Sky.A[i] = ComputeHosek( datasetsRGB[i] + 0, 9, turbidity, albedo, theta );
		Sky.B[i] = ComputeHosek( datasetsRGB[i] + 1, 9, turbidity, albedo, theta );
		Sky.C[i] = ComputeHosek( datasetsRGB[i] + 2, 9, turbidity, albedo, theta );
		Sky.D[i] = ComputeHosek( datasetsRGB[i] + 3, 9, turbidity, albedo, theta );
		Sky.E[i] = ComputeHosek( datasetsRGB[i] + 4, 9, turbidity, albedo, theta );
		Sky.F[i] = ComputeHosek( datasetsRGB[i] + 5, 9, turbidity, albedo, theta );
		Sky.G[i] = ComputeHosek( datasetsRGB[i] + 6, 9, turbidity, albedo, theta );
		Sky.H[i] = ComputeHosek( datasetsRGB[i] + 8, 9, turbidity, albedo, theta );
		Sky.I[i] = ComputeHosek( datasetsRGB[i] + 7, 9, turbidity, albedo, theta );
		Sky.Z[i] = ComputeHosek( datasetsRGBRad[i], 1, turbidity, albedo, theta );
	}

	const vec3f S = PerezExtinction( cos( theta ), 0.0f, 1.0f ) * Sky.Z;
	Sky.Z /= Dot( S, vec3f( 0.2126f, 0.7152f, 0.0722f ) );
	Sky.Z *= sunPosition;
}

f64 Skydome::SplineHosek( const f64* spline, u32 stride, f64 value ) const {
	return 1 * pow( 1 - value, 5 ) * spline[0 * stride] +
								5 * pow( 1 - value, 4 ) * pow( value, 1 ) * spline[1 * stride] +
								10 * pow( 1 - value, 3 ) * pow( value, 2 ) * spline[2 * stride] +
								10 * pow( 1 - value, 2 ) * pow( value, 3 ) * spline[3 * stride] +
								5 * pow( 1 - value, 1 ) * pow( value, 4 ) * spline[4 * stride] +
								1 * pow( value, 5 ) * spline[5 * stride];
}

f32 Skydome::ComputeHosek( const f64* dataset, u32 stride, f32 turbidity, f32 albedo, f32 m_SunTheta ) {
	const f64 ek = pow( max( 0.0, 1 - m_SunTheta / ( PI / 2 ) ), 1 / 3.0 );
		
	i32 t0       = (i32)turbidity;
	Clamp(t0, 1, 10);

	const i32 t1 = min( t0 + 1, 10 );
		
	f32 tk       = turbidity - (f32)t0;
	SATURATE(tk);

	const f64* datasetA0 = dataset;
	const f64* datasetA1 = dataset + stride * 6 * 10;
	const f64 a0t0       = SplineHosek( datasetA0 + stride * 6 * ( t0 - 1 ), stride, ek );
	const f64 a1t0       = SplineHosek( datasetA1 + stride * 6 * ( t0 - 1 ), stride, ek );
	const f64 a0t1       = SplineHosek( datasetA0 + stride * 6 * ( t1 - 1 ), stride, ek );
	const f64 a1t1       = SplineHosek( datasetA1 + stride * 6 * ( t1 - 1 ), stride, ek );

	return (f32)( a0t0 * ( 1 - albedo ) * ( 1 - tk ) + a1t0 * albedo * 
								( 1 - tk ) + a0t1 * ( 1 - albedo ) * tk + a1t1 * albedo * tk );
}

vec3f Skydome::PerezExtinction( f32 cos_theta, f32 gamma, f32 cos_gamma ) {
		const vec3f chi = ( 1 + cos_gamma * cos_gamma ) / Pow( 1.0f + Sky.H * Sky.H - 2.0f * cos_gamma * Sky.H, vec3f( 1.5f ) );
		return ( 1.f + Sky.A * Exp( Sky.B / ( cos_theta + 0.01f ) ) ) * ( Sky.C + Sky.D * Exp( Sky.E * gamma ) + Sky.F 
								  * ( cos_gamma * cos_gamma ) + Sky.G * chi + Sky.I * sqrt( max( 0.f, cos_theta ) ) );
	}
