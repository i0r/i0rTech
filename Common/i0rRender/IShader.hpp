#pragma once

#define SHADER_STAGE_VERTEX        0x01
#define SHADER_STAGE_TESS_CONTROL  0x02
#define SHADER_STAGE_TESS_EVAL     0x04
#define SHADER_STAGE_GEOMETRY      0x08
#define SHADER_STAGE_COMPUTE       0x10
#define SHADER_STAGE_FRAGMENT      0x20

class IShader {
	DECLARE_MEMBER( protected, bool, IsCompiled )
	DECLARE_MEMBER( protected, u32,  Program )

	public:
		virtual ~IShader() {
			m_IsCompiled = false;
		};

		virtual bool  Compile() = 0;

		virtual void  Flush() = 0;
		virtual void  OnRecompile() = 0;

		virtual void  AddStage( const std::string uberStageFile, const u32 stageType, std::vector<std::string>* stageFlags, 
								                  const bool isReloading = false ) = 0;

		virtual void  AddAttribute( const char* attributeName ) = 0;
		virtual void  AddUniformBlock( const char* uniformBlockName ) = 0;
		virtual void  AddStorageBlock( const char* storageBlockName ) = 0;

		virtual void  SetConstantAttribute1i( const char* attribute, const i32 value ) = 0;
		virtual void  SetConstantUniformBufferIndex( const char* attribute, const i32 value ) = 0;
		virtual void  SetConstantStorageBufferIndex( const char* attribute, const i32 value ) = 0;

		virtual void  SetAttribute1i( const char* attribute, const i32 value ) = 0;
		
		virtual void  SetAttribute1fv( const char* attribute, const f32* value ) = 0;
		virtual void  SetAttribute2fv( const char* attribute, const vec2f value ) = 0;
		virtual void  SetAttribute3fv( const char* attribute, const vec3f value ) = 0;
		virtual void  SetAttribute3fv( const char* attribute, const colorrgbf value ) = 0;
		virtual void  SetAttribute4fv( const char* attribute, vec4f value ) = 0;
		virtual void  SetAttribute4fv( const char* attribute, const colorrgbaf value ) = 0;

		virtual void  SetAttribute2iv( const char* attribute, const vec2i value ) = 0;
		virtual void  SetAttribute4iv( const char* attribute, const vec4i value ) = 0;

		virtual void  SetAttribute3x3fv( const char* attribute, const mat3x3f value ) = 0;
		virtual void  SetAttribute4x4fv( const char* attribute, const mat4x4f value ) = 0;

		virtual void  SetUniformBufferIndex( const char* attribute, const u32 value ) = 0;
		virtual void  SetStorageBufferIndex( const char* attribute, const u32 value ) = 0;

		virtual void Bind() const = 0;

		virtual bool IsCompiled() const = 0;

		virtual u32 GetProgramID() const = 0;
		virtual void SetProgramID( const u32 index ) = 0;
};
