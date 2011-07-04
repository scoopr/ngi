
#include "ngi/ngi_config.h"


#ifdef NGI_WINDOW_WIN32



#define VC_EXTRALEAN
#define WIN32_LEAN_AND_MEAN

#include <windows.h>

#include "ngi/ngi.h"

wchar_t* NGI_WINDOW_CLASS_NAME = L"ngi";

#include <stdio.h>

ngi_event_cb current_event_cb;


#define bits(val,from,to) ( ((val) >> (from)) & ( (1 << (to) ) - 1 ) )

LRESULT CALLBACK WndProc(   HWND    hWnd,
                UINT    uMsg,
                WPARAM  wParam,
                LPARAM  lParam)
{

    double timestamp = GetMessageTime() / 1000.0;
    ngi_window *win = GetWindowLongPtr(hWnd, GWLP_USERDATA);

    switch(uMsg) {
        case WM_KEYUP:
        case WM_KEYDOWN:
        {
            int repeats = bits(lParam, 0, 15);
            // int scancode = bits(lParam, 16, 23);
            // int extended = bits(lParam, 24, 24);
            int prevstate = bits(lParam, 30, 30);

            ngi_event ev;
            ev.type = uMsg == WM_KEYDOWN ? ngi_key_down_event : ngi_key_up_event;
            ev.common.timestamp = timestamp;
            ev.key.down = uMsg == WM_KEYDOWN;
            ev.key.keycode = "todo";
            ev.key.modifiers = 0; 
            
            current_event_cb(&ev);
        }
        return 1;
        
        case WM_IME_KEYUP:
        case WM_IME_KEYDOWN:
        printf("WIM_IME_KEY_\n");
        break;

        case WM_IME_CHAR:
        printf("IME");
        case WM_SYSCHAR:
        printf("SYS");
        case WM_UNICHAR:
        printf("UNI");
        case WM_CHAR:
        {
            
            printf("CHAR U+%0X\n",wParam);
//            ngi_event ev;
//            ev.type = ngi_text_event;
//            ev.text.timestamp = timestamp;
//            ev.text.codepoint = wParam;
            
//            current_event_cb(&ev);
            
        }
        return 1;
//        break;
        case WM_IME_STARTCOMPOSITION:
        printf("WM_IME_STARTCOMPOSITION\n");
        break;
        case WM_IME_COMPOSITION:
        printf("WM_IME_COMPOSITION\n");
        {
            wchar_t *buf;
            int bytes = ImmGetCompositionStringW(ImmGetContext(hWnd), GCS_COMPSTR, NULL, 0);
            buf = _alloca(bytes+1);
            memset(buf,0,bytes+1);
            ImmGetCompositionStringW(ImmGetContext(hWnd), GCS_COMPSTR, buf, bytes);


            
        }
        break;
        case WM_IME_ENDCOMPOSITION:
        printf("WM_IME_ENDCOMPOSITION\n");
        break;


        case WM_SIZE:
        {
            RECT r;
            if(GetClientRect(hWnd, &r)) {
                win->width = r.right;
                win->height = r.bottom;
            }
        }


        break;
    }
    
    
    return DefWindowProc(hWnd,uMsg,wParam,lParam);
}


int ngi_window_init_win32(ngi_application* app, ngi_window* win) {

    HWND hWnd;

    wchar_t* title = L"ngi window";

    DWORD dwExStyle=WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;
    DWORD dwStyle=WS_OVERLAPPEDWINDOW | WS_CLIPSIBLINGS | WS_CLIPCHILDREN;

    int width = 100;
    int height = 100;
    HINSTANCE hInstance = GetModuleHandle(NULL);

    RECT r;


    hWnd=CreateWindowExW(dwExStyle,
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
        win->plat.iwnd = 0;
        return 0;
    }

    SetWindowLongPtr(hWnd, GWLP_USERDATA, win);

    win->plat.iwnd = (int)hWnd;

    if(GetClientRect(hWnd, &r)) {
        win->width = r.right;
        win->height = r.bottom;
    }

    ShowWindow(hWnd,SW_SHOW);
    SetForegroundWindow(hWnd);
    SetFocus(hWnd);

    return 1;
}

void ngi_application_init_win32(ngi_application* app) {
    WNDCLASSW    wc;
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


//    UINT oldcp = GetConsoleOutputCP();
    
    SetConsoleOutputCP(CP_UTF8);

    if (!RegisterClassW(&wc))
    {

    }
}

void ngi_application_win32_runloop_iteration(ngi_application* app) {
    MSG msg;
    current_event_cb = app->event_callback;
    if (GetMessageW(&msg,NULL,0,0))
    {

        if (msg.message==WM_QUIT)
        {

        } else
        {

            TranslateMessage(&msg);
            DispatchMessageW(&msg);
        }
    }

}



int ngi_context_wgl_init(ngi_context* ctx, ngi_window* win) {

    HGLRC hRC=NULL;
    HDC hDC=NULL;

    HWND hWnd = (HWND)win->plat.iwnd;
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

    ctx->type = ngi_context_api_wgl;
    ctx->graphics = ngi_graphics_api_opengl;

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

