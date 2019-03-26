#pragma once

/*===========================
aabb_t
	Axis Aligned Bounding Box
===========================*/
struct aabb_t {
	explicit aabb_t( const vec3f &worldPosition, const vec3f &dimensions )  : WorldPosition( worldPosition ), 
								                                                                           Dimensions( dimensions ) { 
		#ifdef FLAG_DEBUG 
			DebugRender = nullptr;
			DebugColor  = colorrgbaf();
		#endif
	}

	explicit aabb_t()  : WorldPosition(),
								                      Dimensions() { 
		#ifdef FLAG_DEBUG 
			DebugRender = nullptr;
			DebugColor = colorrgbaf();
		#endif
	}

	~aabb_t() {
		#ifdef FLAG_DEBUG 
			DebugRender = nullptr;
			DebugColor = colorrgbaf();
		#endif

		WorldPosition = vec3f();
		Dimensions    = vec3f();
	}


	bool RayHitTest( vec3f rayStart, vec3f rayDir ) {
		f32 tMin = -F32_MAX, tMax = F32_MAX;

		for( i32 i = 2; i >= 0; --i ) {
			const f32 invDir = 1.0f / rayDir[i];

			f32 minI   = WorldPosition[i] - Dimensions[i] / 2.0f;
			f32 maxI   = WorldPosition[i] + Dimensions[i] / 2.0f;

			f32 t0 = ( minI - rayStart[i] ) * invDir;
			f32 t1 = ( maxI - rayStart[i] ) * invDir;

			if( invDir < 0.0f ) ::Swap( &t0, &t1 );

			tMin = t0 > tMin ? t0 : tMin;
			tMax = t1 < tMax ? t1 : tMax;

			if( tMax < tMin ) {
				return false;
			}
		}

		return true;
	}

	bool RayHitTest( vec3f rayStart, vec3f rayDir, vec3f &hCoords ) {
		bool isInside  = true;

		vec3f minBound  = WorldPosition - Dimensions / 2.0f;
		vec3f maxBound  = WorldPosition + Dimensions / 2.0f;
		vec3f hitCoords = vec3f( 0.0f );
		
		vec3f maxT      = vec3f( -1.0f );

		for( i32 i = 0; i < 3; ++i ) {
			if( rayStart[i] < minBound[i] ) {
				hitCoords[i] = minBound[i];
				isInside     = false;
			
				if( ( (u32&)rayDir[i] ) ) {
					maxT[i] = ( minBound[i] - rayStart[i] ) / rayDir[i];
				}
			} else if( rayStart[i] > maxBound[i] ) {
				hitCoords[i] = maxBound[i];
				isInside = false;

				if( ( (u32&)rayDir[i] ) ) {
					maxT[i] = ( maxBound[i] - rayStart[i] ) / rayDir[i];
				}
			}
		}

		if( isInside ) {
			hCoords = rayStart;
			return true;
		}

		u32 plane = 0;
		
		if( maxT[1] > maxT[plane] ) plane = 1;
		if( maxT[2] > maxT[plane] ) plane = 2;

		if( ( (u32&)maxT[plane] ) & 0x80000000 )
			return false;

		for( i32 i = 0; i < 3; ++i ) {
			if( i != plane ) {
				hitCoords[i] = rayStart[i] + maxT[plane] * rayDir[i];

				if( hitCoords[i] < minBound[i] || hitCoords[i] > maxBound[i] ) 
					return false;
			}
		}

		hCoords = hitCoords;
		return true;
	}

	#ifdef FLAG_DEBUG
		renderable_t* DebugRender;
		colorrgbaf    DebugColor;
	#endif

	vec3f         WorldPosition;
	vec3f         Dimensions;
};