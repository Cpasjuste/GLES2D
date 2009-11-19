#ifndef __GLES2D_PLATFORM_H__
#define __GLES2D_PLATFORM_H__

#ifdef _WIN32
#define GDECLSPEC	__declspec(dllexport)
#else
#define GDECLSPEC
#endif

#ifdef DEBUG
#define gprintf( fmt, ... ) \
	DEBUG_PRINTF( "(%s():%s:%d): ", __func__, __FILE__, __LINE__ );\
	DEBUG_PRINTF( fmt, __VA_ARGS__ );\
	DEBUG_PRINTF( "\n" );
#define gprintf( str ) \
	DEBUG_PRINTF( "(%s():%s:%d): ", __func__, __FILE__, __LINE__ );\
	DEBUG_PRINTF( str );\
	DEBUG_PRINTF( "\n" );
#else
#define gprintf( fmt, ... )
#define gprint( str )
#endif
#endif
