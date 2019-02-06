#include "stdafx.h"

namespace cnie {
	
	#define MAX_LOADSTRING 100

	extern HINSTANCE hInstance;
	extern WCHAR szTitle[MAX_LOADSTRING];
	extern WCHAR szWindowClass[MAX_LOADSTRING];
	extern HWND base_window;
	extern void(*startup)();
	extern void(*onResize)();
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

	typedef LRESULT (CALLBACK *buttonBack)(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);

	int setup(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow, void(*startup)(), void(*onResize)());

	ATOM myRegisterClass(HINSTANCE hInstance);
	void InitWindow(int nCmdShow);
	LRESULT CALLBACK wndProc(HWND, UINT, WPARAM, LPARAM);
	LRESULT CALLBACK hookProc(int code, WPARAM wParam, LPARAM lParam);
	INT_PTR CALLBACK about(HWND, UINT, WPARAM, LPARAM);

	void getWindowSizes();

	HWND createBlankButton(int x, int y, int width, int height);
	HWND createImageButton(int x, int y, int width, int height);
	HWND createTextButton(int x, int y, int width, int height, const wchar_t* text);

	HANDLE loadBitmap(int id);
	HANDLE loadBitmap(int id, int w, int h);

	void registerProc(HWND button, buttonBack back);

	void setButtonImage(HWND button, HANDLE image);
	void setButtonText(HWND button, const wchar_t* text);
	void setButtonSize(HWND button, int x, int y, int width, int height);

	void connectWebcam(HWND capWindow);
	HWND createCaptureWindow(int x, int y, int width, int height);

	void startCapture(HWND capWindow);
	void stopCapture(HWND capWindow);
	void captureFrame(HWND capWindow);

}