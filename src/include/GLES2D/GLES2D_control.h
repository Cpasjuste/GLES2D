#ifndef GLES2D_CONTROL_H
#define GLES2D_CONTROL_H

#include <SDL/SDL.h>
#include "GLES2D/GLES2D_platform.h"
#include <GLES2D/GLES2D_texture.h>
#include "X11/Xlib.h"
#include "X11/Xutil.h"
#include <X11/Xos.h>
#include <X11/Xatom.h>
#include <X11/keysym.h>

GDECLSPEC KeySym GLES2D_XKeycodeToKeysym ( int Keycode );

#define XK_PND GLES2D_XKeycodeToKeysym(147)

/* Logitech Precision GamePad keycodes */
#define PAD_UP 100
#define PAD_DOWN 101
#define PAD_LEFT  102
#define PAD_RIGHT 103
#define A 0
#define B  3
#define X 1
#define Y  2
#define L 4
#define R  5
#define START 9
#define SELECT 8
#define MENU 10

enum
{
	MOUSE_LEFT = 1,
	MOUSE_RIGHT,
	MOUSE_MIDDLE
};

/**
  * Mouse coordinates.
  */
GDECLSPEC int MOUSE_X;
GDECLSPEC int MOUSE_Y;

/**
  * Keyboard keys (internal usage, should not be called directly)
  */
GDECLSPEC unsigned int GLES2D_Keyboard[256];

/**
  * Pad buttons (internal usage, should not be called directly)
  */
GDECLSPEC int GLES2D_Pad[12];


GDECLSPEC int GLES2D_GetJoystickNum();

GDECLSPEC const char *GLES2D_GetJoystickName( int num );

GDECLSPEC void GLES2D_UseJoystick( int num );

/**
  * Update keyboard/joypad with a delay (in ms)
  */
GDECLSPEC void GLES2D_HandleEvents( int delay );

GDECLSPEC int GLES2D_PadPressed ( int num );

GDECLSPEC int GLES2D_PadHold ( int num );

GDECLSPEC int GLES2D_KeyboardPressed ( KeySym key );

GDECLSPEC int GLES2D_KeyboardHold (  KeySym key );

/* Mouse */
GDECLSPEC int GLES2D_GetMouseNum();

GDECLSPEC char *GLES2D_GetMouseName( int num );

GDECLSPEC void GLES2D_UseMouse( int num );

GDECLSPEC int GLES2D_GetMouseClick( int button );

GDECLSPEC void GLES2D_GetMouseLoc( int *x, int *y );

GDECLSPEC void GLES2D_SetMouseLoc( int x, int y );

GDECLSPEC int GLES2D_MouseIsOver( int x, int y, int w, int h );

GDECLSPEC int GLES2D_MouseIsOverTexture( GLES2D_Texture *texture_structure );

#endif

