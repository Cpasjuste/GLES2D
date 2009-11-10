#ifndef __GLES2D_TILE_H__
#define __GLES2D_TILE_H__

#include "GLES2D/GLES2D_platform.h"
#include "GLES2D/GLES2D_texture.h"

#define GLES2D_LEFT 0
#define GLES2D_RIGHT 1
#define GLES2D_UP 2
#define GLES2D_DOWN 3

/* Mappy tile map support */
GDECLSPEC int GLES2D_TilesMapLoad( char * Filename , int left , int top , int width , int height );

GDECLSPEC void GLES2D_TilesMapMoveTo( int x , int y );

GDECLSPEC int GLES2D_TilesMapDraw ( int w, int h );

GDECLSPEC int GLES2D_TilesMapDrawFG ( int w, int h );

GDECLSPEC int GLES2D_TilesMapDrawParallax ( GLES2D_Texture *texture, int w, int h );

GDECLSPEC int GLES2D_TilesMapDrawMovingParallax ( GLES2D_Texture *texture, int w, int h, int direction, int speed );

GDECLSPEC short int GLES2D_TilesMapGetWidth();

GDECLSPEC short int GLES2D_TilesMapGetHeight();

GDECLSPEC void GLES2D_TilesMapUpdateAnims();

GDECLSPEC void GLES2D_TilesMapInitAnims();

GDECLSPEC int GLES2D_MapChangeLayer (int newlyr);

GDECLSPEC void GLES2D_TilesMapFree ();

#endif
