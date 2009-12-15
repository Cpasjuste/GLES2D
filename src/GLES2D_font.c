#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>

#include <GLES/gl.h>
#include <GLES/egl.h>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_ttf.h>
#include <GLES2D/GLES2D_platform.h>
#include <GLES2D/GLES2D_texture.h>
#include <GLES2D/GLES2D_drawing.h>
#include <GLES2D/GLES2D_font.h>

GDECLSPEC GLES2D_FontCache *GLES2D_CreateFontCache( char *filename, char *string, int style, int size, int width )
{
	SDL_Color white = {0xFF, 0xFF, 0xFF, 0x00};
	GLES2D_FontCache *_font;

	_font = (GLES2D_FontCache *) malloc( sizeof( GLES2D_FontCache ) );

	TTF_Font *ttffont = TTF_OpenFont( filename, size );

	if ( ttffont == NULL )
	{
		gprintf( "ERROR opening font: %s\n", TTF_GetError() );
		return NULL;
	}

	TTF_SetFontStyle( ttffont, style );
	
	_font->ascent = TTF_FontAscent( ttffont );
	_font->descent = TTF_FontDescent( ttffont );
	_font->height = TTF_FontHeight( ttffont );
	_font->lineskip = TTF_FontLineSkip( ttffont );

	//gprintf("ascent : %i || descent : %i || lineskip : %i || height : %i\n", _font->ascent, _font->descent, _font->lineskip, _font->height );

	int i = 0;
	char msg[512];
	int w = 0;

	int msg_lenght = strlen( string );
	for ( i = 0; i < msg_lenght; i++ )
	{
		memset( msg, 0, 512 );
		strncpy( msg, string, i );
		TTF_SizeText( ttffont, msg, &w, NULL );
		
		if ( w >= 800 || w >= width )
			break;
	}

	memset( msg, 0, 512 );
	strncpy( msg, string, i );

	TTF_SizeText( ttffont, msg, &_font->width, &_font->height );

	SDL_Surface *surface = TTF_RenderText_Blended( ttffont, msg, white );

	_font->texture = GLES2D_CreateTextureFromSurface( surface, 0 );

	SDL_FreeSurface( surface );

	TTF_CloseFont( ttffont );

	return _font;
}

GDECLSPEC GLES2D_Font *GLES2D_CreateFont( char *filename, int style, int size )
{
	TTF_Font* ttffont;
	SDL_Surface* surf;
	int i;
	GLenum gl_error;
	SDL_Color white = {0xFF, 0xFF, 0xFF, 0x00};

	GLES2D_Font *_font;
	_font = (GLES2D_Font *) malloc( sizeof( GLES2D_Font ) );

	ttffont = TTF_OpenFont( filename, size );
	if ( ttffont == NULL )
	{
		gprintf( "ERROR opening font: %s\n", TTF_GetError() );
		return NULL;
	}

	TTF_SetFontStyle( ttffont, style );
	
	_font->ascent = TTF_FontAscent( ttffont );
	_font->descent = TTF_FontDescent( ttffont );
	_font->height = TTF_FontHeight( ttffont );
	_font->lineskip = TTF_FontLineSkip( ttffont );

	for ( i = 0; i < 128; i++ )
	{
		surf = TTF_RenderGlyph_Blended( ttffont, i, white );
		if ( surf == NULL )
		{
			gprintf( "ERROR rendering glyph %c: %s\n", (char)i, TTF_GetError() );
			TTF_CloseFont( ttffont );
			return NULL;			
		}
		
		//gprintf("%c: ", (char)i );
		_font->texture[i] = GLES2D_CreateTextureFromSurface( surf, 0 );

		TTF_GlyphMetrics( ttffont, i, &_font->minx[i], &_font->maxx[i], &_font->miny[i], &_font->maxy[i], &( _font->advance[i] ) );
		
		if ( ( gl_error = glGetError()) != GL_NO_ERROR )
		{
			/* If this failed, the text may exceed texture size limits */
			gprintf( "Warning: Couldn't create texture: 0x%x\n", gl_error );
			SDL_FreeSurface( surf );
			TTF_CloseFont( ttffont );
			return NULL;
		}		
		SDL_FreeSurface( surf );
	}
	
	TTF_CloseFont(ttffont);
	
	return _font;
}

GDECLSPEC void GLES2D_DrawFontCache( GLES2D_FontCache *font, int x, int y )
{
	GLES2D_DrawTextureSimple( font->texture, x, y  );
}

GDECLSPEC void GLES2D_DrawFontCacheCentered( GLES2D_FontCache *font, int x, int y )
{
	GLES2D_DrawTextureCentered( font->texture, x, y  );

}

GDECLSPEC void _GLES2D_DrawFont( GLES2D_Font *font, int x, int y, char *str, int minx, int maxx )
{
	int i;
	while (*str != 0)
	{
		i = (int)*str;
		
		if ( ( i >= 0 ) && ( i < 128 ) )
		{
			/* crasy hack, understand nothing */
			int y2 = font->miny[i];
			if ( font->miny[i] ) y2 += 2;

			if ( minx || maxx )
			{
				glEnable( GL_SCISSOR_TEST );
				glScissor( minx, 0, maxx - minx, 480 );

				if ( ( x >= minx - font->advance[i] ) && ( x + font->advance[i] < maxx + font->advance[i] ) )
				{
					GLES2D_DrawTextureSimple( font->texture[i], x, ( y + ( ( abs(y2)/2 ) - font->maxy[i] ) ) + font->ascent );
				}

				glDisable( GL_SCISSOR_TEST );
			}
			else
			{
				GLES2D_DrawTextureSimple( font->texture[i], ( x + font->minx[i] ), ( y + ( ( abs(y2)/2 ) - font->maxy[i] ) ) + font->ascent );
			}

			x += font->advance[i];
		}
		str++;
	}
}

GDECLSPEC void GLES2D_DrawFontScroll( GLES2D_Font *font, int x, int y, int x_min, int x_max, char *str )
{
	_GLES2D_DrawFont( font, x , y, str, x_min, x_max );
}

GDECLSPEC void GLES2D_DrawFont( GLES2D_Font *font, int x, int y, char *str )
{
	_GLES2D_DrawFont( font, x, y, str, 0, 0 );
}

GDECLSPEC void GLES2D_DrawFontCut( GLES2D_Font *font, int x, int y, int x_max, char *str )
{
	_GLES2D_DrawFont( font, x, y, str, x, x_max );
}

GDECLSPEC int GLES2D_GetTextWidth( GLES2D_Font *font, char *str )
{
	int i, width = 0;

	while (*str != 0)
	{
		i = (int)*str;
		if ( ( i >= 0 ) && ( i < 128 ) )
		{
			width += font->advance[i];
		}
		str++;
	}	

	return width;
}

GDECLSPEC void GLES2D_DrawFontCentered( GLES2D_Font *font, int x, int y, char *str )
{
	GLES2D_DrawFont( font, x - ( GLES2D_GetTextWidth( font, str ) / 2 ), y , str );
}


GDECLSPEC void GLES2D_DrawFontBox( GLES2D_Font *font, int x, int y, int width, int height, char *str )
{
	char word[512][512];
	char line[64][512];

	int word_count = 0;
	int char_count = 0;
	int i = 0, n = 0;

	for ( i = 0; i < 512; i++ )
	{
		memset ( word[i], 0, 512 );

		if ( i < 64 )
			memset ( line[i], 0, 512 );
	}

	/* Decompose String in words */
	while ( *str != 0 )
	{
		word[word_count][char_count] = *str;
		char_count++;

		if ( *str == ' ' )
		{
			char_count = 0;
			word_count += 1;
		}
		str++;
	}

	if ( char_count > 0 )
		word_count += 1;

	/* Build lines with words */
	for ( i = 0; i < word_count; i++ )
	{
		int word_pixels = GLES2D_GetTextWidth( font, word[i] );
		int line_pixels = GLES2D_GetTextWidth( font, line[n] );

		if ( ( line_pixels + word_pixels + 10 ) >= width )
		{
			n++;
		}
		if ( ! strlen( line[n] ) ) 
			strcpy( line[n], word[i] );
		else 
			strcat( line[n], word[i] );
	}

	/* Render line by line */
	y += font->lineskip;
	for ( i = 0; i < n+1; i++ )
	{
		GLES2D_DrawFontCentered( font, x + width/2, y, line[i] );
		y += font->lineskip;
	}
}

GDECLSPEC void GLES2D_DrawFontBoxFill( GLES2D_Font *font, int x, int y, int width, int height, int br, int bg, int bb, int ba, int tr, int tg, int tb, int ta, char *str )
{
       // SDL_SetRenderDrawBlendMode(SDL_BLENDMODE_MASK);
//	SDL_SetRenderDrawColor( br, bg, bb, ba );
	GLES2D_DrawRectangle( x, y, width, height );
	//SDL_SetRenderDrawBlendMode(SDL_BLENDMODE_BLEND);
//	GLES2D_SetDrawingColor( tr, tg, tb, ta );
	GLES2D_DrawFontBox( font, x, y, width, height, str );

	//SDL_SetRenderDrawBlendMode(SDL_BLENDMODE_BLEND);
}


GDECLSPEC void GLES2D_SetFontColor( GLES2D_Font *font, int r, int g, int b, int a )
{
	int i;
	float _r = r/255.0, _g = g/255.0, _b = b/255.0, _a = a/255.0;

	for ( i = 0; i < 128; i++ )
	{
		if ( font->texture[i] != NULL )
		{
			font->texture[i]->color[0] = _r;
			font->texture[i]->color[1] = _g;
			font->texture[i]->color[2] = _b;
			font->texture[i]->color[3] = _a;
		}
	}
}

GDECLSPEC void GLES2D_SetFontAlpha( GLES2D_Font *font, int alpha )
{
	int i;
	float _a = alpha/255.0;

	for ( i = 0; i < 128; i++ )
	{
		if ( font->texture[i] != NULL )
		{
			font->texture[i]->color[0] = 1.0f;
			font->texture[i]->color[1] = 1.0f;
			font->texture[i]->color[2] = 1.0f;
			font->texture[i]->color[3] = _a;
		}
	}
}

GDECLSPEC void GLES2D_SetFontFiltering( GLES2D_Font *font, int param )
{
	int i;

	for ( i = 0; i < 128; i++ )
	{
		if ( font->texture[i] != NULL )
		{
			GLES2D_SetTextureFiltering( font->texture[i], 1 );
		}
	}
}

GDECLSPEC void GLES2D_FreeFont( GLES2D_Font *font )
{
	int i;

	for ( i = 0; i < 128; i++ )
	{
		if ( font->texture[i] != NULL )
			GLES2D_FreeTexture( font->texture[i] );
	}

	free ( font  );
	font  = NULL;
}

GDECLSPEC void GLES2D_SetFontCacheColor( GLES2D_FontCache *font, int r, int g, int b, int a )
{
	float _r = r/255.0, _g = g/255.0, _b = b/255.0, _a = a/255.0;

	font->texture->color[0] = _r;
	font->texture->color[1] = _g;
	font->texture->color[2] = _b;
	font->texture->color[3] = _a;
}

GDECLSPEC void GLES2D_FreeFontCache( GLES2D_FontCache *font )
{
	if ( font->texture != NULL )
		GLES2D_FreeTexture( font->texture );

	font->texture = NULL;

	free ( font  );
	font  = NULL;
}


