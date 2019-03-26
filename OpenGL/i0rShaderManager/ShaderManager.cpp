#include "../Common.hpp"
#include "Shader.hpp"
#include "ShaderManager.hpp"

ShaderManager::ShaderManager()  : m_DefaultShader( nullptr ) {

}

ShaderManager::~ShaderManager() {
	for( std::pair<u32, IShader*> shader : m_CachedShaders ) {
		shader.first = 0;
		SAFE_DELETE( shader.second )
	}

	m_CachedShaders.clear();
	m_DefaultShader = nullptr;
}


	bool ShaderManager::Initialize() {
		m_DefaultShader = GetShader( "Default", SHADER_STAGE_VERTEX | SHADER_STAGE_FRAGMENT );
		return m_DefaultShader != nullptr;
	}


IShader* ShaderManager::GetShader( std::string shaderName, u32 shaderStages, std::vector<std::string> shaderFlags ) {
	u32 shaderHash = HashStringCRC32( shaderName + TextFileToString( shaderFlags ) );
	shaderHash ^= shaderStages;

	std::unordered_map<u32, IShader*>::const_iterator it = m_CachedShaders.find( shaderHash );

	if( it != m_CachedShaders.end() ) {
		return it->second;
	}

	Shader* shader = new Shader();

#if defined( FLAG_DEBUG )
	shader_watch_t watch = {
		shaderName,
		shaderStages,
		shaderFlags,
		shader
	};

	m_WatchedShaders[shaderName].push_back( watch );
#endif

	if( shaderStages & SHADER_STAGE_COMPUTE ) {
			shader->AddStage( uberShaderFolder + shaderName + 
								             stageComputeExtension, GL_COMPUTE_SHADER, 
								             &shaderFlags );
	} else {
		if( shaderStages & SHADER_STAGE_VERTEX ) {
			shader->AddStage( uberShaderFolder + shaderName + 
								             stageVertexExtension, GL_VERTEX_SHADER, 
								             &shaderFlags );
		}

		if( shaderStages & SHADER_STAGE_TESS_CONTROL ) {
			shader->AddStage( uberShaderFolder + shaderName + 
								             stageTessControlExtension, GL_TESS_CONTROL_SHADER, 
								             &shaderFlags );
		}

		if( shaderStages & SHADER_STAGE_TESS_EVAL ) {
			shader->AddStage( uberShaderFolder + shaderName + 
								             stageTessEvalExtension, GL_TESS_EVALUATION_SHADER, 
								             &shaderFlags );
		}

		if( shaderStages & SHADER_STAGE_GEOMETRY ) {
			shader->AddStage( uberShaderFolder + shaderName + 
								             stageGeometryExtension, GL_GEOMETRY_SHADER, 
								             &shaderFlags );
		}

		if( shaderStages & SHADER_STAGE_FRAGMENT ) {
			shader->AddStage( uberShaderFolder + shaderName + 
								             stageFragmentExtension, GL_FRAGMENT_SHADER, 
								             &shaderFlags );
		}
	}

	if( !shader->Compile() ) {
		CONSOLE_PRINT_ERROR( "%s => Failed to compile shader %s\n", 
								               __FUNCTION__,
								               shaderName.c_str() );
		SAFE_DELETE( shader )
		return m_DefaultShader;
	}

	m_CachedShaders.insert( std::make_pair( shaderHash, shader ) );
	return shader;
}

void ShaderManager::BuildEnvProbeShader( IShader* &shader, std::vector<std::string> flags ) {
	shader = Instance.ShaderManager->GetShader( "EnvProbe", SHADER_STAGE_VERTEX | SHADER_STAGE_FRAGMENT, flags );

	shader->Bind();
	shader->AddUniformBlock( "ubo_common_t" );
	shader->SetConstantUniformBufferIndex( "ubo_common_t", UBO_COMMON );

	shader->AddUniformBlock( "ubo_materials_t" );
	shader->SetConstantUniformBufferIndex( "ubo_materials_t", UBO_MATERIALS );

	shader->AddAttribute( "uDirection" );
	shader->AddAttribute( "uModel" );
	shader->AddAttribute( "uView" );
	shader->AddAttribute( "uMaterialIndex" );

	if( std::find( flags.begin(), flags.end(), "TEXTURE_DIFFUSE" ) != flags.end() ) {
		shader->AddAttribute( "uDiffuseSampler" );
		shader->SetConstantAttribute1i( "uDiffuseSampler", 8 );
	}

	if( std::find( flags.begin(), flags.end(), "TEXTURE_METALNESS" ) != flags.end() ) {
		shader->AddAttribute( "uMetalnessSampler" );
		shader->SetConstantAttribute1i( "uMetalnessSampler", 9 );
	}

	if( std::find( flags.begin(), flags.end(), "TEXTURE_NORMAL" ) != flags.end() ) {
		shader->AddAttribute( "uNormalSampler" );
		shader->SetConstantAttribute1i( "uNormalSampler", 10 );
	}

	if( std::find( flags.begin(), flags.end(), "TEXTURE_ROUGHNESS" ) != flags.end() ) {
		shader->AddAttribute( "uRoughnessSampler" );
		shader->SetConstantAttribute1i( "uRoughnessSampler", 11 );
	}

	if( std::find( flags.begin(), flags.end(), "TEXTURE_AMBIENT" ) != flags.end() ) {
		shader->AddAttribute( "uAmbientSampler" );
		shader->SetConstantAttribute1i( "uAmbientSampler", 13 );
	}
}

void ShaderManager::BuildMaterialShader( IShader* &shader, std::vector<std::string> flags ) {
	shader = Instance.ShaderManager->GetShader("Mrt", SHADER_STAGE_VERTEX | SHADER_STAGE_FRAGMENT, flags);

	shader->Bind();
	shader->AddUniformBlock("ubo_common_t");
	shader->SetConstantUniformBufferIndex("ubo_common_t", UBO_COMMON );

	shader->AddUniformBlock("ubo_materials_t");
	shader->SetConstantUniformBufferIndex("ubo_materials_t", UBO_MATERIALS );

	shader->AddUniformBlock( "ubo_shadows_t" );
	shader->SetConstantUniformBufferIndex( "ubo_shadows_t", UBO_SHADOWS );

	shader->AddUniformBlock( "ubo_sky_t" );
	shader->SetConstantUniformBufferIndex( "ubo_sky_t", UBO_SKY );

	shader->AddAttribute( "uSunShadows" );
	shader->SetConstantAttribute1i( "uSunShadows", 7 );

	shader->AddAttribute( "uModel" );
	shader->AddAttribute( "uMaterialIndex" );

	if ( std::find( flags.begin(), flags.end(), "TEXTURE_DIFFUSE" ) != flags.end() ) {
		shader->AddAttribute( "uDiffuseSampler" );
		shader->SetConstantAttribute1i( "uDiffuseSampler", 8 );
	}

	if (std::find(flags.begin(), flags.end(), "TEXTURE_METALNESS") != flags.end()) {
		shader->AddAttribute("uMetalnessSampler");
		shader->SetConstantAttribute1i("uMetalnessSampler", 9);
	}

	if (std::find(flags.begin(), flags.end(), "TEXTURE_NORMAL") != flags.end()) {
		shader->AddAttribute("uNormalSampler");
		shader->SetConstantAttribute1i("uNormalSampler", 10);
	}

	if (std::find(flags.begin(), flags.end(), "TEXTURE_ROUGHNESS") != flags.end()) {
		shader->AddAttribute("uRoughnessSampler");
		shader->SetConstantAttribute1i("uRoughnessSampler", 11);
	}

	if( std::find( flags.begin(), flags.end(), "TEXTURE_AMBIENT" ) != flags.end() ) {
		shader->AddAttribute( "uAmbientSampler" );
		shader->SetConstantAttribute1i( "uAmbientSampler", 13 );
	}
}

void ShaderManager::BuildDepthShader( IShader* &shader, std::vector<std::string> flags ) {
	shader = Instance.ShaderManager->GetShader( "RenderDepth", SHADER_STAGE_VERTEX | SHADER_STAGE_FRAGMENT, flags );

	shader->Bind();
	shader->AddAttribute( "uModel" );
	shader->AddAttribute( "uViewProjection" );
}

#if defined( FLAG_DEBUG )
void ShaderManager::Reload( const std::string &shaderToReload ) {
	std::string fileOnly = shaderToReload;
	RemoveWord( fileOnly, uberShaderFolder );
	fileOnly = RemoveFileExtension( fileOnly );

	RemoveWord( fileOnly, "Fragment" );
	RemoveWord( fileOnly, "Vertex" );
	RemoveWord( fileOnly, "Geometry" );
	RemoveWord( fileOnly, "Compute" );
	RemoveWord( fileOnly, "TessellationControl" );
	RemoveWord( fileOnly, "TessellationEvaluation" );

	for( shader_watch_t shader : m_WatchedShaders[fileOnly] ) {
		shader.Shader->Flush();

		if( shader.Stages & SHADER_STAGE_COMPUTE ) {
			shader.Shader->AddStage( uberShaderFolder + shader.Name +
				stageComputeExtension, GL_COMPUTE_SHADER,
				&shader.Flags, true );
		} else {
			if( shader.Stages & SHADER_STAGE_VERTEX ) {
				shader.Shader->AddStage( uberShaderFolder + shader.Name +
					stageVertexExtension, GL_VERTEX_SHADER,
					&shader.Flags, true );
			}

			if( shader.Stages & SHADER_STAGE_TESS_CONTROL ) {
				shader.Shader->AddStage( uberShaderFolder + shader.Name +
					stageTessControlExtension,
					GL_TESS_CONTROL_SHADER, &shader.Flags,
					true );
			}

			if( shader.Stages & SHADER_STAGE_TESS_EVAL ) {
				shader.Shader->AddStage( uberShaderFolder + shader.Name +
					stageTessEvalExtension,
					GL_TESS_EVALUATION_SHADER,
					&shader.Flags, true );
			}

			if( shader.Stages & SHADER_STAGE_GEOMETRY ) {
				shader.Shader->AddStage( uberShaderFolder + shader.Name +
					stageGeometryExtension,
					GL_GEOMETRY_SHADER, &shader.Flags, true );
			}

			if( shader.Stages & SHADER_STAGE_FRAGMENT ) {
				shader.Shader->AddStage( uberShaderFolder + shader.Name +
					stageFragmentExtension,
					GL_FRAGMENT_SHADER, &shader.Flags, true );
			}
		}

		if( !shader.Shader->Compile() ) {
			CONSOLE_PRINT_ERROR( "%s => Failed to reload shader %s\n",
				__FUNCTION__,
				shader.Name.c_str() );
			shader.Shader->SetProgramID( m_DefaultShader->GetProgramID() );
			return; // do not flush attribs (or a engine restart is required)
		}

		shader.Shader->OnRecompile();
	}
}
#endif
