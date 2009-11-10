#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include <GLES2D/GLES2D.h>

GLES2D_Texture *background, *smiley, *rgb565, *rgba;
GLES2D_Font *font;

float rot = 0;


/* function to handle events */
void handleEvents()
{
	GLES2D_HandleEvents( 10 );

	if ( GLES2D_KeyboardPressed( XK_space ) )
		printf("space pressed\n");

}

int main( int argc, char *argv[] )
{
	if ( ! GLES2D_InitVideo( 800, 480, 0, 0, 0) )
		exit(0);

	//background = GLES2D_CreateTexture( "../ressources/backg.bmp", 0 );
	smiley = GLES2D_CreateTexture( "../ressources/smiley64.png", 0 );
	rgb565 = GLES2D_CreateTexture( "../ressources/rgb565.bmp", 0 );
	rgba = GLES2D_CreateTexture( "../ressources/rgba.bmp", 0 );

	background = GLES2D_CreateTexture( "../ressources/backg.bmp", 0 );

	font = GLES2D_CreateFont( "../ressources/font2.ttf", TTF_STYLE_NORMAL, 20 );
	GLES2D_SetFontColor( font, 114, 10, 145, 255 );

	GLES2D_FpsCounterInit();

	while(1)
	{
		GLES2D_FpsCounterUpdate();

		handleEvents();

		GLES2D_DrawTextureSimple( background, 0, 0 );
		GLES2D_DrawTextureSimple( rgb565, 0, 0 );
		GLES2D_DrawTextureSimple( rgba, 672, 0 );

		//GLES2D_DrawTextureRotatedCenteredScaled( smiley, 400, 240, 128, 128, rot );

		smiley->src->x = 32;
		smiley->src->y = 32;
		smiley->src->w = 32;
		smiley->src->h = 32;
		smiley->dst->w = 32;	
		smiley->dst->h = 32;

		GLES2D_DrawTextureCentered( smiley, 400, 240 );
		rot += 0.5f;

		GLES2D_DrawFont( font, 10, 10, "aBcdefghijklmnopqrstuvwxyz" );

		GLES2D_DrawFont( font, 10, 440, GLES2D_GetFpsChar() );

		GLES2D_DrawPoint( 790, 470 ); 

		GLES2D_DrawLine( 750, 460, 790, 460 );

		if ( ! GLES2D_SwapBuffers() )
			exit(0);

		//exit(0);
	}

	GLES2D_FreeTexture( background );
	GLES2D_QuitVideo();

	return 0;
}

