#pragma once

/*===========================
color_rgb_t
	RGB Color Template
===========================*/
template<typename T>
struct color_rgb_t {
	explicit constexpr color_rgb_t( const T ir, const T ig, const T ib )  : r( ir ),
								                                                                         g( ig ),
								                                                                         b( ib ) {

	}

	explicit color_rgb_t( const T i )  : r( i ),
								                                      g( i ),
								                                      b( i ) {

	}

	explicit color_rgb_t( const vector3_t<T> vc )  : r( vc.x ),
								                                                  g( vc.y ),
								                                                  b( vc.z ) {
	
	}

	explicit color_rgb_t( const color_rgba_t<T> rgba )  : r( rgba.r ),
								                                                       g( rgba.g ),
								                                                       b( rgba.b ) {

	}

	explicit color_rgb_t()  : r( (T)0 ),
								                           g( (T)0 ),
								                           b( (T)0 ) {

	}

	~color_rgb_t() {
		r = ( T )0;
		g = ( T )0;
		b = ( T )0;
	}

	INLINE constexpr T &operator[] ( const i32 i ) const  {
		return ( i == 0 ) ? r : ( i == 1 ) ? g : b;
	}

	T  r;
	T  g;
	T  b;
};

//=====================================================================================================================
	
/*===========================
color_rgba_t
	RGBA Color Template
===========================*/
template<typename T>
struct color_rgba_t {
	explicit color_rgba_t( const color_rgb_t<T> irgb, const T ia )  : r( irgb.r ),
								                                                                   g( irgb.g ),
								                                                                   b( irgb.b ),
								                                                                   a( ia ) {

	}

	explicit color_rgba_t( const color_rgb_t<T> irgb )  : r( irgb.r ), 
								                                                       g( irgb.g ),
								                                                       b( irgb.b ), 
								                                                       a( ( T )1 ) { 
	
	}

	explicit color_rgba_t( const T i )  : r( i ),
								                                       g( i ),
								                                       b( i ),
								                                       a( i ) {

	}

	explicit color_rgba_t( const T i, const T a )  : r( i ),
								                                                  g( i ),
								                                                  b( i ),
								                                                  a( a ) {

	}

	explicit color_rgba_t( const T ir, const T ig, const T ib, const T ia )  : r( ir ),
								                                                                            g( ig ),
								                                                                            b( ib ),
								                                                                            a( ia ) {

	}

	explicit color_rgba_t( const vector3_t<T> c, const T ia )  : r( c.x ), 
								                                                              g( c.y ), 
								                                                              b( c.z ), 
								                                                              a( ia ) {

	}

	explicit color_rgba_t()  : r( (T)0 ), 
								                            g( (T)0 ), 
								                            b( (T)0 ), 
								                            a( (T)0 ) {

	}

	~color_rgba_t() {
		r = ( T )0;
		g = ( T )0;
		b = ( T )0;
		a = ( T )0;
	}

	INLINE constexpr T &operator[] ( const i32 &i ) const  {
		return ( !i ) ? r : ( i == 1 ) ? g : ( i == 2 ) ? b : a;
	}

	T  r;
	T  g;
	T  b;
	T  a;
};

//=====================================================================================================================

#define COLOR_RGB_ALICE_BLUE           colorrgbf( 0.941f, 0.973f, 1.000f )
#define COLOR_RGB_ANTIQUEWHITE         colorrgbf( 0.980f, 0.922f, 0.843f )
#define COLOR_RGB_AQUA                 colorrgbf( 0.000f, 1.000f, 1.000f )
#define COLOR_RGB_AQUAMARINE           colorrgbf( 0.498f, 1.000f, 0.831f )
#define COLOR_RGB_AZURE                colorrgbf( 0.941f, 1.000f, 1.000f )
#define COLOR_RGB_BEIGE                colorrgbf( 0.961f, 0.961f, 0.863f )
#define COLOR_RGB_BISQUE               colorrgbf( 1.000f, 0.894f, 0.769f )
#define COLOR_RGB_BLACK                colorrgbf( 0.000f, 0.000f, 0.000f )
#define COLOR_RGB_BLANCHEDALMOND       colorrgbf( 1.000f, 0.922f, 0.804f )
#define COLOR_RGB_BLUE                 colorrgbf( 0.000f, 0.000f, 1.000f )
#define COLOR_RGB_BLUEVIOLET           colorrgbf( 0.541f, 0.169f, 0.886f )
#define COLOR_RGB_BROWN                colorrgbf( 0.647f, 0.165f, 0.165f )
#define COLOR_RGB_BURLYWOOD            colorrgbf( 0.871f, 0.722f, 0.529f )
#define COLOR_RGB_CADETBLUE            colorrgbf( 0.373f, 0.620f, 0.627f )
#define COLOR_RGB_CHARTREUSE           colorrgbf( 0.498f, 1.000f, 0.000f )
#define COLOR_RGB_CHOCOLATE            colorrgbf( 0.824f, 0.412f, 0.118f )
#define COLOR_RGB_CORAL                colorrgbf( 1.000f, 0.498f, 0.314f )
#define COLOR_RGB_CORNFLOWERBLUE       colorrgbf( 0.392f, 0.584f, 0.929f )
#define COLOR_RGB_CORNSILK             colorrgbf( 1.000f, 0.973f, 0.863f )
#define COLOR_RGB_CRIMSON              colorrgbf( 0.863f, 0.078f, 0.235f )
#define COLOR_RGB_CYAN                 colorrgbf( 0.000f, 1.000f, 1.000f )
#define COLOR_RGB_DARKBLUE             colorrgbf( 0.000f, 0.000f, 0.545f )
#define COLOR_RGB_DARKCYAN             colorrgbf( 0.000f, 0.545f, 0.545f )
#define COLOR_RGB_DARKGOLDENROD        colorrgbf( 0.722f, 0.525f, 0.043f )
#define COLOR_RGB_DARKGRAY             colorrgbf( 0.663f, 0.663f, 0.663f )
#define COLOR_RGB_DARKGREEN            colorrgbf( 0.000f, 0.392f, 0.000f )
#define COLOR_RGB_DARKGREY             colorrgbf( 0.663f, 0.663f, 0.663f )
#define COLOR_RGB_DARKKHAKI            colorrgbf( 0.741f, 0.718f, 0.420f )
#define COLOR_RGB_DARKMAGENTA          colorrgbf( 0.545f, 0.000f, 0.545f )
#define COLOR_RGB_DARKOLIVEGREEN       colorrgbf( 0.333f, 0.420f, 0.184f )
#define COLOR_RGB_DARKORANGE           colorrgbf( 1.000f, 0.549f, 0.000f )
#define COLOR_RGB_DARKORCHID           colorrgbf( 0.600f, 0.196f, 0.800f )
#define COLOR_RGB_DARKRED              colorrgbf( 0.545f, 0.000f, 0.000f )
#define COLOR_RGB_DARKSALMON           colorrgbf( 0.914f, 0.588f, 0.478f )
#define COLOR_RGB_DARKSEAGREEN         colorrgbf( 0.561f, 0.737f, 0.561f )
#define COLOR_RGB_DARKSLATEBLUE        colorrgbf( 0.282f, 0.239f, 0.545f )
#define COLOR_RGB_DARKSLATEGRAY        colorrgbf( 0.184f, 0.310f, 0.310f )
#define COLOR_RGB_DARKSLATEGREY        colorrgbf( 0.184f, 0.310f, 0.310f )
#define COLOR_RGB_DARKTURQUOISE        colorrgbf( 0.000f, 0.808f, 0.820f )
#define COLOR_RGB_DARKVIOLET           colorrgbf( 0.580f, 0.000f, 0.827f )
#define COLOR_RGB_DEEPPINK             colorrgbf( 1.000f, 0.078f, 0.576f )
#define COLOR_RGB_DEEPSKYBLUE          colorrgbf( 0.000f, 0.749f, 1.000f )
#define COLOR_RGB_DIMGRAY              colorrgbf( 0.412f, 0.412f, 0.412f )
#define COLOR_RGB_DIMGREY              colorrgbf( 0.412f, 0.412f, 0.412f )
#define COLOR_RGB_DODGERBLUE           colorrgbf( 0.118f, 0.565f, 1.000f )
#define COLOR_RGB_FIREBRICK            colorrgbf( 0.698f, 0.133f, 0.133f )
#define COLOR_RGB_FLORALWHITE          colorrgbf( 1.000f, 0.980f, 0.941f )
#define COLOR_RGB_FORESTGREEN          colorrgbf( 0.133f, 0.545f, 0.133f )
#define COLOR_RGB_FUCHSIA              colorrgbf( 1.000f, 0.000f, 1.000f )
#define COLOR_RGB_GAINSBORO            colorrgbf( 0.863f, 0.863f, 0.863f )
#define COLOR_RGB_GHOSTWHITE           colorrgbf( 0.973f, 0.973f, 1.000f )
#define COLOR_RGB_GOLD                 colorrgbf( 1.000f, 0.843f, 0.000f )
#define COLOR_RGB_GOLDENROD            colorrgbf( 0.855f, 0.647f, 0.125f )
#define COLOR_RGB_GRAY                 colorrgbf( 0.502f, 0.502f, 0.502f )
#define COLOR_RGB_GREEN                colorrgbf( 0.000f, 0.502f, 0.000f )
#define COLOR_RGB_GREENYELLOW          colorrgbf( 0.678f, 1.000f, 0.184f )
#define COLOR_RGB_GREY                 colorrgbf( 0.502f, 0.502f, 0.502f )
#define COLOR_RGB_HONEYDEW             colorrgbf( 0.941f, 1.000f, 0.941f )
#define COLOR_RGB_HOTPINK              colorrgbf( 1.000f, 0.412f, 0.706f )
#define COLOR_RGB_INDIANRED            colorrgbf( 0.804f, 0.361f, 0.361f )
#define COLOR_RGB_INDIGO               colorrgbf( 0.294f, 0.000f, 0.510f )
#define COLOR_RGB_IVORY                colorrgbf( 1.000f, 1.000f, 0.941f )
#define COLOR_RGB_KHAKI                colorrgbf( 0.941f, 0.902f, 0.549f )
#define COLOR_RGB_LAVENDER             colorrgbf( 0.902f, 0.902f, 0.980f )
#define COLOR_RGB_LAVENDERBLUSH        colorrgbf( 1.000f, 0.941f, 0.961f )
#define COLOR_RGB_LAWNGREEN            colorrgbf( 0.486f, 0.988f, 0.000f )
#define COLOR_RGB_LEMONCHIFFON         colorrgbf( 1.000f, 0.980f, 0.804f )
#define COLOR_RGB_LIGHTBLUE            colorrgbf( 0.678f, 0.847f, 0.902f )
#define COLOR_RGB_LIGHTCORAL           colorrgbf( 0.941f, 0.502f, 0.502f )
#define COLOR_RGB_LIGHTCYAN            colorrgbf( 0.878f, 1.000f, 1.000f )
#define COLOR_RGB_LIGHTGOLDENRODYELLOW colorrgbf( 0.980f, 0.980f, 0.824f )
#define COLOR_RGB_LIGHTGRAY            colorrgbf( 0.827f, 0.827f, 0.827f )
#define COLOR_RGB_LIGHTGREEN           colorrgbf( 0.565f, 0.933f, 0.565f )
#define COLOR_RGB_LIGHTGREY            colorrgbf( 0.827f, 0.827f, 0.827f )
#define COLOR_RGB_LIGHTPINK            colorrgbf( 1.000f, 0.714f, 0.757f )
#define COLOR_RGB_LIGHTSALMON          colorrgbf( 1.000f, 0.627f, 0.478f )
#define COLOR_RGB_LIGHTSEAGREEN        colorrgbf( 0.125f, 0.698f, 0.667f )
#define COLOR_RGB_LIGHTSKYBLUE         colorrgbf( 0.529f, 0.808f, 0.980f )
#define COLOR_RGB_LIGHTSLATEGRAY       colorrgbf( 0.467f, 0.533f, 0.600f )
#define COLOR_RGB_LIGHTSLATEGREY       colorrgbf( 0.467f, 0.533f, 0.600f )
#define COLOR_RGB_LIGHTSTEELBLUE       colorrgbf( 0.690f, 0.769f, 0.871f )
#define COLOR_RGB_LIGHTYELLOW          colorrgbf( 1.000f, 1.000f, 0.878f )
#define COLOR_RGB_LIME                 colorrgbf( 0.000f, 1.000f, 0.000f )
#define COLOR_RGB_LIMEGREEN            colorrgbf( 0.196f, 0.804f, 0.196f )
#define COLOR_RGB_LINEN                colorrgbf( 0.980f, 0.941f, 0.902f )
#define COLOR_RGB_MAGENTA              colorrgbf( 1.000f, 0.000f, 1.000f )
#define COLOR_RGB_MAROON               colorrgbf( 0.502f, 0.000f, 0.000f )
#define COLOR_RGB_MEDIUMAQUAMARINE     colorrgbf( 0.400f, 0.804f, 0.667f )
#define COLOR_RGB_MEDIUMBLUE           colorrgbf( 0.000f, 0.000f, 0.804f )
#define COLOR_RGB_MEDIUMORCHID         colorrgbf( 0.729f, 0.333f, 0.827f )
#define COLOR_RGB_MEDIUMPURPLE         colorrgbf( 0.576f, 0.439f, 0.859f )
#define COLOR_RGB_MEDIUMSEAGREEN       colorrgbf( 0.235f, 0.702f, 0.443f )
#define COLOR_RGB_MEDIUMSLATEBLUE      colorrgbf( 0.482f, 0.408f, 0.933f )
#define COLOR_RGB_MEDIUMSPRINGGREEN    colorrgbf( 0.000f, 0.980f, 0.604f )
#define COLOR_RGB_MEDIUMTURQUOISE      colorrgbf( 0.282f, 0.820f, 0.800f )
#define COLOR_RGB_MEDIUMVIOLETRED      colorrgbf( 0.780f, 0.082f, 0.522f )
#define COLOR_RGB_MIDNIGHTBLUE         colorrgbf( 0.098f, 0.098f, 0.439f )
#define COLOR_RGB_MINTCREAM            colorrgbf( 0.961f, 1.000f, 0.980f )
#define COLOR_RGB_MISTYROSE            colorrgbf( 1.000f, 0.894f, 0.882f )
#define COLOR_RGB_MOCCASIN             colorrgbf( 1.000f, 0.894f, 0.710f )
#define COLOR_RGB_NAVAJOWHITE          colorrgbf( 1.000f, 0.871f, 0.678f )
#define COLOR_RGB_NAVY                 colorrgbf( 0.000f, 0.000f, 0.502f )
#define COLOR_RGB_OLDLACE              colorrgbf( 0.992f, 0.961f, 0.902f )
#define COLOR_RGB_OLIVE                colorrgbf( 0.502f, 0.502f, 0.000f )
#define COLOR_RGB_OLIVEDRAB            colorrgbf( 0.420f, 0.557f, 0.137f )
#define COLOR_RGB_ORANGE               colorrgbf( 1.000f, 0.647f, 0.000f )
#define COLOR_RGB_ORANGERED            colorrgbf( 1.000f, 0.271f, 0.000f )
#define COLOR_RGB_ORCHID               colorrgbf( 0.855f, 0.439f, 0.839f )
#define COLOR_RGB_PALEGOLDENROD        colorrgbf( 0.933f, 0.910f, 0.667f )
#define COLOR_RGB_PALEGREEN            colorrgbf( 0.596f, 0.984f, 0.596f )
#define COLOR_RGB_PALETURQUOISE        colorrgbf( 0.686f, 0.933f, 0.933f )
#define COLOR_RGB_PALEVIOLETRED        colorrgbf( 0.859f, 0.439f, 0.576f )
#define COLOR_RGB_PAPAYAWHIP           colorrgbf( 1.000f, 0.937f, 0.835f )
#define COLOR_RGB_PEACHPUFF            colorrgbf( 1.000f, 0.855f, 0.725f )
#define COLOR_RGB_PERU                 colorrgbf( 0.804f, 0.522f, 0.247f )
#define COLOR_RGB_PINK                 colorrgbf( 1.000f, 0.753f, 0.796f )
#define COLOR_RGB_PLUM                 colorrgbf( 0.867f, 0.627f, 0.867f )
#define COLOR_RGB_POWDERBLUE           colorrgbf( 0.690f, 0.878f, 0.902f )
#define COLOR_RGB_PURPLE               colorrgbf( 0.502f, 0.000f, 0.502f )
#define COLOR_RGB_RED                  colorrgbf( 1.000f, 0.000f, 0.000f )
#define COLOR_RGB_ROSYBROWN            colorrgbf( 0.737f, 0.561f, 0.561f )
#define COLOR_RGB_ROYALBLUE            colorrgbf( 0.255f, 0.412f, 0.882f )
#define COLOR_RGB_SADDLEBROWN          colorrgbf( 0.545f, 0.271f, 0.075f )
#define COLOR_RGB_SALMON               colorrgbf( 0.980f, 0.502f, 0.447f )
#define COLOR_RGB_SANDYBROWN           colorrgbf( 0.957f, 0.643f, 0.376f )
#define COLOR_RGB_SEAGREEN             colorrgbf( 0.180f, 0.545f, 0.341f )
#define COLOR_RGB_SEASHELL             colorrgbf( 1.000f, 0.961f, 0.933f )
#define COLOR_RGB_SIENNA               colorrgbf( 0.627f, 0.322f, 0.176f )
#define COLOR_RGB_SILVER               colorrgbf( 0.753f, 0.753f, 0.753f )
#define COLOR_RGB_SKYBLUE              colorrgbf( 0.529f, 0.808f, 0.922f )
#define COLOR_RGB_SLATEBLUE            colorrgbf( 0.416f, 0.353f, 0.804f )
#define COLOR_RGB_SLATEGRAY            colorrgbf( 0.439f, 0.502f, 0.565f )
#define COLOR_RGB_SLATEGREY            colorrgbf( 0.439f, 0.502f, 0.565f )
#define COLOR_RGB_SNOW                 colorrgbf( 1.000f, 0.980f, 0.980f )
#define COLOR_RGB_SPRINGGREEN          colorrgbf( 0.000f, 1.000f, 0.498f )
#define COLOR_RGB_STEELBLUE            colorrgbf( 0.275f, 0.510f, 0.706f )
#define COLOR_RGB_TAN                  colorrgbf( 0.824f, 0.706f, 0.549f )
#define COLOR_RGB_TEAL                 colorrgbf( 0.000f, 0.502f, 0.502f )
#define COLOR_RGB_THISTLE              colorrgbf( 0.847f, 0.749f, 0.847f )
#define COLOR_RGB_TOMATO               colorrgbf( 1.000f, 0.388f, 0.278f )
#define COLOR_RGB_TURQUOISE            colorrgbf( 0.251f, 0.878f, 0.816f )
#define COLOR_RGB_VIOLET               colorrgbf( 0.933f, 0.510f, 0.933f )
#define COLOR_RGB_WHEAT                colorrgbf( 0.961f, 0.871f, 0.702f )
#define COLOR_RGB_WHITE                colorrgbf( 1.000f, 1.000f, 1.000f )
#define COLOR_RGB_WHITESMOKE           colorrgbf( 0.961f, 0.961f, 0.961f )
#define COLOR_RGB_YELLOW               colorrgbf( 1.000f, 1.000f, 0.000f )
#define COLOR_RGB_YELLOWGREEN          colorrgbf( 0.604f, 0.804f, 0.196f )

//=====================================================================================================================
