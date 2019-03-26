#pragma once

#include <i0rGame/Spawn.hpp>
#include <i0rRender/Material.hpp>
#include <i0rRender/Lighting/PointLight.hpp>
#include <i0rRender/IRenderer.hpp>
#include <i0rRender/ITextureManager.hpp>
#include <i0rRender/Mesh.hpp>
#include <i0rRender/AABB.hpp>

#define SCENE_MAGIC               0x53523049
#define SCENE_SPAWN_MAGIC         0x4E575053
#define SCENE_DIR_LIGHT_MAGIC     0x5249444C
#define SCENE_POINT_LIGHT_MAGIC   0x544E504C
#define SCENE_MATERIAL_MAGIC      0x4554414D
#define SCENE_GEOM_MAGIC          0x4D4F4547
#define SCENE_TEXTURE_DICO        0x44584554
#define SCENE_INFOS               0x454E4353

#define SCENE_VERSION_MAJOR       1
#define SCENE_VERSION_MINOR       1
#define SCENE_VERSION_PATCH       0

struct file_header_its_t {
	u32 Magic;
	u8  VersionMajor;
	u8  VersionMinor;
	u8  VersionPatch;
	u8  __PADDING__[9];
};

struct file_header_its_material_t {
	u32       Hashcode;

	u8        ShadingType;
	u8        DiffuseFactor;
	u8        SpecularFactor;
	u8        SpecularHardnessFactor;

	u8        EmissiveFactor;
	u8        AmbientFactor;
	u8        AlphaFactor;
	u8        __PLACEHOLDER__;

	u32       Flags;
	u32       LayerCount;

	colorrgbf DiffuseColor;
	colorrgbf SpecularColor;

	u32       __PADDING__;
};

struct texture_entry_t {
	u32 Hashcode;
	u32 PathLength;
};

// note: implicit padding is commented
struct file_header_its_mesh_t {
	u32                       Hashcode;
	u32                       Parent;
	u32                       Material;
	u32                       LODCount;

	u32                       Features;
	u8                        __RESERVED__[12];

	vec3f                     BoundBoxPosition;
	//u32                     __PADDING__;

	vec3f                     BoundBoxDimensions;
	//u32                     __PADDING__;

	vec3f                     WorldTranslation;
	//u32                     __PADDING__;

	quatf                     WorldOrientation;

	vec3f                     WorldScale;
	f32                       Weight;

	f32                       Resistance;
	u64                       __RESERVED_PHYSICS__;
	//u32                     __PADDING__;

	vec3f                     Velocity;
	u32                       __PADDING__;
};

struct file_header_its_mesh_lod_t {
	f32  LOD;
	u32  VBOSize;
	u32  VAOSize;
	u32  __PADDING__;
};

typedef std::map<u32, std::string> texture_dictionary_t;

class FSceneStatic {
	public:
		static void LoadSceneSpawnPoints( file_bloc_generic_t* bloc, char* ptr, ISceneManager* activeScene ) {
			spawn_point_t* spawnPoint = nullptr;

			for( u32 i = 0; i < bloc->Size; ) {
				spawnPoint = new spawn_point_t();
				*spawnPoint = FILE_READ_RECURSIVE( spawn_point_t );
				activeScene->AddSpawn( spawnPoint );
			}
		}

		static void LoadScenePointLights( file_bloc_generic_t* bloc, char* ptr, ISceneManager* activeScene ) {
			point_light_t* pointLight = nullptr;

			for( u32 i = 0; i < bloc->Size; ) {
				pointLight = new point_light_t();
				*pointLight = FILE_READ_RECURSIVE( point_light_t );

				activeScene->AddPointLight( pointLight );
			}
		}

		static void LoadSceneTextures( file_bloc_generic_t* meshBloc, char* ptr, texture_dictionary_t &dictionary ) {
			texture_entry_t* textureEntry = new texture_entry_t();
			char* texturePath             = new char[MAX_PATH];
			u32 i = 0;

			for( ; i < meshBloc->Size; ) {
				*textureEntry = FILE_READ_RECURSIVE( texture_entry_t );

				memset( texturePath, 0,   MAX_PATH );
				memcpy( texturePath, ptr, textureEntry->PathLength );

				ptr += textureEntry->PathLength * sizeof(char);
				i   += textureEntry->PathLength * sizeof(char);

				dictionary.insert( std::make_pair( textureEntry->Hashcode, texturePath ) );
			}

			delete[] texturePath;
		}

		static void LoadSceneMaterials( file_bloc_generic_t* meshBloc, char* ptr, ISceneManager* activeScene, 
								                          texture_dictionary_t &dictionary ) {
			file_header_its_material_t materialHeader = {};
			texture_slot_t textureSlot                = {};
			material_t* material                      = nullptr;

			for( u32 i = 0; i < meshBloc->Size; ) {
				material       = new material_t();
				materialHeader = FILE_READ_RECURSIVE( file_header_its_material_t );

				material->Hashcode = materialHeader.Hashcode;
				material->Index    = Instance.Renderer->AllocateMaterialSlot( material->Data );

				material->Data->Flags   = materialHeader.Flags;
				material->Data->Shading = materialHeader.ShadingType;
				material->Data->Alpha   = materialHeader.AlphaFactor;

				material->Data->DSSHEFactors.x = ( f32 )materialHeader.DiffuseFactor          / 255.0f;
				material->Data->DSSHEFactors.y = ( f32 )materialHeader.SpecularFactor         / 255.0f;
				material->Data->DSSHEFactors.z = ( f32 )materialHeader.SpecularHardnessFactor / 255.0f;
				material->Data->DSSHEFactors.w = ( f32 )materialHeader.EmissiveFactor         / 255.0f;

				material->Data->DiffuseColor  = materialHeader.DiffuseColor;
				material->Data->SpecularColor = materialHeader.SpecularColor;

				for( u32 j = 0; j < materialHeader.LayerCount; ++j ) {
					textureSlot = FILE_READ_RECURSIVE( texture_slot_t );

					std::string textureName = dictionary[textureSlot.Hashcode];
					textureName             = RemoveFileExtension(textureName);

					material->Layers.insert( std::make_pair( textureSlot.Layer, Instance.TextureManager->GetTexture2D( textureName ) ) );
				}

				ReadPadding( ptr, i, 16 );

				material->Build();

				activeScene->AddMaterial( material );
			}
		}

		static void LoadSceneMeshes( file_bloc_generic_t* meshBloc, char* ptr, ISceneManager* scene ) {
			// 16 bytes must be removed (implicit padding)
			const std::size_t headerSize = sizeof( file_header_its_mesh_t ) - 16;

			file_header_its_mesh_t meshHeader = {};
			Mesh* mesh                        = nullptr;

			for( u32 i = 0; i < meshBloc->Size; ) {
				meshHeader = FILE_READ_SIZE_RECURSIVE( file_header_its_mesh_t, headerSize );

				mesh = new Mesh( meshHeader.Hashcode );

				mesh->m_BoundingBox.Dimensions    = meshHeader.BoundBoxDimensions;
				mesh->m_BoundingBox.WorldPosition = meshHeader.BoundBoxPosition;

				mesh->m_WorldOrientation = meshHeader.WorldOrientation;
				mesh->m_WorldTranslation = meshHeader.WorldTranslation;
				mesh->m_WorldScale       = meshHeader.WorldScale;
				mesh->UpdateModelMatrix();

				mesh->m_Material = scene->GetMaterial( meshHeader.Material );
				
				// read buffer and upload data to the gpu using current graphics api calls
				Instance.Renderer->AllocateMeshData( ptr, i, meshHeader.LODCount, meshHeader.Features, mesh );
	
				if( meshHeader.LODCount <= 1 ) {
					CONSOLE_PRINT_WARNING( "LoadSceneMeshes => [OPTIMIZATION]0x%X : no LOD available\n", meshHeader.Hashcode );
				}

				scene->AddNode( mesh );
			}
		}
};
