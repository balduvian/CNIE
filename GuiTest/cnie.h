#include "stdafx.h"

namespace cnie {
	
	#define MAX_LOADSTRING 100

	extern HINSTANCE hInstance;
	extern WCHAR szTitle[MAX_LOADSTRING];
	extern WCHAR szWindowClass[MAX_LOADSTRING];
	extern HWND base_window;
	extern void(*startup)();

	int setup(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow, void(*startup)());

	ATOM myRegisterClass(HINSTANCE hInstance);
	void InitWindow(int nCmdShow);
	LRESULT CALLBACK wndProc(HWND, UINT, WPARAM, LPARAM);
	INT_PTR CALLBACK about(HWND, UINT, WPARAM, LPARAM);

	HWND createBlankButton(int x, int y, int width, int height, int id);
	HWND createImageButton(int x, int y, int width, int height, int id);
	HWND createTextButton(int x, int y, int width, int height, const wchar_t* text, int id);

	HANDLE loadBitmap(int id);
	HANDLE loadBitmap(int id, int w, int h);

	void setButtonImage(HWND button, HANDLE image);

}