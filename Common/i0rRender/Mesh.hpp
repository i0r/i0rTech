#pragma once

#include "AABB.hpp"
#include "Lod.hpp"
#include "IGraphicsApiContext.hpp"

struct material_t;
struct environment_probe_t;

enum mesh_feature_t {
	MESH_FEATURE_UV_MAP         = 1 << 0,
	MESH_FEATURE_NORMAL_MAPPING = 1 << 1,
};

class Mesh {
	DECLARE_MEMBER( private, std::vector<lod_t*>, Lods )
	DECLARE_MEMBER( private, std::string,         Name )

	public:
		explicit Mesh( const u32 meshHashcode, const std::string meshName = "Mesh" )  : m_Name( meshName ),
								                                                                                  m_ModelMatrix(),
								                                                                                  m_WorldOrientation(), 
								                                                                                  m_WorldTranslation(),
								                                                                                  m_WorldScale(),
								                                                                                  m_BoundingBox(),
								                                                                                  m_Material( nullptr ),
								                                                                                  m_Hashcode( meshHashcode ) {
			m_ModelMatrix.Identity();
		};
		
		~Mesh() {
			for( lod_t* lod : m_Lods ) {
				SAFE_DELETE( lod );
			}

			m_Lods.clear();

			m_ModelMatrix      = mat4x4f();
			m_WorldOrientation = quatf();
			m_WorldTranslation = vec3f();
			m_WorldScale       = vec3f();

			m_BoundingBox.~aabb_t();
			m_Material         = nullptr;
			m_Hashcode         = 0;
		};

		void UpdateModelMatrix() {
			m_ModelMatrix.Identity(); // resets matrix each update

			m_ModelMatrix.Translate( m_WorldTranslation );
			m_ModelMatrix.Rotate( m_WorldOrientation );
			m_ModelMatrix.Scale( m_WorldScale );
		}

		INLINE void AddLod( lod_t* lod ) {
			m_Lods.push_back( lod );
		}

		mat4x4f                              m_ModelMatrix;
		quatf                                m_WorldOrientation;
		vec3f                                m_WorldTranslation;
		vec3f                                m_WorldScale;
		aabb_t                               m_BoundingBox;
		material_t*                          m_Material;
		u32                                  m_Hashcode;
};
