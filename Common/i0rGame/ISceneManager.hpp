#pragma once

class Actor;
class Camera;
class Mesh;

struct scene_node_t;
struct spawn_point_t;

#include <i0rRender/Material.hpp>
#include <i0rRender/Lighting/PointLight.hpp>

class ISceneManager {
	DECLARE_MEMBER( protected, std::vector<Mesh*>,          Meshes )
	DECLARE_MEMBER( protected, std::vector<scene_node_t*>,  Nodes )
	DECLARE_MEMBER( protected, std::vector<spawn_point_t*>, Spawns )
	DECLARE_MEMBER( protected, std::vector<point_light_t*>, PointLights )
	DECLARE_MEMBER( protected, Actor*,                      LocalActor )
	DECLARE_MEMBER( protected, Camera*,                     CameraFirstPerson )
	DECLARE_MEMBER( protected, Camera*,                     CameraEditor )
	DECLARE_MEMBER( protected, Camera*,                     ActiveCamera )
	DECLARE_MEMBER( protected, bool,                        IsDevEditor )
	DECLARE_MEMBER( protected, bool,                        DisableVisibilityTest )

	public:
		virtual ~ISceneManager() {}
		virtual void Create( const bool displayEditorGrid = false )  = 0;
		virtual void Clear()                                         = 0;
		virtual void OnTick()                                        = 0;
		virtual void ComputeVisibility()                             = 0;
		virtual void CreateLocalActor()                              = 0;
		virtual void AddNode( Mesh* mesh )                           = 0;

		INLINE void AddPointLight( point_light_t* pointLight ) {
			m_PointLights.push_back(pointLight);
		}

		INLINE void AddSpawn( spawn_point_t* spawnPoint ) {
			m_Spawns.push_back(spawnPoint);
		}

		INLINE void AddMaterial( material_t* material ) {
			m_Materials[material->Hashcode] = material;
		}

		INLINE material_t* GetMaterial( const u32 hash ) {
			return m_Materials[hash];
		}

		struct scene_informations_t {
			vec3f                          SceneDimensionsMin;
			// u32                         __PADDING__;
			vec3f                          SceneDimensionsMax;
			// u32                         __PADDING__;
			u8                             __PADDING__[12];
		} Informations;

	std::unordered_map<u32, material_t*> m_Materials;
};