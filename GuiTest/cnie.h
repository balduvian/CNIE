#include "stdafx.h"

namespace cnie {
	
	#define MAX_LOADSTRING 100

	extern HINSTANCE hInstance;
	extern WCHAR szTitle[MAX_LOADSTRING];
	extern WCHAR szWindowClass[MAX_LOADSTRING];
	extern HWND base_window;
	extern void(*startup)();
	extern void(*onResize)();
	extern bool(*onButtonClick)(int id);
	extern WNDPROC oldProc;
	extern HHOOK keyHook;

	extern int winWidth;
	extern int winHeight;

	#define buttonProc(name, func)                                                    \
		LRESULT CALLBACK name(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) { \
			switch (message) {                                                          \
				case WM_LBUTTONUP: {                                                     \
					func();                                                               \
				}                                                                          \
			}                                                                               \
			return CallWindowProc(cnie::oldProc, hwnd, message, wParam, lParam);             \
		}

	class Button;

	typedef LRESULT (CALLBACK *ButtonBack)(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);

	/*
		switch (message) {
				case WM_LBUTTONUP: {
					onClick();
				}
				return CallWindowProc(cnie::oldProc, hwnd, message, wParam, lParam);
			}
	*/

	int setup(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow, void(*startup)(), void(*onResize)(), bool(*onButtonClick)(int));

	ATOM myRegisterClass(HINSTANCE hInstance);
	void InitWindow(int nCmdShow);
	LRESULT CALLBACK wndProc(HWND, UINT, WPARAM, LPARAM);
	LRESULT CALLBACK hookProc(int code, WPARAM wParam, LPARAM lParam);
	INT_PTR CALLBACK about(HWND, UINT, WPARAM, LPARAM);

	void getWindowSizes();

	HWND createBlankButton(int x, int y, int width, int height, int id);
	HWND createImageButton(int x, int y, int width, int height, int id);
	HWND createTextButton(int x, int y, int width, int height, int id, const wchar_t* text);

	HANDLE loadBitmap(int id);
	HANDLE loadBitmap(int id, int w, int h);

	void setButtonImage(HWND button, HANDLE image);
	void setButtonText(HWND button, const wchar_t* text);
	void setButtonSize(HWND button, int x, int y, int width, int height);

	void connectWebcam(HWND capWindow);
	HWND createCaptureWindow(int x, int y, int width, int height);

	void startCapture(HWND capWindow);
	void stopCapture(HWND capWindow);
	void captureFrame(HWND capWindow);
	void getCaptureDims(HWND capWindow, int& width, int& height);
}