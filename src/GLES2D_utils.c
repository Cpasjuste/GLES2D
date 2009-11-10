#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <stdio.h>

#include <SDL/SDL.h>
#include "GLES2D/GLES2D_platform.h"

float GLES2D_lastFrame, GLES2D_thisFrame;
int GLES2D_ff, GLES2D_fps;
char GLES2D_fps_char[16];

GDECLSPEC void GLES2D_FpsCounterInit()
{
	GLES2D_lastFrame = SDL_GetTicks();
}

GDECLSPEC void GLES2D_FpsCounterUpdate()
{
	GLES2D_thisFrame=SDL_GetTicks(); 

	GLES2D_ff++;
	if( ( GLES2D_thisFrame - GLES2D_lastFrame ) > 1000 )
	{ 
		GLES2D_fps = GLES2D_ff;
		GLES2D_ff = 0;
		GLES2D_lastFrame = GLES2D_thisFrame;
	}
}


GDECLSPEC int GLES2D_GetFpsInt()
{
	return GLES2D_fps;
}

GDECLSPEC char* GLES2D_GetFpsChar()
{
	sprintf( GLES2D_fps_char, "fps:%i", GLES2D_fps );

	return GLES2D_fps_char;
}

GDECLSPEC int GLES2D_InitDebugConsole()
{
#ifdef _WIN32
	if(AllocConsole())
	{
		freopen("CONOUT$", "wt", stdout);

		SetConsoleTitle(L"GLES2D : Debug Console");

		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_RED);

		printf("n");
		printf("GLES2D debug console started");
		printf("n");
		
		return 1;
	}
	return 0;
#endif
	return 1;
}

