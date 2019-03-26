#pragma once

	struct smallgeometry_t;

	class Skydome {
		DECLARE_MEMBER( private, smallgeometry_t*, Dome )
		DECLARE_MEMBER( private, texture_t*, Noise )
		DECLARE_MEMBER( private, IShader*, Shader )
		DECLARE_MEMBER( private, IShader*, ShaderEnvProbe )
		DECLARE_MEMBER( private, f32, SunTheta )
		DECLARE_MEMBER( private, f32, SunGamma )
		DECLARE_MEMBER( private, f32, SkyTurbidity )
		DECLARE_MEMBER( private, f32, SunPosition )
		DECLARE_MEMBER( private, f32, SkyAlbedo )

		public:
			Skydome();
			~Skydome();

			bool  Initialize();
			void  Render();

		private:
			void UpdateDome();
			void ComputeNoise();

			void  ComputeRadiance( const f32 sunPosition, const f32 theta, const f32 turbidity, const f32 albedo ); 
			f32   ComputeHosek( const f64* dataset, const u32 stride, const f32 turbidity, const f32 albedo, const f32 sunTheta );
			f64   SplineHosek( const f64* spline, u32 stride, f64 value ) const;
			vec3f PerezExtinction( const f32 cos_theta, const f32 gamma, const f32 cos_gamma );
	};
