#pragma once

#include "../i0rCore/IMemoryManager.hpp"
#include "../i0rCore/MemoryPage.hpp"

#include "IShaderManager.hpp"
#include "ITexture.hpp"

class Shader;

enum material_flags_t {
	MATERIAL_FLAG_IS_VISIBLE            = 0x1 << 0,
	MATERIAL_FLAG_CAST_SHADOW           = 0x1 << 1,
	MATERIAL_FLAG_RECEIVE_SHADOW        = 0x1 << 2,
	MATERIAL_FLAG_CAST_REFLECTION       = 0x1 << 3,
	MATERIAL_FLAG_IS_ALPHA_TESTED       = 0x1 << 4,
	MATERIAL_FLAG_IS_GLOBAL_ILLUMINATED = 0x1 << 5,
	MATERIAL_FLAG_IS_STATIC_ENVMAP      = 0x1 << 6,
	MATERIAL_FLAG_IS_DYNAMIC_ENVMAP     = 0x1 << 7,
	MATERIAL_FLAG_IS_PLANAR_REFLECTIONS = 0x1 << 8,
	MATERIAL_FLAG_IS_DECAL              = 0x1 << 9,
};

enum texture_layer_t : i32 {
	TEXTURE_LAYER_DIFFUSE   = 0,
	TEXTURE_LAYER_METALNESS = 1,
	TEXTURE_LAYER_NORMAL    = 2,
	TEXTURE_LAYER_ROUGHNESS = 3,
	TEXTURE_LAYER_AMBIENT   = 5,

	TEXTURE_LAYER_COUNT,
};

static const char* texture_layer_string[TEXTURE_LAYER_COUNT] = {
	"uDiffuseSampler",
	"uMetalnessSampler",
	"uNormalSampler",
	"uRoughnessSampler",
	"uAmbientSampler",
};

// data formatted to fit in gpu memory
struct material_data_t {
	~material_data_t() {
		DSSHEFactors  = vec4f();
		Flags         = 0;
		Shading       = 0;
		Alpha         = 0;
		DiffuseColor  = colorrgbf();
		SpecularColor = colorrgbf();
	}
	
	vec4f     DSSHEFactors;   // DiffuseSpecularSpecularHardnessEmissive
	u32       Flags;
	u32       Shading;
	u32       Alpha;
	u32       __PLACEHOLDER__;
	colorrgbf DiffuseColor;
	u32       __PADDINGDIFFUSE__;
	colorrgbf SpecularColor;
	u32       __PADDINGSPECULAR__;
};

struct texture_slot_t {
	u32              Hashcode;
	texture_layer_t  Layer;
	u8               DiffuseFactor;
	u8               __PLACEHOLDER__[3];
};

struct material_t {
	~material_t() {
		Hashcode = 0;
		Index    = 0;

		Data->~material_data_t();
		RENDER_ALLOCATED_FREE( Data );
		Data = nullptr;

		ShaderDefault  = nullptr;
		ShaderEnvProbe = nullptr;
		ShaderDepth    = nullptr;

		for( std::pair<u32, Itexture_t*> it : Layers ) {
			it.first = 0;
			SAFE_DELETE( it.second );
		}

		Layers.clear();
	}
	
	void Build() {
		std::vector<std::string> shaderFlags = {};

		if( Layers[TEXTURE_LAYER_DIFFUSE] ) {
			shaderFlags.push_back( "TEXTURE_DIFFUSE" );
		}

		if( Layers[TEXTURE_LAYER_NORMAL] ) {
			shaderFlags.push_back( "TEXTURE_NORMAL" );
		}

		if( Layers[TEXTURE_LAYER_METALNESS] ) {
			shaderFlags.push_back( "TEXTURE_METALNESS" );
		}

		if( Layers[TEXTURE_LAYER_ROUGHNESS] ) {
			shaderFlags.push_back( "TEXTURE_ROUGHNESS" );
		}

		if( Layers[TEXTURE_LAYER_AMBIENT] ) {
			shaderFlags.push_back( "TEXTURE_AMBIENT" );
		}

		if( Data->Flags & MATERIAL_FLAG_IS_DECAL ) {
			shaderFlags.push_back( "DECAL" );
			Instance.ShaderManager->BuildMaterialShader( ShaderDefault, shaderFlags );

			ShaderEnvProbe = nullptr;
			ShaderDepth    = nullptr;
		} else {
			Instance.ShaderManager->BuildMaterialShader( ShaderDefault, shaderFlags );
			Instance.ShaderManager->BuildEnvProbeShader( ShaderEnvProbe, shaderFlags );
			Instance.ShaderManager->BuildDepthShader( ShaderDepth, shaderFlags );
		}
	}

	u32                        Hashcode;
	u32                        Index; // UBO INDEX
	material_data_t*           Data;
	IShader*                   ShaderDefault;
	IShader*                   ShaderEnvProbe;
	IShader*                   ShaderDepth;
	std::map<u32, Itexture_t*> Layers;
};
