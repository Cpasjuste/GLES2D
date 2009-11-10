#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include <GLES2D/GLES2D.h>


/* Texture */
GLES2D_Font *font;
GLES2D_sprite *sprite1;

int done = 0;

int spriteX = 300, spriteY = 300;

void exitError ( char *errormsg )
{
	printf( "%s", errormsg );

	GLES2D_FreeFont( font ); font = NULL;

	GLES2D_Quit();	
	exit(0);
}

/* function to handle events */
void handleEvents()
{
	GLES2D_HandleEvents( 20 );	

	sprite1->playing = 0;

	if ( GLES2D_KeyboardPressed( XK_Escape ) || GLES2D_PadPressed( START ) )
	{
		done = 1;
	}

	if ( GLES2D_KeyboardHold( XK_Up ) || GLES2D_PadHold( PAD_UP ) )
	{
		sprite1->playing = 1;
		sprite1->frameLayer = 2;

		if ( spriteY > 5 )
			spriteY -= 5;

	}
	if ( GLES2D_KeyboardHold( XK_Down ) || GLES2D_PadHold( PAD_DOWN ) )
	{
		sprite1->playing = 1;
		sprite1->frameLayer = 1;

		if ( spriteY < 480 - sprite1->texture->dst->h )
			spriteY += 5;
	}

	if ( GLES2D_KeyboardHold( XK_Left ) || GLES2D_PadHold( PAD_LEFT ) )
	{
		sprite1->playing = 1;
		sprite1->frameLayer = 3;

		if ( spriteX > 5 )
			spriteX -= 5;
	}
	if ( GLES2D_KeyboardHold( XK_Right ) || GLES2D_PadHold( PAD_RIGHT ) )
	{
		sprite1->playing = 1;
		sprite1->frameLayer = 4;

		if ( spriteX < 800 - sprite1->texture->dst->w )
			spriteX += 5;
	}
}

int main(int argc, char *argv[])
{
	/* Init openGL(ES) display in fullscreen with Full Screen Anti Aliasing */
	if ( ! GLES2D_InitVideo( 800, 480, 0, 1, 0 ) )
		exitError( "Error : GLES2D_InitVideoMode failed\n" );

	/* Load a ttf font in memory */
	font = GLES2D_CreateFont( "../ressources/font2.ttf", TTF_STYLE_NORMAL, 16 );

	/* Check if all data are loaded correctly */
	if ( !font)
		exitError( "Error : data loading failed\n" );

	GLES2D_FpsCounterInit();

	int colorkey[4] = { 255, 0, 255, 255 };
	sprite1 = GLES2D_SpriteInit( "../ressources/clive.png", 23, 49, 3, 4, colorkey, 0 );

	/* functions inclued in "common.c" file */
	//choose_joystick( font );

	/* Main loop */
	while ( ! done )
	{
		/* Handle events ... */
		handleEvents();

		GLES2D_FpsCounterUpdate();

		GLES2D_ClearScreen( 0, 255, 0, 255 );

		GLES2D_SpriteDraw( sprite1, spriteX, spriteY );
			
		GLES2D_DrawFont( font, 5, 460, GLES2D_GetFpsChar() );		

		/* Swap buffers (render to screen) */
		GLES2D_SwapBuffers();
	}

	/* Free all data */
	GLES2D_FreeFont( font ); font = NULL;

	/* Quit properly... */
	GLES2D_Quit();

	return 0;
}

