#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <stdio.h>

#include <SDL/SDL.h>
#include "GLES2D/GLES2D_platform.h"
#include "GLES2D/GLES2D_texture.h"
#include "GLES2D/GLES2D_video.h"
#include "GLES2D/GLES2D_control.h"
#include "X11/Xlib.h"
#include "X11/Xutil.h"
#include <X11/Xos.h>
#include <X11/Xatom.h>
#include <X11/keysym.h>

#ifdef _PANDORA_
extern void PND_SendKeyEvents( );
#endif

extern int GLES2D_Quit();

extern Display*	x11Display;
XEvent GLES2D_event;

int GLES2D_JoystickNum;

SDL_bool GLES2D_isActive;
SDL_Joystick * GLES2D_joy;
int count, x, i;

int GLES2D_timerset, GLES2D_start, GLES2D_now;
int GLES2D_delay = 0;

GDECLSPEC int GLES2D_GetJoystickNum()
{
	return SDL_NumJoysticks();
}

GDECLSPEC const char *GLES2D_GetJoystickName( int num )
{
	return SDL_JoystickName( num );
}

GDECLSPEC void GLES2D_UseJoystick( int num )
{
	if ( GLES2D_JoystickNum != 255 )
	{
		if( SDL_JoystickOpened( GLES2D_JoystickNum ) )
		{
			SDL_JoystickClose( GLES2D_joy );
		}
		GLES2D_JoystickNum = num;
		GLES2D_joy = SDL_JoystickOpen( GLES2D_JoystickNum );
	}
}

void joyDown(int axis, int value)
{
	if ( axis == 1 )
	{
		//down 
		if (value > 30000)
		{
			if ( GLES2D_Pad[PAD_UP] )
			{
				GLES2D_Pad[PAD_UP] = 0;
			} 
    			else
			{
				GLES2D_Pad[PAD_DOWN] = 1;
				GLES2D_Pad[PAD_UP] = 0;
			}
		}
 		// up
  		else if (value < -30000 )
		{
			if ( GLES2D_Pad[PAD_DOWN] )
			{
				GLES2D_Pad[PAD_DOWN] = 0;
			} 
    			else
			{
				GLES2D_Pad[PAD_UP] = 1;
				GLES2D_Pad[PAD_DOWN] = 0;
			}
		}
		else
		{
			GLES2D_Pad[PAD_UP] = 0;
			GLES2D_Pad[PAD_DOWN] = 0;
		}
	}
	
	if ( axis == 0 )
	{
		//	Right 
		if (value > 30000)
		{
			if ( GLES2D_Pad[PAD_LEFT] )
			{
				GLES2D_Pad[PAD_LEFT] = 0;
			} 
    			else
			{
				GLES2D_Pad[PAD_RIGHT] = 1;
				GLES2D_Pad[PAD_LEFT] = 0;
			}
		}
 		// Left
  		else if (value < -30000 )
		{
			if ( GLES2D_Pad[PAD_RIGHT] )
			{
				GLES2D_Pad[PAD_RIGHT] = 0;
			} 
    			else
			{
				GLES2D_Pad[PAD_LEFT] = 1;
				GLES2D_Pad[PAD_RIGHT] = 0;
			}
		}
		else
		{
			GLES2D_Pad[PAD_LEFT] = 0;
			GLES2D_Pad[PAD_RIGHT] = 0;
		}
	}
}


GDECLSPEC int GLES2D_PadPressed ( int num )
{
#ifdef _PANDORA_
	static int GLES2D_Pad_old[12];

	if ( !GLES2D_Pad_old[num] && GLES2D_Pad[num] )
	{
		GLES2D_Pad_old[num] = GLES2D_Pad[num];
		return 1;
	}
	
	GLES2D_Pad_old[num] = GLES2D_Pad[num];
#endif
	return 0;

}

GDECLSPEC int GLES2D_PadHold ( int num )
{
#ifdef _PANDORA_
	if ( ( GLES2D_now - GLES2D_start ) >= GLES2D_delay )
	{
		GLES2D_timerset = 0;
	}

	if ( ! GLES2D_timerset )
	{
		if ( GLES2D_Pad[num] ) return 1;
	}
#endif
	return 0;
}


GDECLSPEC int GLES2D_KeyboardPressed ( KeySym key )
{
#ifdef _X11_
	static unsigned int GLES2D_Keyboard_old[256];

	if ( !GLES2D_Keyboard_old[XKeysymToKeycode(x11Display, key)] && GLES2D_Keyboard[XKeysymToKeycode(x11Display, key)] )
	{
		GLES2D_Keyboard_old[XKeysymToKeycode(x11Display, key)] = GLES2D_Keyboard[XKeysymToKeycode(x11Display, key)];
		return 1;
	}
	
	GLES2D_Keyboard_old[XKeysymToKeycode(x11Display, key)] = GLES2D_Keyboard[XKeysymToKeycode(x11Display, key)];
#endif
	return 0;
}

GDECLSPEC int GLES2D_KeyboardHold ( KeySym key )
{
#ifdef _X11_
	if ( ( GLES2D_now - GLES2D_start ) >= GLES2D_delay )
	{
		GLES2D_timerset = 0;
	}

	if ( ! GLES2D_timerset )
	{
		if ( GLES2D_Keyboard[XKeysymToKeycode(x11Display, key)] ) return 1;		
	}
#endif
	return 0;
}


GDECLSPEC void GLES2D_HandleEvents( int delay )
{

	GLES2D_delay = delay;

	if ( ! GLES2D_timerset )
	{
		GLES2D_timerset = 1;
		GLES2D_start = SDL_GetTicks();
	}
	GLES2D_now = SDL_GetTicks();

#ifdef _PANDORA_
 	/* Pandora Joystick input */
	PND_SendKeyEvents( );
#else
/*
	if ( GLES2D_JoystickNum != 255 )
	{
		SDL_JoystickUpdate();

		joyDown( 0, SDL_JoystickGetAxis( GLES2D_joy, 0 ) );
		joyDown( 1, SDL_JoystickGetAxis( GLES2D_joy, 1 ) );

		for ( i = 0; i < SDL_JoystickNumButtons(GLES2D_joy); i++ )
		{
			GLES2D_Pad[i] = SDL_JoystickGetButton(GLES2D_joy, i);
		}
	}
	else
	{
		for ( i = 0; i < 12; i++ )
		{
			GLES2D_Pad[i] = 0;
		}
	}
*/
#endif

#ifdef _X11_

	if ( XPending( x11Display ) )
	{
		XNextEvent( x11Display, &GLES2D_event );

		switch( GLES2D_event.type )
		{
			// Exit on mouse click
			case ButtonPress:
				GLES2D_Quit();
				exit(0);
			break;

			case KeyPress:
	  			GLES2D_Keyboard[ XKeysymToKeycode(x11Display, (XLookupKeysym( &GLES2D_event.xkey, 0 ))) ] = 1;
			break;

			case KeyRelease:
				GLES2D_Keyboard[ XKeysymToKeycode(x11Display, (XLookupKeysym( &GLES2D_event.xkey, 0 ))) ] = 0;
			break;


			default:
			break;
		}
	}
#endif
}

