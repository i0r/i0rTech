#pragma once

bool GetStageCompileStatus( const u32 stage );
bool GetProgramLinkStatus( const u32 program );

struct shader_storage_buffer_t;

enum shader_constant_type_t {
	SHADER_CONSTANT_TYPE_INTEGER,
	SHADER_CONSTANT_TYPE_UBO,
	SHADER_CONSTANT_TYPE_SSBO,
};

struct shader_constant_attribute_t {
	i32                    Value;
	shader_constant_type_t Type;
};
		
#include <i0rRender/IShader.hpp>

class Shader : public IShader {
	public:
		Shader();
		~Shader();

		bool  Compile();

		void  Flush();
		void  OnRecompile();

		void  AddStage( const std::string uberStageFile, const u32 stageType,
								          std::vector<std::string>* stageFlags, const bool isReloading = false );

		virtual void  AddAttribute( const char* attributeName );
		virtual void  AddUniformBlock( const char* uniformBlockName );
		void  AddStorageBlock( const char* storageBlockName );

		virtual void  SetConstantAttribute1i( const char* attribute, const i32 value );
		virtual void  SetConstantUniformBufferIndex( const char* attribute, const i32 value );
		void  SetConstantStorageBufferIndex( const char* attribute, const i32 value );

		virtual void  SetAttribute1i( const char* attribute, const i32 value );
		
		void  SetAttribute1fv( const char* attribute, const f32* value );
		void  SetAttribute2fv( const char* attribute, const vec2f value );
		void  SetAttribute3fv( const char* attribute, const vec3f value );
		void  SetAttribute3fv( const char* attribute, const colorrgbf value );
		void  SetAttribute4fv( const char* attribute, vec4f value );
		void  SetAttribute4fv( const char* attribute, const colorrgbaf value );

		void  SetAttribute2iv( const char* attribute, const vec2i value );
		void  SetAttribute4iv( const char* attribute, const vec4i value );

		void  SetAttribute3x3fv( const char* attribute, const mat3x3f value );
		virtual void  SetAttribute4x4fv( const char* attribute, const mat4x4f value );

		void  SetUniformBufferIndex( const char* attribute, const u32 value );
		void  SetStorageBufferIndex( const char* attribute, const u32 value );

		INLINE void Bind() const {
			glUseProgram( m_Program );
		}

		INLINE u32 GetProgramID() const {
			return m_Program;
		}

		INLINE bool IsCompiled() const {
			return m_IsCompiled;
		}

		INLINE void SetProgramID( const u32 index ) {
			m_Program = index;
		}

	private:
		void                                                          DeleteStages();

		bool                                                          m_IsCompiled;

		u32                                                        m_Program;

		std::unordered_map<std::string, shader_constant_attribute_t>  m_Constants;

		std::unordered_map<std::string, u32>                     m_UniformsStorage;
		std::unordered_map<std::string, u32>                     m_UniformsBlock;
		std::unordered_map<std::string, u32>                     m_Uniforms;

		std::unordered_map<std::string, u32>::const_iterator     m_UniformIterator;

		u32                                                      m_ComputeStage;
		u32                                                      m_VertexStage;
		u32                                                      m_GeometryStage;
		u32                                                      m_FragmentStage;
		u32                                                      m_TessellationControlStage;
		u32                                                      m_TessellationEvaluationStage;
};
