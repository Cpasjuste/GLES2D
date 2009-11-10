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

#include "GLES2D_particle.h"
SDL_RWops *tmpmem;

GLES2D_Texture *particleTex;

int GLES2D_CreateParticle()
{
	/* Load a particule texture */
	particleTex = (GLES2D_Texture *) malloc( sizeof( GLES2D_Texture ) );

	SDL_Surface *tmp;

	tmpmem = SDL_RWFromMem( particle, PARTICLE_LEN );
	tmp = IMG_LoadPNG_RW( tmpmem );


	particleTex = GLES2D_CreateTextureFromSurface( tmp, 0 );

	SDL_FreeSurface( tmp );

	return 1;
}

