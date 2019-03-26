#pragma once

#include <i0rCore/Common.hpp>
#include <i0rRender/AABB.hpp>
#include <i0rMaths/Common.hpp>

struct frustum_t {
	struct {
		vec4f Left;
		vec4f Right;
		vec4f Top;
		vec4f Bottom;
		vec4f Far;
		vec4f Near;

		vec4f &operator[]( const i32 i ) {
			switch( i ) {
				case 0:  return Left;
				case 1:  return Right;
				case 2:  return Top;
				case 3:  return Bottom;
				case 4:  return Far;
				case 5:  return Near;
				default: return Near;
			}
		}
	} Planes;

	explicit frustum_t() {
		memset( &Planes, 0, sizeof( Planes ) );
	}

	~frustum_t() {
		memset( &Planes, 0, sizeof( Planes ) );
	}

	void Update( const mat4x4f &viewProjection ) {
		Planes.Right = Normalize( vec4f( viewProjection.r0.w - viewProjection.r0.x,
								                           viewProjection.r1.w - viewProjection.r1.x,
								                           viewProjection.r2.w - viewProjection.r2.x,
								                           viewProjection.r3.w - viewProjection.r3.x ) );

		Planes.Left = Normalize( vec4f( viewProjection.r0.w + viewProjection.r0.x,
								                          viewProjection.r1.w + viewProjection.r1.x,
								                          viewProjection.r2.w + viewProjection.r2.x,
								                          viewProjection.r3.w + viewProjection.r3.x ) );

		Planes.Bottom = Normalize( vec4f( viewProjection.r0.w + viewProjection.r0.y,
								                            viewProjection.r1.w + viewProjection.r1.y,
								                            viewProjection.r2.w + viewProjection.r2.y,
								                            viewProjection.r3.w + viewProjection.r3.y ) );

		Planes.Top = Normalize( vec4f( viewProjection.r0.w - viewProjection.r0.y,
								                         viewProjection.r1.w - viewProjection.r1.y,
								                         viewProjection.r2.w - viewProjection.r2.y,
								                         viewProjection.r3.w - viewProjection.r3.y ) );

		Planes.Far = Normalize( vec4f( viewProjection.r0.w - viewProjection.r0.z,
								                         viewProjection.r1.w - viewProjection.r1.z,
								                         viewProjection.r2.w - viewProjection.r2.z,
								                         viewProjection.r3.w - viewProjection.r3.z ) );

		Planes.Near = Normalize( vec4f( viewProjection.r0.w + viewProjection.r0.z,
								                          viewProjection.r1.w + viewProjection.r1.z,
								                          viewProjection.r2.w + viewProjection.r2.z,
								                          viewProjection.r3.w + viewProjection.r3.z ) );
	}

	bool IsBoxInFrustum( const aabb_t* boundingBox ) {
		const f32 boundingBoxSize = max( boundingBox->Dimensions.x, max( boundingBox->Dimensions.y, boundingBox->Dimensions.z ) ) / 2.0f;
		
		i32 c = 0;

		for( i32 p = 0; p < 6; ++p ) {
			c = 0;
			if( Planes[p][0] * ( boundingBox->WorldPosition.x - boundingBoxSize ) 
					+ Planes[p][1] * ( boundingBox->WorldPosition.y - boundingBoxSize ) 
					+ Planes[p][2] * ( boundingBox->WorldPosition.z - boundingBoxSize ) + Planes[p][3] > 0 )
				c++;
			if( Planes[p][0] * ( boundingBox->WorldPosition.x + boundingBoxSize ) 
					+ Planes[p][1] * ( boundingBox->WorldPosition.y - boundingBoxSize ) 
					+ Planes[p][2] * ( boundingBox->WorldPosition.z - boundingBoxSize ) + Planes[p][3] > 0 )
				c++;
			if( Planes[p][0] * ( boundingBox->WorldPosition.x - boundingBoxSize ) 
					+ Planes[p][1] * ( boundingBox->WorldPosition.y + boundingBoxSize ) 
					+ Planes[p][2] * ( boundingBox->WorldPosition.z - boundingBoxSize ) + Planes[p][3] > 0 )
				c++;
			if( Planes[p][0] * ( boundingBox->WorldPosition.x + boundingBoxSize ) 
					+ Planes[p][1] * ( boundingBox->WorldPosition.y + boundingBoxSize ) 
					+ Planes[p][2] * ( boundingBox->WorldPosition.z - boundingBoxSize ) + Planes[p][3] > 0 )
				c++;
			if( Planes[p][0] * ( boundingBox->WorldPosition.x - boundingBoxSize ) 
					+ Planes[p][1] * ( boundingBox->WorldPosition.y - boundingBoxSize ) 
					+ Planes[p][2] * ( boundingBox->WorldPosition.z + boundingBoxSize ) + Planes[p][3] > 0 )
				c++;
			if( Planes[p][0] * ( boundingBox->WorldPosition.x + boundingBoxSize ) 
					+ Planes[p][1] * ( boundingBox->WorldPosition.y - boundingBoxSize ) 
					+ Planes[p][2] * ( boundingBox->WorldPosition.z + boundingBoxSize ) + Planes[p][3] > 0 )
				c++;
			if( Planes[p][0] * ( boundingBox->WorldPosition.x - boundingBoxSize ) 
					+ Planes[p][1] * ( boundingBox->WorldPosition.y + boundingBoxSize ) 
					+ Planes[p][2] * ( boundingBox->WorldPosition.z + boundingBoxSize ) + Planes[p][3] > 0 )
				c++;
			if( Planes[p][0] * ( boundingBox->WorldPosition.x + boundingBoxSize ) 
					+ Planes[p][1] * ( boundingBox->WorldPosition.y + boundingBoxSize ) 
					+ Planes[p][2] * ( boundingBox->WorldPosition.z + boundingBoxSize ) + Planes[p][3] > 0 )
				c++;

			if( !c ) return false;
		}

		return true;
	}
};
