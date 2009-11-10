#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <stdio.h>

#include <SDL/SDL.h>
#include "GLES2D/GLES2D_platform.h"
#include "GLES2D/GLES2D_texture.h"

static Uint8 iR=0, iG=0, iB=0, iA=0;
static int xstart;
static int xend;
static int ystart;
static int yend;
static int x, y;

#include "assert.h"

#define SDL_COLLIDE_MAX(a,b)	((a > b) ? a : b)
#define SDL_COLLIDE_MIN(a,b)	((a < b) ? a : b)

/* Return pixel color */
Uint32 GLES2D_getPixelColor( SDL_Surface *surf, int x, int y )
{
	int bpp = surf->format->BytesPerPixel;
	Uint8 *p = (Uint8 *)surf->pixels + y * surf->pitch + x * bpp;
	
	switch (bpp)
	{
		case 1:
			return *p;
			
		case 2:
			return *(Uint16 *)p;
			
		case 3:
			if (SDL_BYTEORDER == SDL_BIG_ENDIAN)
				return p[0] << 16 | p[1] << 8 | p[2];
			else
				return p[0] | p[1] << 8 | p[2] << 16;
			
		case 4:
			return *(Uint32 *)p;
			
		default:
			return 0;
	}
}

int GLES2D_CollideTransparentPixel( GLES2D_Texture *texture, int x , int y )
{
	/*assert that (x,y) offsets lie within surface*/
	assert( ( x < texture->src->w ) && ( y < texture->src->h ) );

	SDL_GetRGBA( GLES2D_getPixelColor( texture->collision, x, y ), texture->collision->format, &iR, &iG, &iB, &iA );

	if ( iA == 0 )
		return 1;

	return 0;
}


int GLES2D_CollideColorPixel( GLES2D_Texture *texture, int x , int y, int *color )
{
	/*assert that (x,y) offsets lie within surface*/
	assert( ( x < texture->src->w ) && ( y < texture->src->h ) );

	SDL_GetRGBA( GLES2D_getPixelColor( texture->collision, x, y ), texture->collision->format, &iR, &iG, &iB, &iA );
	
	if ( ( iR == color[0] ) && ( iG == color[1] ) && ( iB == color[2] ) && ( iA == color[3] ) )
		return 1;

	return 0;
}

/*
	pixel color perfect collision test
*/
GDECLSPEC int GLES2D_CollidePixelColor( GLES2D_Texture *texture1, GLES2D_Texture *texture2, int *color, int skip )
{
	/*a - bottom right co-ordinates in world space*/
	int ax1 = texture1->src->x + texture1->src->w - 1;
	int ay1 = texture1->src->y + texture1->src->h - 1;

	/*b - bottom right co-ordinates in world space*/
	int bx1 = texture2->src->x + texture2->src->w - 1;
	int by1 = texture2->src->y + texture2->src->h - 1;

	/*check if bounding boxes intersect*/
	if((bx1 < texture1->src->x) || (ax1 < texture2->src->x))
		return 0;
	if((by1 < texture1->src->y) || (ay1 < texture2->src->y))
		return 0;

	xstart = SDL_COLLIDE_MAX(texture1->src->x,texture2->src->x);
	xend = SDL_COLLIDE_MIN(ax1,bx1);

	ystart = SDL_COLLIDE_MAX(texture1->src->y,texture2->src->y);
	yend = SDL_COLLIDE_MIN(ay1,by1);

	for(y = ystart ; y <= yend ; y += skip)
	{
		for(x = xstart ; x <= xend ; x += skip)
		{
			/*compute offsets for surface
			before pass to TransparentPixel test*/
			if( ! GLES2D_CollideTransparentPixel( texture1 , x-texture1->src->x , y-texture1->src->y )
					&& GLES2D_CollideColorPixel( texture2 , x-texture2->src->x , y-texture2->src->y, color ) )
				return 1;
		}
	}
	return 0;
}

/*
	pixel perfect collision test
*/
GDECLSPEC int GLES2D_CollidePixel( GLES2D_Texture *texture1, GLES2D_Texture *texture2, int skip )
{
	/*a - bottom right co-ordinates in world space*/
	int ax1 = texture1->src->x + texture1->src->w - 1;
	int ay1 = texture1->src->y + texture1->src->h - 1;

	/*b - bottom right co-ordinates in world space*/
	int bx1 = texture2->src->x + texture2->src->w - 1;
	int by1 = texture2->src->y + texture2->src->h - 1;

	/*check if bounding boxes intersect*/
	if((bx1 < texture1->src->x) || (ax1 < texture2->src->x))
		return 0;
	if((by1 < texture1->src->y) || (ay1 < texture2->src->y))
		return 0;

/*Now lets make the bouding box for which we check for a pixel collision*/

	/*To get the bounding box we do
            Ax1,Ay1______________
                |                |
                |                |
                |                |
                |    Bx1,By1____________
                |        |       |      |
                |        |       |      |
                |________|_______|      |
                         |    Ax2,Ay2   |
                         |              |
                         |              |
                         |___________Bx2,By2

	To find that overlap we find the biggest left hand cordinate
	AND the smallest right hand co-ordinate

	To find it for y we do the biggest top y value
	AND the smallest bottom y value

	Therefore the overlap here is Bx1,By1 --> Ax2,Ay2

	Remember	Ax2 = Ax1 + SA->w
			Bx2 = Bx1 + SB->w

			Ay2 = Ay1 + SA->h
			By2 = By1 + SB->h
	*/

	/*now we loop round every pixel in area of
	intersection
		if 2 pixels alpha values on 2 surfaces at the
		same place != 0 then we have a collision*/
	xstart = SDL_COLLIDE_MAX(texture1->src->x,texture2->src->x);
	xend = SDL_COLLIDE_MIN(ax1,bx1);
	ystart = SDL_COLLIDE_MAX(texture1->src->y,texture2->src->y);
	yend = SDL_COLLIDE_MIN(ay1,by1);

	for(y = ystart ; y <= yend ; y += skip)
	{
		for(x = xstart ; x <= xend ; x += skip)
		{
			/*compute offsets for surface
			before pass to TransparentPixel test*/
			if(!GLES2D_CollideTransparentPixel(texture1 , x-texture1->src->x , y-texture1->src->y)
					&& !GLES2D_CollideTransparentPixel(texture2 , x-texture2->src->x , y-texture2->src->y))
				return 1;
		}
	}
	return 0;
}

/*
	Bounding box collision
*/
GDECLSPEC int GLES2D_Collide( GLES2D_Texture *texture1, GLES2D_Texture *texture2 )
{

	if( ( texture2->dst->x + texture2->dst->w < texture1->dst->x ) || \
		( texture2->dst->x > texture1->dst->x + texture2->dst->w ) || \
		( texture2->dst->y + texture2->dst->h < texture1->dst->y ) || \
		( texture2->dst->y > texture1->dst->y + texture1->dst->h ) )
		return 0;

	return 1;				//bounding boxes intersect
}



/*
	tests whether 2 circles intersect

	circle1 : centre (x1,y1) with radius r1
	circle2 : centre (x2,y2) with radius r2
	
	(allow distance between circles of offset)
*/
/*
int CollideBoundingCircle(int x1 , int y1 , int r1 ,
                                 int x2 , int y2 , int r2 , int offset)
{
	int xdiff = x2 - x1;	// x plane difference
	int ydiff = y2 - y1;	// y plane difference
	
	// distance between the circles centres squared 
	int dcentre_sq = (ydiff*ydiff) + (xdiff*xdiff);
	
	// calculate sum of radiuses squared 
	int r_sum_sq = r1 + r2;	// square on seperate line, so
	r_sum_sq *= r_sum_sq;	// dont recompute r1 + r2

	return (dcentre_sq - r_sum_sq <= (offset*offset));
}
*/

/*
	a circle intersection detection algorithm that will use
	the position of the centre of the surface as the centre of
	the circle and approximate the radius using the width and height
	of the surface (for example a rect of 4x6 would have r = 2.5).
*/
/*
int CollideBoundingCircle(SDL_Surface *a , int x1 , int y1 ,
                                 SDL_Surface *b , int x2 , int y2 ,
                                   int offset)
{
	//if radius is not specified
	//we approximate them using SDL_Surface's
	//width and height average and divide by 2
	int r1 = (a->w + a->h) / 4;	// same as / 2) / 2;
	int r2 = (b->w + b->h) / 4;

	x1 += a->w / 2;		// offset x and y
	y1 += a->h / 2;		// co-ordinates into
				// centre of image
	x2 += b->w / 2;
	y2 += b->h / 2;

	return CollideBoundingCircle(x1,y1,r1,x2,y2,r2,offset);
}
*/

