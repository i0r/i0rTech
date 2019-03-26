#include "../Common.hpp"
#include "PrimitiveEditorGrid.hpp"

IIndiceBuffer*   EditorGrid::VAO         = nullptr;
IVertexBuffer*   EditorGrid::VBO         = nullptr;
mat4x4f          EditorGrid::ModelMatrix = mat4x4f( 0.0f );
IShader*         EditorGrid::ShaderMrt   = nullptr;
Itexture_t*      EditorGrid::Texture     = nullptr;
material_data_t* EditorGrid::Material    = nullptr;

void EditorGrid::Create() {
	const vertex_buffer_element_t planePosition = {
		3,
		{  
					Instance.Configuration->Graphics.FarPlane, 0.0f,  Instance.Configuration->Graphics.FarPlane,
					Instance.Configuration->Graphics.FarPlane, 0.0f, -Instance.Configuration->Graphics.FarPlane,
				-Instance.Configuration->Graphics.FarPlane, 0.0f, -Instance.Configuration->Graphics.FarPlane,
				-Instance.Configuration->Graphics.FarPlane, 0.0f,  Instance.Configuration->Graphics.FarPlane,
					Instance.Configuration->Graphics.FarPlane, 0.0f,  Instance.Configuration->Graphics.FarPlane,
				-Instance.Configuration->Graphics.FarPlane, 0.0f, -Instance.Configuration->Graphics.FarPlane,
		}
	},

	texCoordinates = { 
		2,
		{ 
			1.0f, 0.0f, 
			1.0f, 1.0f, 
			0.0f, 1.0f, 
			0.0f, 0.0f, 
			1.0f, 0.0f, 
			0.0f, 1.0f, 
		} 
	},

	normals = { 
		3, 
		{ 
			0.0f, 1.0f, 0.0f, 
			0.0f, 1.0f, 0.0f,
			0.0f, 1.0f, 0.0f, 
			0.0f, 1.0f, 0.0f, 
			0.0f, 1.0f, 0.0f, 
			0.0f, 1.0f, 0.0f 
		} 
	};

	VAO = Instance.GraphicsApiContext->CreateIndiceBuffer();
	VBO = Instance.GraphicsApiContext->CreateVertexBuffer();

	VAO->Bind();
	VBO->AddElement( planePosition );
	VBO->AddElement( texCoordinates );
	VBO->AddElement( normals );
	VBO->BufferElements( GENERIC_RENDER_STATIC_DRAW );
	VAO->BufferElements( { 0, 1, 2, 3, 4, 5 }, GENERIC_RENDER_STATIC_DRAW );
	VAO->Unbind();

	ModelMatrix.Identity();
	ShaderMrt = Instance.ShaderManager->GetShader( "Mrt", SHADER_STAGE_VERTEX | SHADER_STAGE_FRAGMENT, 
								                                       { "RELATIVE_POSITION", "TEXTURE_DIFFUSE" } );

	ShaderMrt->Bind();

	ShaderMrt->AddUniformBlock( "ubo_common_t");
	ShaderMrt->SetConstantUniformBufferIndex( "ubo_common_t", UBO_COMMON );

	ShaderMrt->AddAttribute( "uModel" );
	ShaderMrt->SetAttribute4x4fv( "uModel", ModelMatrix );

	ShaderMrt->AddAttribute( "uDiffuseSampler" );
	ShaderMrt->SetConstantAttribute1i( "uDiffuseSampler", 3 );

	ShaderMrt->AddAttribute( "uMaterialIndex" );
	ShaderMrt->SetConstantAttribute1i( "uMaterialIndex", 0 );

	ShaderMrt->AddAttribute( "uSunShadows" );
	ShaderMrt->SetConstantAttribute1i( "uSunShadows", 7 );

	ShaderMrt->AddUniformBlock( "ubo_materials_t" );
	ShaderMrt->SetConstantUniformBufferIndex( "ubo_materials_t", UBO_MATERIALS );

	ShaderMrt->AddUniformBlock( "ubo_shadows_t" );
	ShaderMrt->SetConstantUniformBufferIndex( "ubo_shadows_t", UBO_SHADOWS );

	ShaderMrt->AddUniformBlock( "ubo_sky_t" );
	ShaderMrt->SetConstantUniformBufferIndex( "ubo_sky_t", UBO_SKY );

	Texture = Instance.TextureManager->GetTexture2D( "Editor/E_grid" );
	Texture->FixedSlot = TEXTURE_BIND_GRID;
	Texture->Bind( TEXTURE_BIND_GRID );

	Material                = new material_data_t();
	Material->Alpha         = 255;
	Material->DiffuseColor  = colorrgbf( 1.0f );
	Material->DSSHEFactors  = vec4f( 0.800000012f, 0.498039216f, 0.0941176489f, 0.000000000f );
	Material->Flags         = 6;
	Material->Shading       = 17;
	Material->SpecularColor = colorrgbf( 1.0f );

	Materials.Materials[0]  = *Material;
}

void EditorGrid::Draw() {
	ShaderMrt->Bind();

	VAO->Bind();
	VAO->Draw();
	VAO->Unbind();

	Instance.GraphicsApiContext->UnbindShaderProgram();
}
