#ifndef __GLES2D_COLLISION_H__
#define __GLES2D_COLLISION_H__

#include "GLES2D/GLES2D_platform.h"
#include "GLES2D/GLES2D_texture.h"
#include "GLES2D/GLES2D_sprite.h"

#define COLLIDE_LEFT 1
#define COLLIDE_RIGHT 2
#define COLLIDE_TOP 3
#define COLLIDE_BOTTOM 4

/**
  * Check for simple collision between two textures (bounding box).
  *
  * @param texture1 : 		The first texture to check
  *
  * @param texture2 : 		The second ...
  *
  * @return 1 if a collision is spotted.
  */
GDECLSPEC int GLES2D_Collide( GLES2D_Texture *texture1, GLES2D_Texture *texture2 );

/**
  * Check for pixel perfect collision between two textures, excluding transparent color.
  * Textures must be loaded with the storePixelsColors param set to 1.
  *
  * @param texture1 : 		The first texture to check
  *
  * @param texture2 : 		The second ...
  *
  * @int skip : 		How many pixels we will skip for detection. Passing 1 is very accurate but slower.
  *
  * @return 1 if a collision is spotted.
  */
GDECLSPEC int GLES2D_CollidePixel( GLES2D_Texture *texture1, GLES2D_Texture *texture2, int skip );

/**
  * Check for pixel perfect collision between two textures, excluding transparent colors,
  * against a given color. Textures must be loaded with the storePixelsColors param set to 1.
  *
  * @param texture1 : 		The first texture to check
  *
  * @param texture2 : 		The second texture we will check for the given color collision
  *
  * @int *color :		An array of four colors ( r, g, b, a )
  *
  * @int skip : 		How many pixels we will skip for detection. Passing 1 is very accurate but slower.
  *
  * @return 1 if a collision is spotted.
  */
GDECLSPEC int GLES2D_CollidePixelColor( GLES2D_Texture *texture1, GLES2D_Texture *texture2, int *color, int skip );

/**
  * Check for pixel perfect collision between a sprite and a texture, excluding transparent (sprite colorkey) color,
  * Texture and sprite must be loaded with the storePixelsColors param set to 1.
  *
  * @param sprite : 		The sprite to check
  *
  * @param texture : 		The texture to check
  *
  * @int skip : 		How many pixels we will skip for detection. Passing 1 is very accurate but slower.
  *
  * @return 1 if a collision is spotted.
  */
GDECLSPEC int GLES2D_CollidePixelSprite( GLES2D_sprite *sprite, GLES2D_Texture *texture, int skip );

/**
  * Check for pixel perfect collision between a sprite and a texture, excluding transparent (sprite colorkey) color
  * against a given color. Textures must be loaded with the storePixelsColors param set to 1.
  *
  * @param sprite : 		The first texture to check
  *
  * @param texture : 		The texture we will check for the given color collision
  *
  * @int *color :		An array of four colors ( r, g, b, a )
  *
  * @int skip : 		How many pixels we will skip for detection. Passing 1 is very accurate but slower.
  *
  * @return 1 if a collision is spotted.
  */
GDECLSPEC int GLES2D_CollidePixelColorSprite( GLES2D_sprite *sprite, GLES2D_Texture *texture, int *color, int skip );

GDECLSPEC int GLES2D_TilesSpriteCollision( GLES2D_sprite *sprite, int direction );
GDECLSPEC int GLES2D_TilesTextureCollision( GLES2D_Texture *sprite, int direction );

#endif
