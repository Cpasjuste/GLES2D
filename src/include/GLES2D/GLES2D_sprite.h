#ifndef __GLES2D_SPRITE_H__
#define __GLES2D_SPRITE_H__

#include "GLES2D/GLES2D_platform.h"

#define SPRITE_MAX_LAYER 64

typedef struct
{
	GLES2D_Texture *texture;	/* The texture identifier */
	int framesCount;
	int frameCurrent;
	int frameSpeed;
	int frameLayer;
	int visible;
	int playing;
	int ticker;
	int alphaRGBA[4];
	SDL_Surface *collision;

} GLES2D_sprite;

GDECLSPEC GLES2D_sprite *GLES2D_SpriteInit(char *filename, int frameWidth, int frameHeight, int framesCount, int startingFrameLayer, int *colorkey, int storePixelsColors );

GDECLSPEC void GLES2D_SpriteDraw( GLES2D_sprite *sprite, int x, int y );

#endif

