#include "../Common.hpp"
#include "UberShader.hpp"
#include "Shader.hpp"

bool GetStageCompileStatus( const u32 stage ) {
	i32 linkStatus = 0, logLength = 0;

	glGetShaderiv( stage, GL_COMPILE_STATUS, &linkStatus );
	glGetShaderiv( stage, GL_INFO_LOG_LENGTH, &logLength );

	if( logLength ) {
		std::vector<char> debugLog( logLength );
		glGetShaderInfoLog( stage, logLength, &logLength, &debugLog[0] );

		CONSOLE_PRINT_ALL( "***STAGE COMPILATION INFORMATIONS***\n" );
		CONSOLE_PRINT_ALL( debugLog.data() );
	}

	return ( linkStatus == GENERIC_RENDER_TRUE ) ? true : false;
}

bool GetProgramLinkStatus( const u32 program ) {
	i32 linkStatus = 0, logLength = 0;

	glGetProgramiv( program, GL_LINK_STATUS, &linkStatus );
	glGetProgramiv( program, GL_INFO_LOG_LENGTH, &logLength );

	if( logLength ) {   
		std::vector<char> debugLog( logLength );

		glGetProgramInfoLog( program, logLength, &logLength, &debugLog[0] );
		CONSOLE_PRINT_ALL( "***PROGRAM LINKING INFORMATIONS***\n" );
		CONSOLE_PRINT_ALL( debugLog.data() );
	}

	return ( linkStatus == GENERIC_RENDER_TRUE ) ? true : false;
}

Shader::Shader() : m_VertexStage( 0 ), 
								           m_GeometryStage( 0 ), 
								           m_FragmentStage( 0 ),
								           m_TessellationControlStage( 0 ), 
								           m_TessellationEvaluationStage( 0 ), 
								           m_ComputeStage( 0 ),
								           m_IsCompiled( false ) {
	m_Program = glCreateProgram();
}

Shader::~Shader() {
	m_Uniforms.clear();
	m_UniformsStorage.clear();
	m_UniformsBlock.clear();

	DeleteStages();
	m_IsCompiled = false;

	if( m_Program ) {
		glDeleteProgram( m_Program );
		m_Program = 0;
	}
}

void Shader::AddStage( const std::string uberStageFile, const u32 stageType, std::vector<std::string>* stageFlags,
								               bool isReloading ) {
	char* sft = Instance.FileSystem->GetFileText( uberStageFile.c_str() ) ;
	if( !sft ) {
		CONSOLE_PRINT_ERROR( "UberStage %s is empty or does not exist\n", uberStageFile.c_str() );
		return;
	}

	std::vector<std::string> stageFileText = StringToStringArray( sft );

// preprocessing
	std::size_t data = std::string::npos;

	for( u32 i = 0; i < stageFileText.size(); i++ ) {
		data = stageFileText[i].find( "#include" );
		if (data != std::string::npos) {
			std::string fileName = stageFileText[i].substr( data + 9 );
		
			#if defined( FLAG_DEBUG )
				if( !isReloading ) {
					std::string newFileName = RemoveFileExtension( fileName );
					Instance.ShaderManager->WatchShader( newFileName, this );
				}
			#endif

			char* is = Instance.FileSystem->GetFileText( ( uberShaderFolder + fileName ).c_str() );
			if( !is ) {
				CONSOLE_PRINT_ERROR( "UberStage %s is empty or does not exist\n", ( uberShaderFolder + fileName ).c_str() );
				continue;
			}

			std::vector<std::string> includedShader = StringToStringArray( is );

			u32 lineAppend = i;

			stageFileText.insert( stageFileText.begin() + lineAppend++, "" ); // fix recursive inclusion
			for (const std::string &line : includedShader ) {
				stageFileText.insert( stageFileText.begin() + lineAppend++, line );
			}
		
		stageFileText[lineAppend].clear();
		includedShader.clear();
		Instance.FileSystem->FreeUncompressedFile( is );
		continue;
	}

		// once we reach the stage entry point, we wont find any preprocessor, so we can stop here
		data = stageFileText[i].find( "void main()" );
	
		if( data != std::string::npos ) {
			break;
		}
	}

	std::string shaderLine_ = TextFileToString(stageFileText);
	if( stageFlags && stageFlags->size() ) {
		std::size_t definitionIndex = shaderLine_.find( "\n" );

		if (definitionIndex == std::string::npos) {
			CONSOLE_PRINT_WARNING( "UberStage %s has no version preprocessor\n", uberStageFile.c_str() );
			definitionIndex = 0;
		} else {
			definitionIndex++; // jumps the first new line char
		}

		shaderLine_.insert( definitionIndex, "\r\n" ); // we insert at least one carriage return before injecting preprocessor

		for( std::string flag : *stageFlags ) {
			shaderLine_.insert( definitionIndex, "#define " + flag + "\r\n" );
		}
	}

	const char* shaderLine = shaderLine_.c_str();
	u32* stage = NULL;

	switch (stageType) {
		case GL_VERTEX_SHADER:
			stage = &m_VertexStage;
			break;

		case GL_FRAGMENT_SHADER:
			stage = &m_FragmentStage;
			break;

		case GL_GEOMETRY_SHADER:
			stage = &m_GeometryStage;
			break;

		case GL_TESS_CONTROL_SHADER:
			stage = &m_TessellationControlStage;
			break;

		case GL_TESS_EVALUATION_SHADER:
			stage = &m_TessellationEvaluationStage;
			break;

		case GL_COMPUTE_SHADER:
			stage = &m_ComputeStage;
			break;

		default:
			CONSOLE_PRINT_WARNING( "%s() => Unknown stage type 0x%X\n", __FUNCTION__, stageType );
			stage = &m_VertexStage; // let's assume it's a vertex stage...
			break;
	}

	*stage = glCreateShader( stageType );
	glShaderSource( *stage, 1, &shaderLine, NULL );
	glCompileShader( *stage );

	if (!GetStageCompileStatus( *stage )) {
		CONSOLE_PRINT_ERROR( "Failed to compile stage %s\n", uberStageFile.c_str() );
		return;
	}

	glAttachShader( m_Program, *stage );

	stageFileText.clear();
	Instance.FileSystem->FreeUncompressedFile( sft );
}

	void Shader::AddAttribute( const char* attributeName ) {
u32 uniform = glGetUniformLocation( m_Program, attributeName );

if ( uniform == -1 ) {
		// CONSOLE_PRINT_ERROR( "Couldn't add attribute %s\n", attributeName );
		return;
}

m_Uniforms.insert( std::make_pair( attributeName, uniform ) );
	}

	void Shader::AddUniformBlock( const char* uniformBlockName ) {
u32 uniform = glGetUniformBlockIndex( m_Program, uniformBlockName );

if ( uniform == -1 ) {
		CONSOLE_PRINT_ERROR( "Could not add uniform block %s (error code: %i)\n", uniformBlockName, glGetError() );
		return;
}

m_UniformsBlock.insert( std::make_pair( uniformBlockName, uniform ) );
	}

	void Shader::AddStorageBlock( const char* storageBlockName ) {
u32 uniform = glGetProgramResourceIndex( m_Program, GL_SHADER_STORAGE_BLOCK, storageBlockName );

if ( uniform == -1 ) {
		CONSOLE_PRINT_ERROR( "Couldn't add block %s\n", storageBlockName );
		return;
}

m_UniformsStorage.insert( std::make_pair( storageBlockName, uniform ) );
	}

	void Shader::SetAttribute1fv( const char* attribute, const f32* value ) {
m_UniformIterator = m_Uniforms.find( attribute );

if ( m_UniformIterator != m_Uniforms.end() ) {
		glUniform1fv( m_UniformIterator->second, 1, value );
}
	}

	void Shader::SetAttribute2fv( const char* attribute, const vec2f value ) {
m_UniformIterator = m_Uniforms.find( attribute );

if ( m_UniformIterator != m_Uniforms.end() ) {
		glUniform2fv( m_UniformIterator->second, 1, &value.x );
}
	}

	void Shader::SetAttribute2iv( const char* attribute, const vec2i value ) {
m_UniformIterator = m_Uniforms.find( attribute );

if ( m_UniformIterator != m_Uniforms.end() ) {
		glUniform2iv( m_UniformIterator->second, 1, &value.x );
}
	}

	void Shader::SetAttribute3fv( const char* attribute, const vec3f value ) {
m_UniformIterator = m_Uniforms.find( attribute );

if ( m_UniformIterator != m_Uniforms.end() ) {
		glUniform3fv( m_UniformIterator->second, 1, &value.x );
}
	}

	void Shader::SetAttribute3fv( const char* attribute, const colorrgbf value ) {
m_UniformIterator = m_Uniforms.find( attribute );

if ( m_UniformIterator != m_Uniforms.end() ) {
		glUniform3fv( m_UniformIterator->second, 1, &value.r );
}
	}

	void Shader::SetAttribute4fv( const char* attribute, const vec4f value ) {
m_UniformIterator = m_Uniforms.find( attribute );

if ( m_UniformIterator != m_Uniforms.end() ) {
		glUniform4fv( m_UniformIterator->second, 1, &value.x );
}
	}

	void Shader::SetAttribute4iv( const char* attribute, const vec4i value ) {
m_UniformIterator = m_Uniforms.find( attribute );

if ( m_UniformIterator != m_Uniforms.end() ) {
		glUniform4iv( m_UniformIterator->second, 1, &value.x );
}
	}

	void Shader::SetAttribute4fv( const char* attribute, const colorrgbaf value ) {
m_UniformIterator = m_Uniforms.find( attribute );

if ( m_UniformIterator != m_Uniforms.end() ) {
		glUniform4fv( m_UniformIterator->second, 1, &value.r );
}
	}
		
	void Shader::SetAttribute3x3fv( const char* attribute, const mat3x3f value ) {
m_UniformIterator = m_Uniforms.find( attribute );

if ( m_UniformIterator != m_Uniforms.end() ) {
		glUniformMatrix3fv( m_UniformIterator->second, 1, GENERIC_RENDER_FALSE, &value.r0.x );
}
	}

	void Shader::SetAttribute4x4fv( const char* attribute, const mat4x4f value ) {
m_UniformIterator = m_Uniforms.find( attribute );

if ( m_UniformIterator != m_Uniforms.end() ) {
		glUniformMatrix4fv( m_UniformIterator->second, 1, GENERIC_RENDER_FALSE, &value.r0.x );
}
	}

	void Shader::SetAttribute1i( const char* attribute, const i32 value ) {
m_UniformIterator = m_Uniforms.find( attribute );

if ( m_UniformIterator != m_Uniforms.end() ) {
		glUniform1i( m_UniformIterator->second, value );
}
	}

	void Shader::SetUniformBufferIndex( const char* attribute, const u32 value ) {
m_UniformIterator = m_UniformsBlock.find( attribute );
if( m_UniformIterator != m_UniformsBlock.end() ) {
		glUniformBlockBinding( m_Program, m_UniformIterator->second, value );
}
	}
		
	void Shader::SetStorageBufferIndex( const char* attribute, const u32 value ) {
m_UniformIterator = m_UniformsStorage.find( attribute );
if( m_UniformIterator != m_UniformsStorage.end() ) {
		glShaderStorageBlockBinding( m_Program, m_UniformIterator->second, value );
}
	}

	void Shader::SetConstantAttribute1i( const char* attribute, const i32 value ) {
SetAttribute1i( attribute, value );
m_Constants[attribute] = { value, SHADER_CONSTANT_TYPE_INTEGER };
	}

	void Shader::SetConstantUniformBufferIndex( const char* attribute, const i32 value ) {
SetUniformBufferIndex( attribute, value );
m_Constants[attribute] = { value, SHADER_CONSTANT_TYPE_UBO };
	}

	void Shader::SetConstantStorageBufferIndex( const char* attribute, const i32 value ) {
SetStorageBufferIndex( attribute, value );
m_Constants[attribute] = { value, SHADER_CONSTANT_TYPE_SSBO };
	}

	void Shader::DeleteStages() { 
if ( m_VertexStage ) {
		glDetachShader( m_Program, m_VertexStage );
		glDeleteShader( m_VertexStage );
		m_VertexStage = 0;
}

if ( m_GeometryStage ) {
		glDetachShader( m_Program, m_GeometryStage );
		glDeleteShader( m_GeometryStage );
		m_GeometryStage = 0;
}

if ( m_FragmentStage ) {
		glDetachShader( m_Program, m_FragmentStage );
		glDeleteShader( m_FragmentStage );
		m_FragmentStage = 0;
}

if ( m_TessellationControlStage ) {
		glDetachShader( m_Program, m_TessellationControlStage );
		glDeleteShader( m_TessellationControlStage );
		m_TessellationControlStage = 0;
}

if ( m_TessellationEvaluationStage ) {
		glDetachShader( m_Program, m_TessellationEvaluationStage );
		glDeleteShader( m_TessellationEvaluationStage );
		m_TessellationEvaluationStage = 0;
}

if ( m_ComputeStage ) {
		glDetachShader( m_Program, m_ComputeStage );
		glDeleteShader( m_ComputeStage );
		m_ComputeStage = 0;
}
	}

bool Shader::Compile() {
	glLinkProgram( m_Program );
	m_IsCompiled = GetProgramLinkStatus( m_Program );
	return m_IsCompiled;
}

void Shader::Flush() {
	DeleteStages();
	glDeleteProgram( m_Program );
	m_Program = glCreateProgram();
	m_IsCompiled = false;
}

void Shader::OnRecompile() {
	// get the new uniform location
	std::unordered_map<std::string, u32> tmpUniforms = m_Uniforms;
	m_Uniforms.clear();

	for( std::pair<std::string, u32> uniform : tmpUniforms ) {
		AddAttribute( uniform.first.c_str() );
	}

	tmpUniforms = m_UniformsBlock;
	m_UniformsBlock.clear();

	for( std::pair<std::string, u32> uniformBlock : tmpUniforms ) {
		AddUniformBlock( uniformBlock.first.c_str() );
	}

	tmpUniforms = m_UniformsStorage;
	m_UniformsStorage.clear();

	for( std::pair<std::string, u32> uniformStorage : tmpUniforms ) {
		AddStorageBlock( uniformStorage.first.c_str() );
	}

	std::unordered_map<std::string, shader_constant_attribute_t> tmpConstant = m_Constants;
	m_Constants.clear();

	for( std::pair<std::string, shader_constant_attribute_t> constant : tmpConstant ) {
		if( constant.second.Type == SHADER_CONSTANT_TYPE_INTEGER ) {
	SetConstantAttribute1i( constant.first.c_str(), constant.second.Value );
		} else if( constant.second.Type == SHADER_CONSTANT_TYPE_UBO ) {
	SetConstantUniformBufferIndex( constant.first.c_str(), constant.second.Value );
		} else if( constant.second.Type == SHADER_CONSTANT_TYPE_SSBO ) {
	SetConstantStorageBufferIndex( constant.first.c_str(), constant.second.Value );
		}
	}
}
