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

	extern HHOOK keyHook;
	extern int winWidth;
	extern int winHeight;
	extern bool fullscreen;
	extern long defaultStyle;

	// camera painting stuff
	extern HDC camPaint;
	extern HDC camPaintMem;
	extern RECT camPaintRect;
	extern PAINTSTRUCT camPaintStruct;
	extern HBITMAP camPaintBMP;
	// ^^^^^^^^^^^^^^^^^^^^^

	int setup(
		HINSTANCE hInstance,
		HINSTANCE hPrevInstance,
		LPWSTR lpCmdLine,
		int nCmdShow, 
		std::function<void()> stp,
		std::function<void()> onr,
		std::function<bool(int)> obc,
		std::function<void(int)> okp
	);

	ATOM registerClass(WNDPROC proc, const WCHAR[]);
	void InitWindow(int nCmdShow);
	LRESULT CALLBACK wndProc(HWND, UINT, WPARAM, LPARAM);
	LRESULT CALLBACK hookProc(int code, WPARAM wParam, LPARAM lParam);
	INT_PTR CALLBACK about(HWND, UINT, WPARAM, LPARAM);

	void getWindowSizes();
	void setFullscreen(bool full);

	HWND createBlankButton(int x, int y, int width, int height, int id);
	HWND createImageButton(int x, int y, int width, int height, int id);
	HWND createTextButton(int x, int y, int width, int height, int id, const wchar_t* text);

	HWND createSubWindow(const WCHAR wClass[], int x, int y, int width, int height, int id, const wchar_t* name, int addStyles = 0);

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