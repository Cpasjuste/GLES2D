#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

#include <SDL/SDL.h>
#include "GLES2D/GLES2D_platform.h"
#include <GLES2D/GLES2D_texture.h>
#include <GLES2D/GLES2D_sprite.h>


#define SPRITE_RIGHT 0
#define SPRITE_LEFT 1

	

	//SDL_Surface *sdl_Surface;
	SDL_Rect rect_Position;
	SDL_Rect rect_WHUV;
	int iNumOfFrames;
	int iCurrentFrame;
	SDL_bool bSpriteIsVisible;
	int iFrameSpeed;
	SDL_bool bPlayAnimation;
	int ticker;

	SDL_Surface *sdl_Surface;

	int iX;
	int iY;

	//SDL_Sprite();
	//SDL_Sprite(char *cFilename,int iFrameWidth, int iFrameHeight, int iU, int iV, int iNumOfFrame);

	void Intialize(char *cFilename,int iFrameWidth, int iFrameHeight, int iU, int iV, int iNOfFrames, int *colorkey);

	void Draw3(int iFrame, SDL_Surface *sdl_Screen);  // Draw a Single Frame
	void Draw2(); // Draw an animation
	void Draw1( int dir ); // Draw an animation from the sprite's
												 // right or left side.
												 // Makes Sprite Flipping easy.

	// Set the UV of the Sprite
	void SetUV(int iU, int iV);
	void SetU(int iU);
	void SetV(int iV);

	int GetU();
	int GetV();

	// Set the Width and Height of the Sprite
	void SetWH(int iWidth, int iHeight);
	void SetWidthHeight(int iWidth, int iHeight);
	void SetWidth(int iWidth);
	void SetHeight(int iHeight);
	
	void SetW(int iWidth);
	void SetH(int iHeight);

	void SetNumberOfFrames(int iNFrames);

	void Visible(SDL_bool visible);

	void SetTransparentColor(Uint8 r, Uint8 g, Uint8 b);

	void SetFrameSpeed(int iFSpeed);

//	SDL_bool CheckCollision(SDL_Sprite sprite2);

	void Play();
	void Pause();
	void Stop();

	int GetCurrentFrame();
	int GetNumberOfFrames();

/*returns maximum or minimum of number*/
#define SDL_COLLIDE_MAX(a,b)	((a > b) ? a : b)
#define SDL_COLLIDE_MIN(a,b)	((a < b) ? a : b)

int GLES2D_CollideTransparentPixel( GLES2D_Texture *texture, int x , int y );
Uint32 GLES2D_getPixelColor(SDL_Surface *surf, int x, int y);
int GLES2D_CollideColorPixel( GLES2D_Texture *texture, int x , int y, int *color );
static Uint8 r=0, g=0, b=0, a=0;
int xstart;
int xend;
int ystart;
int yend;
int x, y;
int iii;

int GLES2D_CollideTransparentPixelSprite( GLES2D_sprite *sprite, int x , int y )
{
	/*assert that (u,v) offsets lie within surface*/
	assert( ( x < sprite->texture->src->w ) && ( y < sprite->texture->src->h ) );

	SDL_GetRGBA( GLES2D_getPixelColor( sprite->collision, x, y ), sprite->collision->format, &r, &g, &b, &a );

	if ( ( r == sprite->alphaRGBA[0] ) && \
		( g == sprite->alphaRGBA[1] ) && \
		( b == sprite->alphaRGBA[2] ) && \
		( a == sprite->alphaRGBA[3] ) )
		return 1;

	return 0;
}

GDECLSPEC int GLES2D_CollidePixelColorSprite( GLES2D_sprite *sprite, GLES2D_Texture *texture, int *color, int skip )
{
	/*a - bottom right co-ordinates in world space*/
	int ax1 = sprite->texture->dst->x + sprite->texture->dst->w - 1;
	int ay1 = sprite->texture->dst->y + sprite->texture->dst->h - 1;

	/*b - bottom right co-ordinates in world space*/
	int bx1 = texture->dst->x + texture->dst->w - 1;
	int by1 = texture->dst->y + texture->dst->h - 1;

	/*check if bounding boxes intersect*/
	if((bx1 < sprite->texture->dst->x) || (ax1 < texture->dst->x))
		return 0;
	if((by1 < sprite->texture->dst->y) || (ay1 < texture->dst->y))
		return 0;

	xstart = SDL_COLLIDE_MAX(sprite->texture->dst->x,texture->dst->x);
	xend = SDL_COLLIDE_MIN(ax1,bx1);

	ystart = SDL_COLLIDE_MAX(sprite->texture->dst->y,texture->dst->y);
	yend = SDL_COLLIDE_MIN(ay1,by1);

	for(y = ystart ; y <= yend ; y += skip)
	{
		for(x = xstart ; x <= xend ; x += skip)
		{
			/*compute offsets for surface
			before pass to TransparentPixel test*/
			if( ! GLES2D_CollideTransparentPixelSprite( sprite , x-sprite->texture->dst->x , y-sprite->texture->dst->y )
			&& GLES2D_CollideColorPixel( texture , x-texture->dst->x , y-texture->dst->y, color ) )
				return 1;
		}
	}
	return 0;
}

GDECLSPEC int GLES2D_CollidePixelSprite( GLES2D_sprite *sprite, GLES2D_Texture *texture, int skip )
{
	/*a - bottom right co-ordinates in world space*/
	int ax1 = sprite->texture->dst->x + sprite->texture->dst->w - 1;
	int ay1 = sprite->texture->dst->y + sprite->texture->dst->h - 1;

	/*b - bottom right co-ordinates in world space*/
	int bx1 = texture->dst->x + texture->dst->w - 1;
	int by1 = texture->dst->y + texture->dst->h - 1;

	/*check if bounding boxes intersect*/
	if((bx1 < sprite->texture->dst->x) || (ax1 < texture->dst->x))
		return 0;
	if((by1 < sprite->texture->dst->y) || (ay1 < texture->dst->y))
		return 0;

	xstart = SDL_COLLIDE_MAX(sprite->texture->dst->x,texture->dst->x);
	xend = SDL_COLLIDE_MIN(ax1,bx1);

	ystart = SDL_COLLIDE_MAX(sprite->texture->dst->y,texture->dst->y);
	yend = SDL_COLLIDE_MIN(ay1,by1);

	for(y = ystart ; y <= yend ; y += skip)
	{
		for(x = xstart ; x <= xend ; x += skip)
		{
			/*compute offsets for surface
			before pass to TransparentPixel test*/
			if( ! GLES2D_CollideTransparentPixelSprite( sprite , x-sprite->texture->dst->x , y-sprite->texture->dst->y ) && ! GLES2D_CollideTransparentPixel( texture , x-texture->dst->x , y-texture->dst->y ) )
				return 1;
		}
	}
	return 0;
}

GLES2D_sprite *GLES2D_SpriteInit(char *filename, int frameWidth, int frameHeight, int framesCount, int startingFrameLayer, int *colorkey, int storePixelsColors )
{
	GLES2D_sprite *sprite; 
	sprite = (GLES2D_sprite *) malloc( sizeof( GLES2D_sprite ) );

	//sprite->texture = GLES2D_CreateTexture( filename, 0 );

	sprite->texture = GLES2D_CreateTexture( filename, 0 );
	GLES2D_SetTextureAlphaColor( sprite->texture, r, g, b );
/*
	sprite->collision = IMG_Load( filename );
	if( sprite->collision == NULL )
	{
		printf ( "Error : GLES2D_SpriteInit @ IMG_Load(%s)\n", filename);
		return NULL;
	}

	SDL_SetColorKey( sprite->collision, SDL_SRCCOLORKEY, SDL_MapRGBA( sprite->collision->format, colorkey[0], colorkey[1], colorkey[2], colorkey[3] ) );

	sprite->texID = SDL_CreateTextureFromSurface( SDL_PIXELFORMAT_ABGR8888, sprite->collision );
	if (sprite->texID == 0)
	{
		printf("Error : GLES2D_SpriteInit @ SDL_CreateTextureFromSurface()\n");
		return NULL;
	}

	if ( ! storePixelsColors )
		SDL_FreeSurface ( sprite->collision );

	SDL_SetTextureBlendMode( sprite->texID, SDL_BLENDMODE_BLEND );
*/

	sprite->alphaRGBA[0] = colorkey[0];
	sprite->alphaRGBA[1] = colorkey[1];
	sprite->alphaRGBA[2] = colorkey[2];
	sprite->alphaRGBA[3] = colorkey[3];
	sprite->texture->src->x = 0;
	sprite->texture->src->y = 0;			
	sprite->texture->src->w = frameWidth;	
	sprite->texture->src->h = frameHeight;			
	sprite->texture->dst->x = 0;		
	sprite->texture->dst->y = 0;
	sprite->texture->dst->w = frameWidth;	
	sprite->texture->dst->h = frameHeight;
	sprite->framesCount = framesCount - 1;
	sprite->frameCurrent = 0;
	sprite->frameSpeed = 100;
	sprite->visible = 1;
	sprite->playing = 0;
	sprite->frameLayer = startingFrameLayer;
	sprite->ticker = 0;

	return sprite;
}

GDECLSPEC void GLES2D_SpriteDraw( GLES2D_sprite *sprite, int x, int y )
{	
	sprite->texture->src->y = sprite->texture->src->h * ( sprite->frameLayer - 1);

	if( ( SDL_GetTicks() > sprite->ticker || sprite->ticker == 0 ) && sprite->playing == 1 )
	{
		// What frame are we on?
		if( sprite->frameCurrent < sprite->framesCount )
		{
			sprite->texture->src->x += sprite->texture->src->w;
			sprite->frameCurrent++;
		}
		else
		{
			sprite->texture->src->x = 0;
			sprite->frameCurrent = 0;
		}
		sprite->ticker = SDL_GetTicks() + sprite->frameSpeed;
	}

	if( sprite->visible )
	{
		GLES2D_DrawTextureSimple( sprite->texture, x, y );
/*
		SDL_Rect srcRect;
		SDL_Rect dstRect;
 
		srcRect.x = sprite->srcX;
		srcRect.y = sprite->srcY;
		srcRect.w = sprite->srcW;
		srcRect.h = sprite->srcH;

		dstRect.x = sprite->dstX;
		dstRect.y = sprite->dstY;
		dstRect.w = sprite->dstW;
		dstRect.h = sprite->dstH;

		SDL_RenderCopy( sprite->texID, &srcRect, &dstRect );
*/
	}
}

void SetUV(int iU, int iV)
{
	rect_WHUV.x = iU;
	rect_WHUV.y = iV;
}

void SetU(int iU)
{
	rect_WHUV.x = iU;
}

void SetV(int iV)
{
	rect_WHUV.y = iV;
}

	// Set the Width and Height of the Sprite
void SetWH(int iWidth, int iHeight)
{
	rect_WHUV.w = iWidth;
	rect_WHUV.h = iHeight;
}

void SetWidthHeight(int iWidth, int iHeight)
{
	rect_WHUV.w = iWidth;
	rect_WHUV.h = iHeight;
}

void SetWidth(int iWidth)
{
	rect_WHUV.w = iWidth;
}

void SetHeight(int iHeight)
{
	rect_WHUV.h = iHeight;
}
	
void SetW(int iWidth)
{
	rect_WHUV.w = iWidth;
}

void SetH(int iHeight)
{
	rect_WHUV.h = iHeight;
}

int GetU()
{
	return rect_WHUV.x;
}

int GetV()
{
	return rect_WHUV.y;
}

void Visible(SDL_bool visible)
{
	bSpriteIsVisible = visible;
	
}

void SetTransparentColor(Uint8 r, Uint8 g, Uint8 b)
{
	//SDL_SetColorKey(sdl_Surface, SDL_SRCCOLORKEY, SDL_MapRGB(sdl_Surface->format,r,g,b));
}

int GetCurrentFrame()
{
	return iCurrentFrame;
}

int GetNumberOfFrames()
{
	return iNumOfFrames;
}

void SetFrameSpeed(int iFSpeed)
{
	iFrameSpeed = iFSpeed;
}

void SetNumberOfFrames(int iNFrames)
{
	iNumOfFrames = iNFrames-1;
}

void _Play()
{
	bPlayAnimation = SDL_TRUE;
}

void _Pause()
{
	bPlayAnimation = SDL_FALSE;
}

void _Stop()
{
	bPlayAnimation = SDL_FALSE;
	iCurrentFrame = 0;
	rect_WHUV.x=0;
}
