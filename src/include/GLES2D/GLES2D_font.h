#ifndef __GLES2D_FONT_H__
#define __GLES2D_FONT_H__

#define TTF_STYLE_NORMAL	0x00
#define TTF_STYLE_BOLD		0x01
#define TTF_STYLE_ITALIC	0x02
#define TTF_STYLE_UNDERLINE	0x04

typedef struct
{
	int style, pointsize, height, ascent, descent, lineskip, advance[128];
	int miny[128], maxy[128];
	GLES2D_Texture *texture[128];

} GLES2D_Font;

GDECLSPEC GLES2D_Font *GLES2D_CreateFont( char *filename, int style, int size );
GDECLSPEC void GLES2D_DrawFont( GLES2D_Font *font, int x, int y, char* str );
GDECLSPEC void GLES2D_DrawFontScroll( GLES2D_Font *font, int x, int y, int x_min, int x_max, char *str );
GDECLSPEC void GLES2D_DrawFontBox( GLES2D_Font *font, int x, int y, int width, int height, char *str );
GDECLSPEC int GLES2D_GetTextWidth( GLES2D_Font *font, char *str );
GDECLSPEC void GLES2D_DrawFontCentered( GLES2D_Font *font, int x, int y, char *str );
GDECLSPEC void GLES2D_SetFontColor( GLES2D_Font *font, int r, int g, int b, int a );
GDECLSPEC void GLES2D_SetFontAlpha( GLES2D_Font *font, int alpha );
GDECLSPEC void GLES2D_FreeFont( GLES2D_Font *font );

#endif
