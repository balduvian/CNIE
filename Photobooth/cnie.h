#pragma comment(linker,"\"/manifestdependency:type='win32' \
name='Microsoft.Windows.Common-Controls' version='6.0.0.0' \
processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
#include "stdafx.h"
#include <functional>

namespace cnie {
	
	#define MAX_LOADSTRING 100

	extern HINSTANCE hInstance;
	extern WCHAR szTitle[MAX_LOADSTRING];
	extern WCHAR szWindowClass[MAX_LOADSTRING];
	extern HWND base_window;

	extern std::function<void()> startup;
	extern std::function<void()> onResize;
	extern std::function<bool(int)> onButtonClick;
	extern std::function<void(int)> onKeyPress;
	extern std::function<void(HWND, HDC)> onDraw;

	extern HHOOK keyHook;
	extern int winWidth;
	extern int winHeight;
	extern bool fullscreen;
	extern long defaultStyle;

	int setup(
		HINSTANCE hInstance,
		std::function<void()> stp,
		std::function<void()> onr,
		std::function<bool(int)> obc,
		std::function<void(int)> okp,
		std::function<void(HWND, HDC)> odw
	);

	ATOM registerClass(WNDPROC proc, const WCHAR[]);
	HWND create_window(const wchar_t className[], const wchar_t title[], long& style);
	HWND create_extended_window(const wchar_t className[], const wchar_t title[], long& style);
	LRESULT CALLBACK wndProc(HWND, UINT, WPARAM, LPARAM);
	INT_PTR CALLBACK about(HWND, UINT, WPARAM, LPARAM);

	void getWindowSizes();
	void showWindow(HWND window, bool show);
	void setFullscreen(HWND window, bool full);

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
	HBITMAP captureBitmap(HWND capWindow);
	void captureFrame(HWND capWindow);
	void getCaptureDims(HWND capWindow, int& width, int& height);

	PBITMAPINFO CreateBitmapInfoStruct(HWND hwnd, HBITMAP hBmp);
	void CreateBMPFile(HWND hwnd, LPTSTR pszFile, PBITMAPINFO pbi, HBITMAP hBMP, HDC hDC);

}