#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>

#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <GLES/gl.h>
#include <GLES/egl.h>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_ttf.h>
#include <GLES2D/GLES2D_platform.h>
#include <GLES2D/GLES2D_texture.h>

#include <math.h>
#define degreesToRadian(x) (M_PI * x / 180.0)

extern GLES2D_Texture *particleTex;

extern int GLES2D_TestEGLError( char* pszLocation );

GDECLSPEC int GLES2D_ClearScreen( Uint8 r, Uint8 g, Uint8 b, Uint8 a )
{
	glEnable(GL_BLEND);

	GLfloat _r = r/255.0, _g = g/255.0, _b = b/255.0, _a = a/255.0;

	glClearColor( _r, _g, _b, _a );

	glClear( GL_COLOR_BUFFER_BIT );
	if ( !GLES2D_TestEGLError( "glClear" ) )
	{
		return 0;
	}

	glDisable(GL_BLEND);
	return 1;
}

GDECLSPEC void GLES2D_DrawPoint( int x, int y )
{
	glEnable(GL_BLEND);
//	glDisable(GL_TEXTURE_2D);

	GLshort vertices[2];
	vertices[0] = x;
	vertices[1] = y;

	glVertexPointer(2, GL_SHORT, 0, vertices);
	glEnableClientState(GL_VERTEX_ARRAY);
	glDrawArrays(GL_POINTS, 0, 1);
	glDisableClientState(GL_VERTEX_ARRAY);

	glDisable(GL_BLEND);
//	glEnable(GL_TEXTURE_2D);
}

GDECLSPEC void GLES2D_DrawEllipse (int segments, float width, float height, int x, int y, int filled)
{
	float i;

	glTranslatef(x, y, 0.0);

	GLshort vertices[segments*2];

	int count=0;
	for (i = 0; i < 360.0f; i+=(360.0f/segments))
	{
		vertices[count++] = (cos(degreesToRadian(i))*width);
		vertices[count++] = (sin(degreesToRadian(i))*height);
	}
	glVertexPointer (2, GL_FLOAT , 0, vertices); 
	glDrawArrays ((filled) ? GL_TRIANGLE_FAN : GL_LINE_LOOP, 0, segments);
}

GDECLSPEC void GLES2D_DrawCircle (int circleSegments, float circleSize, int x, int y, int filled) 
{
	glEnable(GL_BLEND);

//	glDisable(GL_TEXTURE_2D);
	glPushMatrix();
	
	GLES2D_DrawEllipse(circleSegments, circleSize, circleSize, x, y, filled);

	glPopMatrix();
	glDisable(GL_BLEND);
//	glEnable(GL_TEXTURE_2D);
}

GDECLSPEC void GLES2D_DrawRectangle( int x, int y, int w, int h )
{
	glEnable(GL_BLEND);
//	glDisable(GL_TEXTURE_2D);

	GLshort minx = x;
	GLshort maxx = x + w;
	GLshort miny = y;
	GLshort maxy = y + h;

	GLshort vertices[8];
	vertices[0] = minx;
	vertices[1] = miny;
	vertices[2] = maxx;
	vertices[3] = miny;
	vertices[4] = minx;
	vertices[5] = maxy;
	vertices[6] = maxx;
	vertices[7] = maxy;

	glVertexPointer(2, GL_SHORT, 0, vertices);
	glEnableClientState(GL_VERTEX_ARRAY);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	glDisableClientState(GL_VERTEX_ARRAY);

	glDisable(GL_BLEND);
//	glEnable(GL_TEXTURE_2D);	
}

GDECLSPEC void GLES2D_DrawLine( int x1, int y1, int x2, int y2 )
{
	glEnable(GL_BLEND);

//	glDisable(GL_TEXTURE_2D);

	GLshort vertices[4];
	vertices[0] = x1;
	vertices[1] = y1;
	vertices[2] = x2;
	vertices[3] = y2;

	glVertexPointer(2, GL_SHORT, 0, vertices);
	glEnableClientState(GL_VERTEX_ARRAY);
	glDrawArrays(GL_LINES, 0, 2);
	glDisableClientState(GL_VERTEX_ARRAY);

	glDisable(GL_BLEND);
//	glEnable(GL_TEXTURE_2D);
} 

GDECLSPEC void GLES2D_DrawTriangle( int ax, int ay, int bx, int by, int cx, int cy)
{
	glEnable(GL_BLEND);

	GLshort vertices[3 * 2];

//	glDisable(GL_TEXTURE_2D);

	vertices[0] = ax;	vertices[1] = ay;
	vertices[2] = bx;	vertices[3] = by;
	vertices[4] = cx;	vertices[5] = cy;

	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(2, GL_FLOAT, 0, vertices);

	glDrawArrays( GL_TRIANGLES , 0, 4 );

	glDisable(GL_BLEND);
//	glEnable(GL_TEXTURE_2D);
}

GDECLSPEC void GLES2D_DrawParticle(int x, int y, int w, int r, int g, int b, int a)
{
	GLES2D_SetTextureColor( particleTex, r, g, b, a );

//	if ( w > 32 ) 
//		GLES2D_EnableTextureFiltering( particleTex );
//	else
//		GLES2D_DisableTextureFiltering( particleTex );

	GLES2D_DrawTextureScaledCentered( particleTex, x, y, w, w );
}

GDECLSPEC void GLES2D_SetDrawingColor( int r, int g, int b, int a )
{
	float _r = r/255.0, _g = g/255.0, _b = b/255.0, _a = a/255.0;

	glColor4f( _r, _g, _b, _a );
}


