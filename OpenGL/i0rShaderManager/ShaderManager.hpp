#pragma once
#include "UberShader.hpp"

#include <i0rRender/IShaderManager.hpp>

class Shader;

class ShaderManager : public IShaderManager {
	public:
		explicit ShaderManager() ;
		~ShaderManager() ;

		bool Initialize() ;
		IShader* GetShader( std::string shaderName, const u32 shaderStages,
								              std::vector<std::string> shaderFlags = std::vector<std::string>() ) ;

		void BuildMaterialShader( IShader* &shader, std::vector<std::string> flags ) ;
		void BuildEnvProbeShader( IShader* &shader, std::vector<std::string> flags ) ;
		void BuildDepthShader( IShader* &shader, std::vector<std::string> flags ) ;

		void Reload( const std::string &shaderToReload ) ;

		#ifdef FLAG_DEBUG
				struct shader_watch_t {
					std::string              Name;
					uint32_t                 Stages;
					std::vector<std::string> Flags;
					IShader*                  Shader;
				};

				INLINE void WatchShader( const std::string &shaderName, IShader* shader ) {
					for( std::pair<std::string, std::vector<shader_watch_t>> watchVec : m_WatchedShaders ) {
						for( shader_watch_t watch : watchVec.second ) {
							if( watch.Shader == shader ) {
								m_WatchedShaders[shaderName].push_back( watch );
								return;
							}
						}
					}
				}
		#endif

	private:
		IShader*                                  m_DefaultShader;
		std::unordered_map<u32, IShader*>         m_CachedShaders;

	#if defined( FLAG_DEBUG )
		std::unordered_map<std::string, std::vector<shader_watch_t>>           m_WatchedShaders;
	#endif
};
