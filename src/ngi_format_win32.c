

#ifdef NGI_CONTEXT_WGL
#include "ngi/ngi.h"

#include <assert.h>

#define WIN32_LEAN_AND_MEAN
#include <windows.h>


int ngi_format_init_win32(ngi_application* app, ngi_format* format, ngi_config *config)
{


	// We don't have a window yet, so no device context for it. 
	// NULL device context is for "whole screen".
	// Hopefully this device context is representative enough.
	HDC dc;
	unsigned int pixelformat;

	PIXELFORMATDESCRIPTOR pfd = { 
	    sizeof(PIXELFORMATDESCRIPTOR),  //  size of this pfd  
		1,                     // version number  
	    PFD_DRAW_TO_WINDOW |   // support window  
		PFD_SUPPORT_OPENGL |   // support OpenGL  
		PFD_DOUBLEBUFFER,      // double buffered  
		PFD_TYPE_RGBA,         // RGBA type  
		24,                    // color depth  
		0, 0, 0, 0, 0, 0,      // color bits ignored  
		0,                     // alpha buffer  
		0,                     // shift bit ignored  
		0,                     // accumulation buffer  
		0, 0, 0, 0,            // accum bits ignored  
		32,                    // z-buffer      
		0,                     // stencil buffer  
		0,                     // auxiliary buffer  
		PFD_MAIN_PLANE,        // main layer  
		0,                     // reserved  
		0, 0, 0                // layer masks ignored  
	}; 

	dc = GetDC(NULL);
	if(dc == NULL) return 0;

	pixelformat = ChoosePixelFormat(dc, &pfd);

	ReleaseDC(NULL, dc);

	if(pixelformat == 0) return 0;

	format->platform.wgl.pixelformat = pixelformat;

	assert( sizeof(PIXELFORMATDESCRIPTOR) == sizeof(format->platform.wgl.pfd) );
	memcpy(format->platform.wgl.pfd, &pfd, sizeof(PIXELFORMATDESCRIPTOR));


	return 1;
    
}

#endif
