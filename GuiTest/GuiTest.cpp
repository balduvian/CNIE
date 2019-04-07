#include "stdafx.h"
#include "cnie.h"
#include <string>
#include <iostream>
#include <chrono>
#include <thread>

#define HOWDY_BUTTON 100
#define PHOTOBOOTH_FRAMEBUFFER 101
#define SETTINGS_WINDOW 102

const WCHAR settingsWindowClass[MAX_LOADSTRING] = L"SettingsWindow";

int numImages = 4;
HBITMAP* photobooth_images;
int stripBorder = 10;

HWND preview1;
HWND button1;
HWND settings_window;

int switcher = 0;
int previewWidth = 0;
int previewHeight = 0;

bool fullscreen = false;

void guiResize() {
	cnie::setButtonSize(button1, 10, 10, cnie::winWidth - 20, 100);
	//cnie::setButtonSize(preview1, 10, 120, cnie::winWidth - 20, cnie::winHeight - 130);

	/*
	 * calculate the size of the preview window based on the aspect ratio of the camera
	 */
	int offsetX = 0;
	int offsetY = 0;
	int finalWidth = 0;
	int finalHeight = 0;

	int maxWidth = cnie::winWidth - 20;
	int maxHeight = cnie::winHeight - 130;

	float camAspect = (float)previewWidth / previewHeight;

	float areaAspect = (float)maxWidth / maxHeight;

	if (areaAspect > camAspect) {
		finalHeight = maxHeight;
		
		finalWidth = (int)(maxHeight * camAspect);

		offsetX = (maxWidth - finalWidth) / 2;
	}
	else {
		finalWidth = maxWidth;

		finalHeight = (int)(maxWidth / camAspect);

		offsetY = (maxHeight - finalHeight) / 2;
	}

	cnie::setButtonSize(preview1, 10 + offsetX, 120 + offsetY, finalWidth, finalHeight);
}

void guiConnectCamera() {
	cnie::connectWebcam(preview1);
}

void guiCaptureStart() {
	cnie::startCapture(preview1);
	cnie::getCaptureDims(preview1, previewWidth, previewHeight);
	guiResize();
}

void drawStrip(int wd, int hgt, HBITMAP* images, HBITMAP& bmp, HDC& hdcScreen) {
	// Get a device context to the screen.
	hdcScreen = GetDC(NULL);

	// Create a device context
	HDC hdcBmp = CreateCompatibleDC(hdcScreen);

	// Create a bitmap and attach it to the device context we created above...
	bmp = CreateCompatibleBitmap(hdcScreen, wd, hgt);
	HBITMAP hbmOld = static_cast<HBITMAP>(SelectObject(hdcBmp, bmp));

	/*
	 * drawing
	 */
	RECT r = { 0, 0, wd, hgt };
	FillRect(hdcBmp, &r, static_cast<HBRUSH>(GetStockObject(WHITE_BRUSH)));

	for (int i = 0; i < numImages; ++i) {
		HDC imageMem = CreateCompatibleDC(hdcBmp);
		SelectObject(imageMem, images[i]);

		int xx = stripBorder;
		int yy = stripBorder + (previewHeight + stripBorder) * i;
		OutputDebugStringW( (std::to_wstring(xx) + L" " + std::to_wstring(yy) + L"\n").c_str() );
		BitBlt(hdcBmp, xx, yy, previewWidth, previewHeight, imageMem, 0, 0, SRCCOPY);

		DeleteDC(imageMem);
		DeleteObject(images[i]);
	}
	/*
	 *
	 */

	// Clean up the GDI objects we've created.
	SelectObject(hdcBmp, hbmOld);
	DeleteDC(hdcBmp);
}

void photoboothBegin() {
	photobooth_images = new HBITMAP[numImages];

	photobooth_images[0] = cnie::captureBitmap(preview1);
	std::this_thread::sleep_for(std::chrono::seconds(1));
	photobooth_images[1] = cnie::captureBitmap(preview1);
	std::this_thread::sleep_for(std::chrono::seconds(1));
	photobooth_images[2] = cnie::captureBitmap(preview1);
	std::this_thread::sleep_for(std::chrono::seconds(1));
	photobooth_images[3] = cnie::captureBitmap(preview1);

	HBITMAP img;
	HDC context;

	drawStrip(
		previewWidth + 2 * stripBorder,
		previewHeight * numImages + stripBorder * (numImages + 1),
		photobooth_images,
		img,
		context
	);

	PBITMAPINFO pbi = cnie::CreateBitmapInfoStruct(cnie::base_window, img);
	cnie::CreateBMPFile(cnie::base_window, (LPTSTR)L"Strip.bmp", pbi, img, context);

	ReleaseDC(NULL, context);
	delete photobooth_images;
}

LRESULT CALLBACK settingsProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message) {
	case WM_COMMAND:
	{
		int wmId = LOWORD(wParam);
			switch (wmId) {
			case VK_SPACE:
				break;
			default:
				return DefWindowProc(hwnd, message, wParam, lParam);
				break;
			}
	}
	break;
	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hwnd, &ps);

		EndPaint(hwnd, &ps);
	}
	break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	case WM_SIZE:
	{
		
	}
	break;
	break;
	default:
		return DefWindowProc(hwnd, message, wParam, lParam);
	}
	return 0;
}

void guiTestStartup() {
	button1 = cnie::createTextButton(10, 10, 400, 100, HOWDY_BUTTON, L"HOWDY");

	preview1 = cnie::createCaptureWindow(10, 120, 400, 400);

	cnie::registerClass(settingsProc, settingsWindowClass);

	settings_window = cnie::createSubWindow(settingsWindowClass, 0, 0, 100, 100, SETTINGS_WINDOW, L"Settings");

	guiCaptureStart();
}

bool guiButtonClick(int id) {
	switch (id) {
	case HOWDY_BUTTON:
		photoboothBegin();
		return true;
	default:
		return false;
	}
}

void guiFullscreen() {
	if (fullscreen) {
		fullscreen = false;
		cnie::setFullscreen(fullscreen);
	}
	else {
		fullscreen = true;
		cnie::setFullscreen(fullscreen);
	}
}

void guiKeypress(int key) {
	switch (key) {
	case VK_F12:
		exit(0);
		break;
	case VK_F11:
		guiFullscreen();
		break;
	case VK_F10:
		guiConnectCamera();
		break;
	case VK_F9:
		cnie::captureFrame(preview1);
		break;
	}
}

int APIENTRY wWinMain(HINSTANCE hi, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow)
{
	cnie::setup(hi, hPrevInstance, lpCmdLine, nCmdShow, guiTestStartup, guiResize, guiButtonClick, guiKeypress);
}