
#include "ngi/ngi_config.h"


#ifdef NGI_WINDOW_WIN32



#define VC_EXTRALEAN
#define WIN32_LEAN_AND_MEAN

#include <windows.h>


#include "ngi/ngi_window.h"

const char* NGI_WINDOW_CLASS_NAME="ngi";

LRESULT CALLBACK WndProc(   HWND    hWnd,
                UINT    uMsg,
                WPARAM  wParam,
                LPARAM  lParam)
{
    return DefWindowProc(hWnd,uMsg,wParam,lParam);
}


int ngi_window_init_win32(ngi_application* app, void* winptr) {

    HWND hWnd;

    const char* title = "ngi window";

    DWORD dwExStyle=WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;
    DWORD dwStyle=WS_OVERLAPPEDWINDOW | WS_CLIPSIBLINGS | WS_CLIPCHILDREN;

    int width = 100;
    int height = 100;
    HINSTANCE hInstance = GetModuleHandle(NULL);


    ngi_window_win32* win = (ngi_window_win32*)winptr;
    win->app = app;



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
        win->hwnd = 0;
        return 0;
    }

    win->hwnd = hWnd;

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



#endif

static inline void dummy() {}


