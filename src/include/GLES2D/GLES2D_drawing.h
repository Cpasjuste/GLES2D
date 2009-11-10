#ifndef __GLES2D_DRAWING_H__
#define __GLES2D_DRAWING_H__

#include <SDL/SDL.h>

GDECLSPEC int GLES2D_ClearScreen( Uint8 r, Uint8 g, Uint8 b, Uint8 a );

GDECLSPEC void GLES2D_DrawPoint( int x, int y );

GDECLSPEC void GLES2D_DrawCircle (int circleSegments, float circleSize, int x, int y, int filled);

GDECLSPEC void GLES2D_DrawRectangle( int x, int y, int w, int h );

GDECLSPEC void GLES2D_DrawLine( int x1, int y1, int x2, int y2 );

GDECLSPEC void GLES2D_DrawTriangle( int ax, int ay, int bx, int by, int cx, int cy);

GDECLSPEC void GLES2D_DrawParticle( int x, int y, int w, int r, int g, int b, int a );

GDECLSPEC void GLES2D_SetDrawingColor( int r, int g, int b, int a );

#endif
