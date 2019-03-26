#pragma once

class IShader;

class IShaderManager {
	public:
		virtual ~IShaderManager() {}
		virtual bool Initialize()                                                                       = 0;
		virtual IShader* GetShader( std::string shaderName, const u32 shaderStages,
								                     std::vector<std::string> shaderFlags = std::vector<std::string>() )  = 0;
		virtual void BuildMaterialShader( IShader* &shader, std::vector<std::string> flags)               = 0;
		virtual void BuildEnvProbeShader( IShader* &shader, std::vector<std::string> flags )             = 0;
		virtual void BuildDepthShader( IShader* &shader, std::vector<std::string> flags )                = 0;

		#ifdef FLAG_DEBUG
				virtual void Reload( const std::string &shaderToReload )                                      = 0;
				virtual void WatchShader( const std::string &shaderName, IShader* shader )                     = 0;
		#endif
};