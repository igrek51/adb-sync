#include "app.h"

using namespace std;

LRESULT CALLBACK windowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

LRESULT CALLBACK wndproc_new(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam){
	return App::geti()->subclass_wndproc_new(hwnd, message, wParam, lParam);
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow){
	App::geti()->hInst = &hInstance;
    App::geti()->windowProc = windowProc;
    App::geti()->wndproc_new = wndproc_new;
	WNDCLASS windowClass;
	windowClass.lpfnWndProc = windowProc;
	windowClass.style       = CS_HREDRAW | CS_VREDRAW;
	windowClass.hInstance   = hInstance;
	windowClass.hCursor     = LoadCursor(NULL, IDC_ARROW);
	windowClass.hIcon       = LoadIcon(NULL, IDI_APPLICATION);
	windowClass.hbrBackground=(HBRUSH)COLOR_BTNSHADOW;
	windowClass.cbClsExtra  = 0;
	windowClass.cbWndExtra  = 0;
	windowClass.lpszClassName = Config::geti()->program_name.c_str();
	windowClass.lpszMenuName  = NULL;
	if(!RegisterClass(&windowClass)){
		IO::geti()->critical_error("RegisterClass failed!");
		return 0;
	}
	HWND window = CreateWindowEx(0, Config::geti()->program_name.c_str(), Config::geti()->program_name.c_str(), WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, Config::geti()->window_w+16, Config::geti()->window_h+38, NULL, NULL, hInstance, NULL);
	if(!window){
		IO::geti()->critical_error("window NULL pointer!");
		return 0;
	}
	ShowWindow(window,nCmdShow);
	UpdateWindow(window);
	MSG message;
	while(GetMessage(&message, NULL, 0, 0)){
		TranslateMessage(&message);
		DispatchMessage(&message);
	}
	return message.wParam;
}

LRESULT CALLBACK windowProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam){
	switch(message){
        case WM_CREATE:{
            App::geti()->event_init(&hwnd);
        }break;
        case WM_COMMAND:{
            App::geti()->event_button(wParam, lParam);
        }break;
        case WM_SIZE:{
            App::geti()->event_resize();
        }break;
        case WM_MOVE:{
            App::geti()->event_move();
        }break;
        case WM_DESTROY:{
            delete App::geti();
            return 0;
        }break;
	}
	return DefWindowProc(hwnd, message, wParam, lParam);
}
