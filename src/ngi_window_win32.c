
#include "ngi/ngi_config.h"


#ifdef NGI_WINDOW_WIN32



#define VC_EXTRALEAN
#define WIN32_LEAN_AND_MEAN

#include <windows.h>


#include "ngi/ngi_window.h"
#include "ngi/ngi_context.h"

const char* NGI_WINDOW_CLASS_NAME="ngi";

LRESULT CALLBACK WndProc(   HWND    hWnd,
                UINT    uMsg,
                WPARAM  wParam,
                LPARAM  lParam)
{
    return DefWindowProc(hWnd,uMsg,wParam,lParam);
}


int ngi_window_init_win32(ngi_application* app, ngi_window* win) {

    HWND hWnd;

    const char* title = "ngi window";

    DWORD dwExStyle=WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;
    DWORD dwStyle=WS_OVERLAPPEDWINDOW | WS_CLIPSIBLINGS | WS_CLIPCHILDREN;

    int width = 100;
    int height = 100;
    HINSTANCE hInstance = GetModuleHandle(NULL);



    hWnd=CreateWindowEx(dwExStyle,
                        NGI_WINDOW_CLASS_NAME,
                        title,
                        dwStyle,
                        0, 0,
                        width, height,
                        NULL,
                        NULL,
                        hInstance,
                        NULL);


    if(!hWnd) {
        win->platform.iwnd = 0;
        return 0;
    }

    win->platform.iwnd = (int)hWnd;

    ShowWindow(hWnd,SW_SHOW);
    SetForegroundWindow(hWnd);
    SetFocus(hWnd);

    return 1;
}

void ngi_application_init_win32(ngi_application* app) {
    WNDCLASS    wc;
    HINSTANCE hInstance = GetModuleHandle(NULL);

    wc.style        = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
    wc.lpfnWndProc      = (WNDPROC) WndProc;
    wc.cbClsExtra       = 0;
    wc.cbWndExtra       = 0;
    wc.hInstance        = hInstance;
    wc.hIcon        = LoadIcon(NULL, IDI_WINLOGO);
    wc.hCursor      = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground    = NULL;
    wc.lpszMenuName     = NULL;
    wc.lpszClassName    = NGI_WINDOW_CLASS_NAME;


    if (!RegisterClass(&wc))
    {

    }
}

void ngi_application_win32_runloop_iteration(ngi_application* app) {
    MSG msg;
    if (GetMessage(&msg,NULL,0,0))
    {

        if (msg.message==WM_QUIT)
        {

        } else
        {

            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

}



int ngi_context_wgl_init(ngi_context* ctx, ngi_window* win) {

    HGLRC hRC=NULL;
    HDC hDC=NULL;

    HWND hWnd = (HWND)win->platform.iwnd;
    unsigned int PixelFormat;

    static  PIXELFORMATDESCRIPTOR pfd=
    {
        sizeof(PIXELFORMATDESCRIPTOR),
        1,
        PFD_DRAW_TO_WINDOW |
        PFD_SUPPORT_OPENGL |
        PFD_DOUBLEBUFFER,
        PFD_TYPE_RGBA,
        32,
        0, 0, 0, 0, 0, 0,
        0,
        0,
        0,
        0, 0, 0, 0,
        16,
        0,
        0,
        PFD_MAIN_PLANE,
        0,
        0, 0, 0
    };


    if (!(hDC=GetDC(hWnd))) {
        return 0;
    }

    if (!(PixelFormat=ChoosePixelFormat(hDC,&pfd)))
    {
        return 0;
    }

    if(!SetPixelFormat(hDC,PixelFormat,&pfd)) {
        return 0;
    }

    if (!(hRC=wglCreateContext(hDC)))
    {
		return 0;
    }
    
    if(!wglMakeCurrent(hDC,hRC))
	{
		return 0;
	}
    ctx->platform.wgl.hdc = hDC;
    return 1;
}

int ngi_context_wgl_swap(ngi_context* ctx) {
    SwapBuffers( ctx->platform.wgl.hdc );
    return 1;
}




#else

inline static void dummy() {}
#endif

