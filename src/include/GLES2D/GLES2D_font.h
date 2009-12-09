#ifndef __GLES2D_FONT_H__
#define __GLES2D_FONT_H__

#define TTF_STYLE_NORMAL	0x00
#define TTF_STYLE_BOLD		0x01
#define TTF_STYLE_ITALIC	0x02
#define TTF_STYLE_UNDERLINE	0x04

typedef struct
{
	int style, pointsize, height, ascent, descent, lineskip, advance[128];
	int miny[128], maxy[128], minx[128], maxx[128];
	GLES2D_Texture *texture[128];

} GLES2D_Font;


typedef struct
{
	int style, pointsize, height, width, ascent, descent, lineskip, advance;
	GLES2D_Texture *texture;

} GLES2D_FontCache;

GDECLSPEC GLES2D_Font *GLES2D_CreateFont( char *filename, int style, int size );
GDECLSPEC void GLES2D_DrawFont( GLES2D_Font *font, int x, int y, char* str );
GDECLSPEC void GLES2D_DrawFontCentered( GLES2D_Font *font, int x, int y, char *str );
GDECLSPEC void GLES2D_DrawFontCut( GLES2D_Font *font, int x, int y, int x_max, char *str );
GDECLSPEC void GLES2D_DrawFontScroll( GLES2D_Font *font, int x, int y, int x_min, int x_max, char *str );
GDECLSPEC void GLES2D_DrawFontBox( GLES2D_Font *font, int x, int y, int width, int height, char *str );
GDECLSPEC void GLES2D_SetFontColor( GLES2D_Font *font, int r, int g, int b, int a );
GDECLSPEC void GLES2D_SetFontAlpha( GLES2D_Font *font, int alpha );
GDECLSPEC void GLES2D_SetFontFiltering( GLES2D_Font *font, int param );
GDECLSPEC int GLES2D_GetTextWidth( GLES2D_Font *font, char *str );
GDECLSPEC void GLES2D_FreeFont( GLES2D_Font *font );

GDECLSPEC GLES2D_FontCache *GLES2D_CreateFontCache( char *filename, char *string, int style, int size, int width );
GDECLSPEC void GLES2D_DrawFontCache( GLES2D_FontCache *font, int x, int y );
GDECLSPEC void GLES2D_DrawFontCacheCentered( GLES2D_FontCache *font, int x, int y );
GDECLSPEC void GLES2D_SetFontCacheColor( GLES2D_FontCache *font, int r, int g, int b, int a );
GDECLSPEC void GLES2D_FreeFontCache( GLES2D_FontCache *font );
#endif
