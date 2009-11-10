#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include <GLES2D/GLES2D.h>

/* Texture */
GLES2D_Font *font;
GLES2D_sprite *sprite1;
GLES2D_Texture *parallax;

int done = 0;
int colorkey[4] = { 255, 0, 255, 255 };
int mapX = 0, mapY = 0;
int spriteX = 300, spriteY = 300;
int layer1X = 0, layer1Y = 0;
int i;

#define PARTICLECOUNT 128
int particleX[PARTICLECOUNT];
int particleY[PARTICLECOUNT];
int particleW[PARTICLECOUNT];
int particleR[PARTICLECOUNT];
int particleG[PARTICLECOUNT];
int particleB[PARTICLECOUNT];
int particleA[PARTICLECOUNT];
int particleSpeed[PARTICLECOUNT];
int particleYMAX[PARTICLECOUNT];

int randomInt(int min, int max)
{
    return min + rand() % (max - min + 1);
}

void exitError ( char *errormsg )
{
	printf( "%s", errormsg );

	GLES2D_TilesMapFree();
	GLES2D_FreeFont( font ); font = NULL;

	GLES2D_QuitVideo();	
	exit(0);
}

/* function to handle events */
void handleEvents()
{
	GLES2D_HandleEvents( 20 );	

	sprite1->playing = 0;

	if ( GLES2D_KeyboardPressed( XK_Escape ) || GLES2D_PadPressed( START ) || GLES2D_PadPressed( X ) )
	{
		done = 1;
	}

	if ( GLES2D_KeyboardHold( XK_Up ) || GLES2D_PadHold( PAD_UP ) )
	{
		sprite1->playing = 1;
		sprite1->frameLayer = 2;

		if ( ! GLES2D_TilesSpriteCollision( sprite1, COLLIDE_TOP ) )
		{	
			if ( ( spriteY + sprite1->texture->dst->h / 2 <= 240 ) && ( mapY > 5 ) )
			{
				mapY -= 5;
				layer1Y -= 6;
			}
			else
			{
				if ( spriteY > 5 )
					spriteY -= 5;
			}
		}

	}
	if ( GLES2D_KeyboardHold( XK_Down ) || GLES2D_PadHold( PAD_DOWN ) )
	{
		sprite1->playing = 1;
		sprite1->frameLayer = 1;

		if ( ! GLES2D_TilesSpriteCollision( sprite1, COLLIDE_BOTTOM )  )
		{
			if ( ( spriteY + sprite1->texture->dst->h / 2 >= 240 ) && ( mapY < GLES2D_TilesMapGetHeight() - 480 ) )
			{
				mapY += 5;
				layer1Y += 6;
			}
			else
			{
				if ( spriteY < 480 - sprite1->texture->dst->h )
					spriteY += 5;

			}
		}
	}

	if ( GLES2D_KeyboardHold( XK_Left ) || GLES2D_PadHold( PAD_LEFT ) )
	{
		sprite1->playing = 1;
		sprite1->frameLayer = 3;

		if ( ! GLES2D_TilesSpriteCollision( sprite1, COLLIDE_LEFT ) )
		{
			if ( ( spriteX + sprite1->texture->dst->w / 2 <= 400 ) && ( mapX > 5 ) )
			{
				mapX -= 5;
				layer1X -= 6;
			}
			else
			{
				if ( spriteX > 5 )
					spriteX -= 5;
			}
		}
	}
	if ( GLES2D_KeyboardHold( XK_Right ) || GLES2D_PadHold( PAD_RIGHT ) )
	{
		sprite1->playing = 1;
		sprite1->frameLayer = 4;

		if ( ! GLES2D_TilesSpriteCollision( sprite1, COLLIDE_RIGHT ) )
		{
			if ( ( spriteX + sprite1->texture->dst->w / 2 >= 400 ) && ( mapX < GLES2D_TilesMapGetWidth() - 800 ) )
			{
				mapX += 5;
				layer1X += 6;
			}
			else
			{
				if ( spriteX < 800 - sprite1->texture->dst->w )
					spriteX += 5;
			}
		}
	}
}

void resetParticle( int i )
{
	particleX[i] = randomInt(0, 800);
	particleY[i] = 0; 
	particleW[i] = randomInt(8, 16);
	particleA[i] = randomInt(100, 255);
	particleSpeed[i] = randomInt(1, 5);
	particleYMAX[i] = randomInt(0, 480);
}

int main(int argc, char *argv[])
{
	/* Win32 debug console, do nothing for other platform's */
	GLES2D_InitDebugConsole();

	/* Init openGL(ES) display in fullscreen with Full Screen Anti Aliasing */
	if ( ! GLES2D_InitVideo( 800, 480, 1, 0, 0 ) )
		exitError("Error : GLES2D_InitVideoMode failed\n");

	/* Load a ttf font in memory */
	font = GLES2D_CreateFont( "../ressources/font2.ttf", TTF_STYLE_NORMAL, 10 );

	/* Check if all data are loaded correctly */
	if ( !font)
		exitError("Error : data loading failed\n");

	/* Load a tilemap done with "mappy" */
	GLES2D_TilesMapLoad( "../ressources/cool.fmp", 0, 0, 800, 480 );
	/* Init tilemap amins done with "mappy" */
	GLES2D_TilesMapInitAnims();

	/* Load a parallax texture */
	parallax = GLES2D_CreateTexture( "../ressources/fog.png", 0 );
	if ( !parallax)
		exitError("Error : parallax loading failed\n");

	GLES2D_FpsCounterInit();

	sprite1 = GLES2D_SpriteInit("../ressources/clive.png", 23, 49, 3, 4, colorkey, 0);

	for ( i = 0; i < PARTICLECOUNT; i++ )
	{
		particleX[i] = randomInt(0, 800);
		particleY[i] = randomInt(0, 480);
		particleW[i] = randomInt(8, 16);
		particleA[i] = randomInt(100, 255);
		particleSpeed[i] = randomInt(1, 5);
		particleYMAX[i] = randomInt(0, 480) + particleY[i];
	}

	/* functions inclued in "common.c" file */
//	choose_joystick( font );
//	choose_mouse( font );

	/* Main loop */
	while ( ! done )
	{
		/* Handle events ... */
		handleEvents();

		GLES2D_FpsCounterUpdate();
		GLES2D_TilesMapUpdateAnims();

		GLES2D_ClearScreen( 0, 0, 0, 255 );

		GLES2D_MapChangeLayer(0);
		GLES2D_TilesMapMoveTo( mapX, mapY );
		GLES2D_TilesMapDraw ( 800, 480 );

		GLES2D_SpriteDraw( sprite1, spriteX, spriteY );

		GLES2D_TilesMapDrawFG ( 800, 480 );

		for ( i = 0; i < PARTICLECOUNT; i++ )
		{
			if ( particleY[i] > particleYMAX[i] ) 
			{
				resetParticle( i );
			}
			GLES2D_DrawParticle ( particleX[i], particleY[i], particleW[i], 255, 255, 255, particleA[i] );

			particleY[i] += particleSpeed[i];
		}

		//GLES2D_TilesMapDrawMovingParallax ( parallax, 800, 480, GLES2D_LEFT, 1 );
	
		GLES2D_DrawFont( font, 5, 460, GLES2D_GetFpsChar() );		

		/* Swap buffers (render to screen) */
		GLES2D_SwapBuffers();
	}

	/* Free all data */
	GLES2D_TilesMapFree();
	GLES2D_FreeFont( font ); font = NULL;

	/* Quit properly... */
	GLES2D_QuitVideo();

	return 0;
}

