#ifndef __GLES2D_TEXTURE_H__
#define __GLES2D_TEXTURE_H__

#include <GLES/gl.h>
#include <SDL/SDL.h>

#define RGBA8888 32
#define RGB24 24
#define RGB565 16
#define OGL4444 3
#define OGL5551 4
#define OGL8888 5
#define OGL565 6
#define OGL888 7
#define OGL88 8
#define OGL8 9
#define OGLPVRTC2 10
#define OGLPVRTC4 11

typedef struct
{
    int x, y;
    int w, h;
} GLES2D_Rect;

/* Texture structure */
typedef struct
{
	GLuint texID;				/* The texture identifier */
	GLES2D_Rect *src;
	GLES2D_Rect *dst;
	int image_w, image_h;
	int flipX;
	int flipY;
	float color[4];
	float colorAlpha[4];
	int filtering;				/* A flag to retrieve/set texture filtering */
	int centered;				/* A flag to retrieve/set texture centering */
	float rotation;				/* A flag to retrieve/set texture rotation */
	SDL_Surface *collision;			/* A Surface to store pixel colors */
	int collision_set;			/* Internal flag to know if the texture collision surface is active/stored */
	int isbuffer;				/* Internal flag, not used for now */
	int format;				/* Internal flag, hold the texture format */
	GLfloat texw;				/* Internal flag, hold the original image/texture ratio */
	GLfloat texh;				/* Internal flag, hold the original image/texture ratio */
	
} GLES2D_Texture;


/**
  * Load a texture in memory.
  *
  * @param filename : 		The path to an image.
  *				BMP, JPEG, PNG and TIFF formats are supported.
  *
  * @int storePixelsColors : 	If set to 1, a table will cache each pixels colors of the texture,
  *				loading the texture is slower but this allow very fast
  *				pixel perfect collision detection.
  *
  * @return the texture structure on success, NULL on error.
  */
GDECLSPEC GLES2D_Texture *GLES2D_CreateTexture( char *filename, int storePixelsColors );

GDECLSPEC GLES2D_Texture *GLES2D_CreateTextureFromSurface( SDL_Surface *surface, int storePixelsColors );

GDECLSPEC GLES2D_Texture *GLES2D_CreateTextureFromBuffer();

/**
  * Free a texture from memory.
  *
  * @param texture : 	A GLES2D texture structure.
  *
  */
GDECLSPEC void GLES2D_FreeTexture(GLES2D_Texture *texture);

/**
  * Draw a texture.
  *
  * @param texture : 	A GLES2D texture structure.
  *
  * @param x :		The coordinate of the texture on the X axis (upper left corner of the texture).
  *
  * @param y :		The coordinate of the texture on the Y axis (upper left corner of the texture).
  *
  */
GDECLSPEC void GLES2D_DrawTextureSimple( GLES2D_Texture *texture, int x, int y );

/**
  * Draw a centered texture.
  *
  * @param texture : 	A GLES2D texture structure.
  *
  * @param x :		The coordinate of the texture on the X axis (center of the texture).
  *
  * @param y :		The coordinate of the texture on the Y axis (center of the texture).
  *
  */
GDECLSPEC void GLES2D_DrawTextureCentered( GLES2D_Texture *texture, int x, int y );

/**
  * Draw a texture with scaling.
  *
  * @param texture :	A GLES2D texture structure.
  *
  * @param x :		The coordinate of the texture on the X axis (upper left corner of the texture).
  *
  * @param y :		The coordinate of the texture on the Y axis (upper left corner of the texture).
  *
  * @param w :		The desired width of the texture.
  *
  * @param h :		The desired height of the texture.
  *
  */
GDECLSPEC void GLES2D_DrawTextureScaled( GLES2D_Texture *texture, int x, int y, int w, int h );

/**
  * Draw a centered texture with scaling.
  *
  * @param texture :	A GLES2D texture structure.
  *
  * @param x :		The coordinate of the texture on the X axis (center of the texture).
  *
  * @param y :		The coordinate of the texture on the Y axis (center of the texture).
  *
  * @param w :		The desired width of the texture.
  *
  * @param h :		The desired height of the texture.
  *
  */
GDECLSPEC void GLES2D_DrawTextureScaledCentered( GLES2D_Texture *texture, int x, int y, int w, int h );

GDECLSPEC void GLES2D_DrawTextureRotated( GLES2D_Texture *texture, int x, int y, float rotation );
GDECLSPEC void GLES2D_DrawTextureRotatedCentered( GLES2D_Texture *texture, int x, int y, float rotation );
GDECLSPEC void GLES2D_DrawTextureRotatedCenteredScaled( GLES2D_Texture *texture, int x, int y, int w, int h, float rotation );

/**
  * Draw a texture will all the possible options.
  *
  * @param texture :	A GLES2D texture structure.
  *
  * @param x :		The coordinate of the texture on the X axis.
  *
  * @param y :		The coordinate of the texture on the Y axis.
  *
  * @param w :		The desired width of the texture.
  *
  * @param h :		The desired height of the texture.
  *
  * @param centered :	If 1 is passed, the texture will be centered.
  *
  * @param rotation :   The rotation value from 0.0f (no rotation) to 360.0f (complete rotation).
  *
  */
GDECLSPEC void GLES2D_DrawTexture( GLES2D_Texture *texture, GLES2D_Rect *src, GLES2D_Rect *dst, int centered, float rotation );

GDECLSPEC void GLES2D_SetTextureColor( GLES2D_Texture *texture, int r, int g, int b, int a );

GDECLSPEC void GLES2D_SetTextureAlpha( GLES2D_Texture *texture, int alpha );

GDECLSPEC void GLES2D_SetTextureAlphaColor( GLES2D_Texture *texture, int r, int g, int b );

GDECLSPEC void GLES2D_SetTextureFiltering( GLES2D_Texture *texture, int param );

GDECLSPEC void GLES2D_SetTextureFlipX( GLES2D_Texture *texture, int param );
GDECLSPEC void GLES2D_SetTextureFlipY( GLES2D_Texture *texture, int param );
#endif

