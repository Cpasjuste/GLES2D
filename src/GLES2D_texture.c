#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>

#include <GLES/gl.h>
#include <GLES/egl.h>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <GLES2D/GLES2D_platform.h>
#include <GLES2D/GLES2D_texture.h>
#include <GLES2D/GLES2D_drawing.h>

GDECLSPEC void GLES2D_Enable2D();
GDECLSPEC void GLES2D_Disable2D();

static __inline__ int GLES2D_p2( int input )
{
	int value = 1;

	 while (value < input)
	{
		value <<= 1;
	}
	return value;
}

GDECLSPEC GLES2D_Texture *GLES2D_CreateTextureFromBuffer()
{
	GLES2D_Texture *tex;
	tex = (GLES2D_Texture *) malloc( sizeof( GLES2D_Texture ) );

	GLES2D_Rect *src;
	src = (GLES2D_Rect *) malloc( sizeof( GLES2D_Rect ) );

	GLES2D_Rect *dst;
	dst = (GLES2D_Rect *) malloc( sizeof( GLES2D_Rect ) );

	tex->src = src;
	tex->dst = dst;

	int w = GLES2D_p2( 800 );
	int h = GLES2D_p2( 480 );
	tex->texw = (GLfloat) 800 / w;
	tex->texh = (GLfloat) 480 / h;
	tex->format = RGB565;
	tex->src->w = 800;
	tex->src->h = 480;
	tex->src->x = 0;
	tex->src->y = 0;
	tex->dst->w = 800;
	tex->dst->h = 480;
	tex->dst->x = 0;
	tex->dst->y = 0;
	tex->image_w = 800;
	tex->image_h = 480;
	tex->isbuffer = 1;
	tex->filtering = 0;
	tex->centered = 0;
	tex->rotation = 0;
	tex->color[0] = 1.0f;
	tex->color[1] = 1.0f;
	tex->color[2] = 1.0f;
	tex->color[3] = 1.0f;
	tex->colorAlpha[0] = 1.0f;
	tex->colorAlpha[1] = 1.0f;
	tex->colorAlpha[2] = 1.0f;
	tex->collision_set = 0;
	tex->flipX = 0;
	tex->flipY = 0;

	glGenTextures( 1, &tex->texID );
 
	// Bind the texture object
	glBindTexture( GL_TEXTURE_2D, tex->texID );
 
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );

	glCopyTexImage2D( GL_TEXTURE_2D, 0, GL_RGB, 0, 0, w, h, 0 );

	return tex;
}

GDECLSPEC GLES2D_Texture *GLES2D_CreateTextureFromSurface( SDL_Surface *surface, int storePixelsColors )
{
	GLint GLinternalFormat;
	GLenum GLformat, GLtype;
	SDL_Surface *tmp;

	GLES2D_Texture *tex;
	tex = (GLES2D_Texture *) malloc( sizeof( GLES2D_Texture ) );

	GLES2D_Rect *src;
	src = (GLES2D_Rect *) malloc( sizeof( GLES2D_Rect ) );

	GLES2D_Rect *dst;
	dst = (GLES2D_Rect *) malloc( sizeof( GLES2D_Rect ) );

	tex->src = src;
	tex->dst = dst;

	tex->collision = NULL;

	tex->collision = surface;

	if ( tex->collision != NULL )
	{
		int w = GLES2D_p2( tex->collision->w );
		int h = GLES2D_p2( tex->collision->h );
		tex->texw = (GLfloat) tex->collision->w / w;
		tex->texh = (GLfloat) tex->collision->h / h;

		//Check the format
		switch ( tex->collision->format->BitsPerPixel )
		{
			case 8:
				gprintf( "Loading surface (8bpp, " );
				tex->format = RGB565;
				GLinternalFormat = GL_RGB;
				GLformat = GL_RGB;
				GLtype = GL_UNSIGNED_SHORT_5_6_5;
				tmp = SDL_CreateRGBSurface( SDL_SWSURFACE, w, h, tex->format, 0XF800, 0X7E0, 0x1F, 0x0000 );
			break;

			case 16:
				gprintf( "Loading surface (16bpp, " );
				tex->format = RGB565;
				GLinternalFormat = GL_RGB;
				GLformat = GL_RGB;
				GLtype = GL_UNSIGNED_SHORT_5_6_5;
				tmp = SDL_CreateRGBSurface( SDL_SWSURFACE, w, h, tex->format, 0XF800, 0X7E0, 0x1F, 0x0000 );
			break;

			case 24:
				gprintf( "Loading surface (24bpp, " );
				tex->format = RGB24;
				GLinternalFormat = GL_RGB;
				GLformat = GL_RGB;
				GLtype = GL_UNSIGNED_BYTE;
				tmp = SDL_CreateRGBSurface( SDL_SWSURFACE, w, h, tex->format, 0x000000FF, 0x0000FF00, 0x00FF0000, 0xFF000000 );

			break;

			case 32:
				gprintf( "Loading surface (32bpp, " );
				tex->format = RGBA8888;
				GLinternalFormat = GL_RGBA;
				GLformat = GL_RGBA;
				GLtype = GL_UNSIGNED_BYTE;
				tmp = SDL_CreateRGBSurface( SDL_SWSURFACE, w, h, tex->format, 0x000000FF, 0x0000FF00, 0x00FF0000, 0xFF000000 );
				SDL_SetAlpha( tex->collision, 0, 0 );

			break;

			default:
				gprintf( "Unknown image format for surface" );
				SDL_FreeSurface( tex->collision );
				return 0;
			break;
		}

		gprintf("image w/h: %i/%i, texture w/h: %i/%i )\n", tex->collision->w, tex->collision->h, w, h );

		SDL_BlitSurface( tex->collision, 0, tmp, 0 );
        
		// Have OpenGL generate a texture object handle for us
		glGenTextures( 1, &tex->texID );
 
		// Bind the texture object
		glBindTexture( GL_TEXTURE_2D, tex->texID );
 
		// Set the texture's stretching properties
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );	
 
		// Edit the texture object's image data using the information SDL_Surface gives us
		glTexImage2D( GL_TEXTURE_2D, 0, GLinternalFormat, w, h, 0, GLformat, GLtype, tmp->pixels );

		SDL_FreeSurface( tmp );
	} 
	else
	{
		gprintf("SDL could not load texture from surface (%s) \n", SDL_GetError() );
		return 0;
	}    

	tex->src->w = tex->collision->w;
	tex->src->h = tex->collision->h;
	tex->src->x = 0;
	tex->src->y = 0;
	tex->dst->w = tex->collision->w;
	tex->dst->h = tex->collision->h;
	tex->dst->x = 0;
	tex->dst->y = 0;
	tex->image_w = tex->collision->w;
	tex->image_h = tex->collision->h;
	tex->isbuffer = 0;
	tex->filtering = 0;
	tex->centered = 0;
	tex->rotation = 0;
	tex->color[0] = 1.0f;
	tex->color[1] = 1.0f;
	tex->color[2] = 1.0f;
	tex->color[3] = 1.0f;
	tex->colorAlpha[0] = 1.0f;
	tex->colorAlpha[1] = 1.0f;
	tex->colorAlpha[2] = 1.0f;
	tex->flipX = 0;
	tex->flipY = 0;
	
	if ( storePixelsColors )
		tex->collision_set = 1;
	else
		tex->collision_set = 0;

	return tex;
}

GDECLSPEC GLES2D_Texture *GLES2D_CreateTexture( char *filename, int storePixelsColors )
{
	GLES2D_Texture *tex = NULL;
	SDL_Surface *tmp = NULL;

	if ( ( tmp = IMG_Load( filename ) ) )
	{
		tex = GLES2D_CreateTextureFromSurface( tmp, storePixelsColors );
		SDL_FreeSurface( tmp );

		if ( tex == NULL )
		{
			gprintf("SDL could not load texture from surface (%s) \n", SDL_GetError() );
			return 0;
		}
	}
	else
	{
		gprintf("SDL could not load texture from surface (%s) \n", SDL_GetError() );
		return 0;	
	}

	return tex;
}


void GLES2D_DrawTexture( GLES2D_Texture *texture, GLES2D_Rect *src, GLES2D_Rect *dst, int centered, float rotation )
{
	if ( texture->format == RGBA8888 )
	{
		glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	}

	glEnable( GL_TEXTURE_2D );
	glBindTexture( GL_TEXTURE_2D, texture->texID );

	glPushMatrix();

	glColor4f( texture->color[0], texture->color[1], texture->color[2], texture->color[3] );

//	glColor4f( texture->colorAlpha[0], texture->colorAlpha[1], texture->colorAlpha[2], 0.5f );

	if ( rotation )
		texture->rotation = rotation;

	if ( centered )
	{
		texture->centered = 1;
		texture->dst->x = dst->x - texture->dst->w / 2;
		texture->dst->y = dst->y - texture->dst->h / 2;	
	}
	else
	{
		texture->centered = 0;
		texture->dst->x = dst->x;
		texture->dst->y = dst->y;
	}

	texture->dst->w = dst->w;
	texture->dst->h = dst->h;
	texture->src->x = src->x;
	texture->src->y = src->y;
	texture->src->w = src->w;
	texture->src->h = src->h;

	if ( texture->rotation )
	{
		glTranslatef( texture->dst->x + ( texture->dst->w / 2 ), texture->dst->y + ( texture->dst->h / 2 ), 0.0f);
		glRotatef( texture->rotation, 0.0f, 0.0f, 1.0f );
		glTranslatef( -( texture->dst->w / 2 ), -( texture->dst->h / 2 ), 0.0f );
	}
	else
	{
		glTranslatef( texture->dst->x, texture->dst->y, 0.0f );
	}

	if ( texture->isbuffer )
	{
		glTranslatef( 0.0f, +texture->dst->h, 0.0f );
		glScalef( 1, -1, 1 );
	}

	if ( texture->flipX )
	{
		glTranslatef( +texture->dst->w, 0.0f, 0.0f );
		glScalef( -1, 1, 1 );
	}
	
	if ( texture->flipY )
	{
		glTranslatef( 0.0f, +texture->dst->h, 0.0f );
		glScalef( 1, -1, 1 );
	}

	int minx, miny, maxx, maxy;
 	GLfloat minu, maxu, minv, maxv;

        minx = 0;
        miny = 0;
        maxx = dst->w;
        maxy = dst->h;

        minu = (GLfloat) src->x / texture->image_w;
        minu *= texture->texw;

        maxu = (GLfloat) (src->x + src->w) / texture->image_w;
        maxu *= texture->texw;

        minv = (GLfloat) src->y / texture->image_h;
        minv *= texture->texh;

        maxv = (GLfloat) (src->y + src->h) / texture->image_h;
        maxv *= texture->texh;

        GLshort vertices[8];
        GLfloat texCoords[8];

        vertices[0] = minx;
        vertices[1] = miny;
        vertices[2] = maxx;
        vertices[3] = miny;
        vertices[4] = minx;
        vertices[5] = maxy;
        vertices[6] = maxx;
        vertices[7] = maxy;

        texCoords[0] = minu;
        texCoords[1] = minv;
        texCoords[2] = maxu;
        texCoords[3] = minv;
        texCoords[4] = minu;
        texCoords[5] = maxv;
        texCoords[6] = maxu;
        texCoords[7] = maxv;

        glVertexPointer(2, GL_SHORT, 0, vertices);
        glEnableClientState(GL_VERTEX_ARRAY);
        glTexCoordPointer(2, GL_FLOAT, 0, texCoords);
        glEnableClientState(GL_TEXTURE_COORD_ARRAY);
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
        glDisableClientState(GL_TEXTURE_COORD_ARRAY);
        glDisableClientState(GL_VERTEX_ARRAY);

	glDisable(GL_TEXTURE_2D);

	if ( texture->format == RGBA8888 )
	{
		glDisable(GL_BLEND);
	}

	glPopMatrix();
}

GDECLSPEC void GLES2D_DrawTextureSimple( GLES2D_Texture *texture, int x, int y )
{
	texture->dst->x = x;
	texture->dst->y = y;
	texture->centered = 0;
	texture->rotation = 0;

	GLES2D_DrawTexture( texture, texture->src, texture->dst, texture->centered, texture->rotation );
}

GDECLSPEC void GLES2D_DrawTextureCentered( GLES2D_Texture *texture, int x, int y )
{
	texture->dst->x = x;
	texture->dst->y = y;
	texture->centered = 1;
	texture->rotation = 0;

	GLES2D_DrawTexture ( texture, texture->src, texture->dst, texture->centered, 0 );
}

GDECLSPEC void GLES2D_DrawTextureScaled( GLES2D_Texture *texture, int x, int y, int w, int h )
{
	texture->dst->x = x;
	texture->dst->y = y;
	texture->dst->w = w;
	texture->dst->h = h;
	texture->centered = 0;
	texture->rotation = 0;

	GLES2D_DrawTexture ( texture, texture->src, texture->dst, texture->centered, texture->rotation );
}

GDECLSPEC void GLES2D_DrawTextureScaledCentered( GLES2D_Texture *texture, int x, int y, int w, int h )
{
	texture->dst->x = x;
	texture->dst->y = y;
	texture->dst->w = w;
	texture->dst->h = h;
	texture->centered = 1;
	texture->rotation = 0;

	GLES2D_DrawTexture ( texture, texture->src, texture->dst, texture->centered, texture->rotation );
}

GDECLSPEC void GLES2D_DrawTextureRotated( GLES2D_Texture *texture, int x, int y, float rotation )
{
	texture->dst->x = x;
	texture->dst->y = y;
	texture->centered = 0;
	texture->rotation = rotation;

	GLES2D_DrawTexture( texture, texture->src, texture->dst, texture->centered, texture->rotation );
}

GDECLSPEC void GLES2D_DrawTextureRotatedCentered( GLES2D_Texture *texture, int x, int y, float rotation )
{
	texture->dst->x = x;
	texture->dst->y = y;
	texture->centered = 1;
	texture->rotation = rotation;

	GLES2D_DrawTexture( texture, texture->src, texture->dst, texture->centered, texture->rotation );
}

GDECLSPEC void GLES2D_DrawTextureRotatedCenteredScaled( GLES2D_Texture *texture, int x, int y, int w, int h, float rotation )
{
	texture->dst->x = x;
	texture->dst->y = y;
	texture->dst->w = w;
	texture->dst->h = h;
	texture->centered = 1;
	texture->rotation = rotation;

	GLES2D_DrawTexture ( texture, texture->src, texture->dst, texture->centered, texture->rotation );
}

GDECLSPEC void GLES2D_SetTextureColor( GLES2D_Texture *texture, int r, int g, int b, int a )
{
	float _r = r/255.0, _g = g/255.0, _b = b/255.0, _a = a/255.0;

	texture->color[0] = _r;
	texture->color[1] = _g;
	texture->color[2] = _b;
	texture->color[3] = _a;
}

GDECLSPEC void GLES2D_SetTextureAlpha( GLES2D_Texture *texture, int alpha )
{
	float _a = alpha/255.0;

	texture->color[0] = 1.0f;
	texture->color[1] = 1.0f;
	texture->color[2] = 1.0f;
	texture->color[3] = _a;
}

GDECLSPEC void GLES2D_SetTextureAlphaColor( GLES2D_Texture *texture, int r, int g, int b )
{
	float _r = r/255.0, _g = g/255.0, _b = b/255.0;

	texture->colorAlpha[0] = _r;
	texture->colorAlpha[1] = _g;
	texture->colorAlpha[2] = _b;
}

GDECLSPEC void GLES2D_SetTextureFiltering( GLES2D_Texture *texture, int param )
{
	glEnable( GL_TEXTURE_2D );
	glBindTexture( GL_TEXTURE_2D, texture->texID );

	texture->filtering = param;

	if ( texture->filtering )
	{
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );		
	}
	else
	{
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );	
	}

	glDisable( GL_TEXTURE_2D );
}

GDECLSPEC void GLES2D_SetTextureFlipX( GLES2D_Texture *texture, int param )
{
	texture->flipX = param;
}

GDECLSPEC void GLES2D_SetTextureFlipY( GLES2D_Texture *texture, int param )
{
	texture->flipY = param;
}

GDECLSPEC void GLES2D_FreeTexture( GLES2D_Texture *texture )
{
	GLES2D_Texture *tex;
	tex = texture;

	if ( texture->collision_set )
		SDL_FreeSurface ( texture->collision );

	glDeleteTextures( 1, &texture->texID );

	free ( tex->dst );
	free ( tex->src );
	free ( tex );

	tex = NULL;
}


