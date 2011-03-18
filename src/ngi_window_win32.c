
#include "ngi/ngi_config.h"

#ifdef NGI_WINDOW_WIN32

#include "ngi/ngi_window_win32.h"

LRESULT CALLBACK WndProc(	HWND	hWnd,					// Handle For This Window
				UINT	uMsg,					// Message For This Window
				WPARAM	wParam,					// Additional Message Information
				LPARAM	lParam)					// Additional Message Information
{
	return DefWindowProc(hWnd,uMsg,wParam,lParam);
}


int ngi_window_init_win32(ngi_application* app, void* winptr) {

	HWND hWnd;

	const char* wndClassName="ngi";
	const char* title = "ngi window";

	DWORD dwExStyle=WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;
    DWORD dwStyle=WS_OVERLAPPEDWINDOW | WS_CLIPSIBLINGS | WS_CLIPCHILDREN;

	int width = 100;
	int height = 100;
	HINSTANCE hInstance = GetModuleHandle(NULL);

	WNDCLASS	wc;

	ngi_window_win32* win = (ngi_window_win32*)winptr;
	win->app = app;


	wc.style		= CS_HREDRAW | CS_VREDRAW | CS_OWNDC;		// Redraw On Move, And Own DC For Window
	wc.lpfnWndProc		= (WNDPROC) WndProc;				// WndProc Handles Messages
	wc.cbClsExtra		= 0;						// No Extra Window Data
	wc.cbWndExtra		= 0;						// No Extra Window Data
	wc.hInstance		= hInstance;					// Set The Instance
	wc.hIcon		= LoadIcon(NULL, IDI_WINLOGO);			// Load The Default Icon
	wc.hCursor		= LoadCursor(NULL, IDC_ARROW);			// Load The Arrow Pointer
	wc.hbrBackground	= NULL;						// No Background Required For GL
	wc.lpszMenuName		= NULL;						// We Don't Want A Menu
	wc.lpszClassName	= wndClassName;					// Set The Class Name


	if (!RegisterClass(&wc))						// Attempt To Register The Window Class
	{
		return 0;
	}

	hWnd=CreateWindowEx(dwExStyle,
					    wndClassName,
					    title,
						dwStyle,
					    0, 0,
					    width, height,
						NULL,			  // parent window
						NULL,			  // menu
						hInstance,		  // instance
						NULL);


	if(!hWnd) {
		win->hwnd = 0;
		return 0;
	}

	win->hwnd = hWnd;

	ShowWindow(hWnd,SW_SHOW);						// Show The Window
	SetForegroundWindow(hWnd);						// Slightly Higher Priority
	SetFocus(hWnd);		

	return 1;
}


void ngi_application_win32_runloop_iteration(ngi_application* app) {
	MSG msg;
	if (GetMessage(&msg,NULL,0,0))			// Is There A Message Waiting?
	{

		if (msg.message==WM_QUIT)				// Have We Received A Quit Message?
		{

		} else							// If Not, Deal With Window Messages
		{

			TranslateMessage(&msg);				// Translate The Message
			DispatchMessage(&msg);				// Dispatch The Message
		}
	}

}



#endif


