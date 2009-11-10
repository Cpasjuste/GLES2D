#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>

#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <GLES/gl.h>
#include <GLES/egl.h>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_ttf.h>
#include <GLES2D/GLES2D_platform.h>

#ifdef _PANDORA_
#include <linux/matroxfb.h>
#include <linux/fb.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <unistd.h>
static int vsync_on = 0;
extern void PND_Setup_Controls( void );
extern void PND_Close_Controls( void );
#else
extern int GLES2D_JoystickNum;
extern SDL_Joystick * GLES2D_joy;
#endif

int GLES2D_Quit();
int GLES2D_InitVideoRaw( int w, int h, int fullscreen, int vsync, int fsaa );
int GLES2D_InitVideoX11( int w, int h, int fullscreen, int vsync, int fsaa );
extern int GLES2D_timerset, GLES2D_start, GLES2D_now;
extern int GLES2D_CreateParticle();

// EGL variables
static EGLDisplay	GLES2D_eglDisplay	= 0;
static EGLConfig	GLES2D_eglConfig	= 0;
static EGLSurface	GLES2D_eglSurface	= 0;
static EGLContext	GLES2D_eglContext	= 0;

// X11 variables
static Window		x11Window	= 0;
Display*		x11Display	= 0;
static long		x11Screen	= 0;
static XVisualInfo*	x11Visual	= 0;
static Colormap		x11Colormap	= 0;

// Store Screen width
static int		GLES2D_SCRw	= 0;
static int		GLES2D_SCRh	= 0;


int GLES2D_TestEGLError( char* pszLocation )
{
	EGLint iErr = eglGetError();
	if (iErr != EGL_SUCCESS)
	{
		printf("%s failed (%d).\n", pszLocation, iErr);
		return 0;
	}

	return 1;
}

GDECLSPEC void GLES2D_Enable2D()
{
	glEnable( GL_TEXTURE_2D );
	glViewport( 0, 0, GLES2D_SCRw, GLES2D_SCRh );
	glMatrixMode( GL_PROJECTION );
	glLoadIdentity();
	glOrthof(0.0f, GLES2D_SCRw, GLES2D_SCRh, 0.0f, -1.0f, 1.0f);
	glMatrixMode( GL_MODELVIEW );
	glLoadIdentity();
}

GDECLSPEC void GLES2D_Disable2D()
{
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();
	glEnable(GL_DEPTH_TEST);
}

GDECLSPEC int GLES2D_InitVideo( int w, int h, int fullscreen, int vsync, int fsaa )
{
	GLES2D_SCRw = w;
	GLES2D_SCRh = h;

	if ( SDL_Init(SDL_INIT_VIDEO) != 0 )
	{
		printf("Unable to initialize SDL: %s\n", SDL_GetError());
		return 1;
	}

#ifdef _X11_
	if ( ! GLES2D_InitVideoX11( w, h, fullscreen, vsync, fsaa ) )
		return 0;
#else
	if ( ! GLES2D_InitVideoRaw( w, h, fullscreen, vsync, fsaa ) )
		return 0;
#endif

	GLES2D_Enable2D();

	GLES2D_timerset = 0;
	GLES2D_start = 0;
	GLES2D_now = 0;

	if ( TTF_Init() < 0 )
	{
		printf( "ERROR Initializing TTF Engine: %s\n", TTF_GetError() );
		return 0;
	}

	GLES2D_CreateParticle();


#ifdef _X11_
/*
	if ( SDL_Init( SDL_INIT_JOYSTICK ) == -1 )
	{
		printf("Error : GLES2D_InitVideoMode @ SDL_Init( SDL_INIT_JOYSTICK ): %s\n", SDL_GetError());
	}
	if ( SDL_NumJoysticks() > 0 )
	{
		GLES2D_JoystickNum = 0;
		GLES2D_joy = SDL_JoystickOpen( GLES2D_JoystickNum );
	}
	else
	{
		GLES2D_JoystickNum = 255;
	}
*/
#else
	PND_Setup_Controls();
#endif
	return 1;
}

GDECLSPEC int GLES2D_InitVideoRaw( int w, int h, int fullscreen, int vsync, int fsaa )
{
	GLES2D_eglDisplay = eglGetDisplay((NativeDisplayType)0);

	EGLint iMajorVersion, iMinorVersion;
	if (!eglInitialize(GLES2D_eglDisplay, &iMajorVersion, &iMinorVersion))
	{
		printf("Error: eglInitialize() failed.\n");
		GLES2D_Quit();
		return 0;
	}

	EGLint pi32ConfigAttribs[5];
	int attrib = 0;
	pi32ConfigAttribs[attrib++] = EGL_SURFACE_TYPE;
	pi32ConfigAttribs[attrib++] = EGL_WINDOW_BIT;
	pi32ConfigAttribs[attrib++] = EGL_NONE;
	if ( fsaa )
	{
		pi32ConfigAttribs[attrib++] = EGL_SAMPLE_BUFFERS, 1;
		pi32ConfigAttribs[attrib++] = EGL_SAMPLES, 4;
	}
	pi32ConfigAttribs[attrib++] = EGL_NONE;

	int iConfigs;
	if (!eglChooseConfig(GLES2D_eglDisplay, pi32ConfigAttribs, &GLES2D_eglConfig, 1, &iConfigs) || (iConfigs != 1))
	{
		printf("Error: eglChooseConfig() failed.\n");
		GLES2D_Quit();
		exit(0);
	}

	GLES2D_eglSurface = eglCreateWindowSurface(GLES2D_eglDisplay, GLES2D_eglConfig, (NativeWindowType)0, NULL);
	if (!GLES2D_TestEGLError("eglCreateWindowSurface"))
	{
		GLES2D_Quit();
		exit(0);
	}

	GLES2D_eglContext = eglCreateContext(GLES2D_eglDisplay, GLES2D_eglConfig, NULL, NULL);
	if (!GLES2D_TestEGLError("eglCreateContext"))
	{
		GLES2D_Quit();
		exit(0);
	}

	eglMakeCurrent(GLES2D_eglDisplay, GLES2D_eglSurface, GLES2D_eglSurface, GLES2D_eglContext);
	if (!GLES2D_TestEGLError("eglMakeCurrent"))
	{
		GLES2D_Quit();
		exit(0);
	}

	if ( vsync )
	{
#ifdef _PANDORA_
		vsync_on = 1;
#else
		eglSwapInterval( GLES2D_eglDisplay, 1 );
#endif
	}


	return 1;
}

GDECLSPEC int GLES2D_InitVideoX11( int w, int h, int fullscreen, int vsync, int fsaa )
{
	Window			sRootWindow;
	XSetWindowAttributes	sWA;
	unsigned int		ui32Mask;
	int			i32Depth;

	x11Display = XOpenDisplay( ":0" );
	if (!x11Display)
	{
		printf("Error: Unable to open X display\n");
		GLES2D_Quit();
		return 0;
	}
	x11Screen = XDefaultScreen( x11Display );

	sRootWindow	= RootWindow(x11Display, x11Screen);
	i32Depth	= DefaultDepth(x11Display, x11Screen);
	x11Visual	= malloc(sizeof(XVisualInfo));
	XMatchVisualInfo( x11Display, x11Screen, i32Depth, TrueColor, x11Visual);
	if (!x11Visual)
	{
		printf("Error: Unable to acquire visual\n");
		GLES2D_Quit();
		exit(0);
	}

	// Colormap of the specified visual type for the display.
	x11Colormap = XCreateColormap( x11Display, sRootWindow, x11Visual->visual, AllocNone );
	sWA.colormap = x11Colormap;

	// List of events to be handled by the application. Add to these for handling other events.
	sWA.event_mask = StructureNotifyMask | ExposureMask | ButtonPressMask | ButtonReleaseMask | KeyPressMask | KeyReleaseMask;

	// Display capabilities list.
   	ui32Mask = CWBackPixel | CWBorderPixel | CWEventMask | CWColormap;

	// Creates the X11 window
	x11Window = XCreateWindow( x11Display, RootWindow(x11Display, x11Screen), 0, 0, w, h,
								 0, CopyFromParent, InputOutput, CopyFromParent, ui32Mask, &sWA);

	// Make the window viewable and flush the output buffer.
	XMapWindow(x11Display, x11Window);
	XFlush(x11Display);

	GLES2D_eglDisplay = eglGetDisplay((NativeDisplayType)x11Display);

	EGLint iMajorVersion, iMinorVersion;
	if (!eglInitialize(GLES2D_eglDisplay, &iMajorVersion, &iMinorVersion))
	{
		printf("Error: eglInitialize() failed.\n");
		GLES2D_Quit();
		return 0;
	}

	EGLint pi32ConfigAttribs[5];
	int attrib = 0;
	pi32ConfigAttribs[attrib++] = EGL_SURFACE_TYPE;
	pi32ConfigAttribs[attrib++] = EGL_WINDOW_BIT;
	pi32ConfigAttribs[attrib++] = EGL_NONE;
	if ( fsaa )
	{
		pi32ConfigAttribs[attrib++] = EGL_SAMPLE_BUFFERS, 1;
		pi32ConfigAttribs[attrib++] = EGL_SAMPLES, 4;
	}
	pi32ConfigAttribs[attrib++] = EGL_NONE;


	int iConfigs;
	if (!eglChooseConfig(GLES2D_eglDisplay, pi32ConfigAttribs, &GLES2D_eglConfig, 1, &iConfigs) || (iConfigs != 1))
	{
		printf("Error: eglChooseConfig() failed.\n");
		GLES2D_Quit();
		return 0;
	}

	GLES2D_eglSurface = eglCreateWindowSurface(GLES2D_eglDisplay, GLES2D_eglConfig, (NativeWindowType)x11Window, NULL);
	if (!GLES2D_TestEGLError("eglCreateWindowSurface"))
	{
		GLES2D_Quit();
		return 0;
	}

	GLES2D_eglContext = eglCreateContext(GLES2D_eglDisplay, GLES2D_eglConfig, NULL, NULL);
	if (!GLES2D_TestEGLError("eglCreateContext"))
	{
		GLES2D_Quit();
		return 0;
	}

	eglMakeCurrent(GLES2D_eglDisplay, GLES2D_eglSurface, GLES2D_eglSurface, GLES2D_eglContext);
	if (!GLES2D_TestEGLError("eglMakeCurrent"))
	{
		GLES2D_Quit();
		return 0;
	}

	if ( vsync )
	{
#ifdef _PANDORA_
		vsync_on = 1;
#else
		eglSwapInterval( GLES2D_eglDisplay, 1 );
#endif
	}

	return 1;
}


GDECLSPEC int GLES2D_SwapBuffers()
{
#ifdef _PANDORA_
	if ( vsync_on )
	{
		int fd = open( "/dev/fb0" , O_RDWR );
		if( 0 < fd )
		{
			int ret = 0;
			ret = ioctl(fd, FBIO_WAITFORVSYNC, &ret );
			if ( ret != 0 ) printf("FBIO_WAITFORVSYNC failed: %d\n", ret );
		}
		close(fd);
	}
#endif

	eglSwapBuffers( GLES2D_eglDisplay, GLES2D_eglSurface );
	if ( !GLES2D_TestEGLError( "eglSwapBuffers" ) )
	{
		return 0;
	}
	return 1;
}


GDECLSPEC int GLES2D_Quit()
{
	eglMakeCurrent( GLES2D_eglDisplay, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT );
	eglDestroyContext ( GLES2D_eglDisplay, GLES2D_eglContext );
	eglDestroySurface ( GLES2D_eglDisplay, GLES2D_eglSurface );
	eglTerminate ( GLES2D_eglDisplay );

#ifdef _X11_
	if (x11Window) XDestroyWindow(x11Display, x11Window);
	if (x11Colormap) XFreeColormap( x11Display, x11Colormap );
	if (x11Display) XCloseDisplay(x11Display);

/*
	if ( GLES2D_JoystickNum != 255 )
	{
		if( SDL_JoystickOpened( GLES2D_JoystickNum ) )
		{
			printf("GLES2D_Quit: SDL_JoystickClose()\n");
                	SDL_JoystickClose( GLES2D_joy );
		}
	}
*/
#else
	PND_Close_Controls( );
#endif
	TTF_Quit();
	SDL_Quit();

	return 1;
}



