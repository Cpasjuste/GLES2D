#ifndef __GLES2D_VIDEO_H__
#define __GLES2D_VIDEO_H__

#define FULLSCREEN 1

/**
  * Init GLES 1.1 video driver and screen.
  * Note that it doesn't matter if X11 is running or not,
  * this function automatically choose X11 mode or FB mode.
  *
  * @param w : 		The screen width.
  *
  * @param h : 		The screen height.
  *
  * @param fullscreen :	If 1 or FULLSCREEN is passed, start the application
  *			in fullscreen. Only usable while X is running.
  *
  * @param vsync : 	If 1 is passed, enable vertical synch. (do not work yet on the pandora)
  *
  * @param fsaa : 	If 1 is passed, enable 4x full scene anti aliasing.
  *
  * @return 1 on success, 0 on error.
  */
GDECLSPEC int GLES2D_InitVideo( int w, int h, int fullscreen, int vsync, int fsaa );

GDECLSPEC int GLES2D_SwapBuffers();

#endif



