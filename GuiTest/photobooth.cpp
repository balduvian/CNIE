#include "stdafx.h"
#include "cnie.h"
#include "photobooth.h"
#include <string>
#include <iostream>
#include <chrono>
#include <thread>

const WCHAR photobooth::settingsWindowClass[MAX_LOADSTRING] = L"SettingsWindow";

int photobooth::num_images = 4;
HBITMAP* photobooth::photobooth_images;
int photobooth::strip_border = 10;

HWND photobooth::preview1;
HWND photobooth::button1;
HWND photobooth::settings_window;

int photobooth::preview_width = 0;
int photobooth::preview_height = 0;

bool photobooth::full = false;

//TODO VALIDATE THIS POSITIONING
void photobooth::resize() {
	cnie::setButtonSize(button1, 10, cnie::winHeight - 110, cnie::winWidth - 20, 100);
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

	float camAspect = (float)preview_width / preview_height;

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

	cnie::setButtonSize(preview1, 10 + offsetX, 10 + offsetY, finalWidth, finalHeight);
}

void photobooth::camera_settings() {
	cnie::connectWebcam(preview1);
}

void photobooth::start_capture() {
	cnie::startCapture(preview1);
	cnie::getCaptureDims(preview1, preview_width, preview_height);
	resize();
}

void photobooth::draw_strip(int image_width, int image_height, int border, HBITMAP* images, int ni, HBITMAP& bmp, HDC& hdcScreen) {

	int full_width = image_width + 2 * border;
	int full_height = image_height * ni + border * (ni + 1);

	// Get a device context to the screen.
	hdcScreen = GetDC(NULL);

	// Create a device context
	HDC hdcBmp = CreateCompatibleDC(hdcScreen);

	// Create a bitmap and attach it to the device context we created above...
	bmp = CreateCompatibleBitmap(hdcScreen, full_width, full_height);
	HBITMAP hbmOld = static_cast<HBITMAP>(SelectObject(hdcBmp, bmp));

	/*
	 * drawing
	 */
	RECT r = { 0, 0, full_width, full_height };
	FillRect(hdcBmp, &r, static_cast<HBRUSH>(GetStockObject(WHITE_BRUSH)));

	for (int i = 0; i < ni; ++i) {
		HDC imageMem = CreateCompatibleDC(hdcBmp);
		SelectObject(imageMem, images[i]);

		int xx = border;
		int yy = border + (image_height + border) * i;
		OutputDebugStringW( (std::to_wstring(xx) + L" " + std::to_wstring(yy) + L"\n").c_str() );
		BitBlt(hdcBmp, xx, yy, image_width, image_height, imageMem, 0, 0, SRCCOPY);

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

void photobooth::photobooth_routine() {
	photobooth_images = new HBITMAP[num_images];

	photobooth_images[0] = cnie::captureBitmap(preview1);
	std::this_thread::sleep_for(std::chrono::seconds(1));
	photobooth_images[1] = cnie::captureBitmap(preview1);
	std::this_thread::sleep_for(std::chrono::seconds(1));
	photobooth_images[2] = cnie::captureBitmap(preview1);
	std::this_thread::sleep_for(std::chrono::seconds(1));
	photobooth_images[3] = cnie::captureBitmap(preview1);

	HBITMAP img;
	HDC context;

	draw_strip(
		preview_width,
		preview_height,
		strip_border,
		photobooth_images,
		num_images,
		img,
		context
	);

	PBITMAPINFO pbi = cnie::CreateBitmapInfoStruct(cnie::base_window, img);
	cnie::CreateBMPFile(cnie::base_window, (LPTSTR)L"Strip.bmp", pbi, img, context);

	ReleaseDC(NULL, context);
	delete photobooth_images;
}

LRESULT CALLBACK photobooth::settingsProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
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
		//we don't want anything happening here
		break;
	case WM_SIZE:
	{
		
	}
	break;
	default:
		return DefWindowProc(hwnd, message, wParam, lParam);
	}
	return 0;
}

void photobooth::startup() {
	button1 = cnie::createTextButton(10, 10, 400, 100, HOWDY_BUTTON, L"HOWDY");

	preview1 = cnie::createCaptureWindow(10, 120, 400, 400);

	cnie::registerClass(settingsProc, settingsWindowClass);
	long yeet = 0;
	settings_window = cnie::create_extended_window(settingsWindowClass, L"Settings", yeet);

	cnie::showWindow(settings_window, true);

	start_capture();
}

bool photobooth::button_click(int id) {
	switch (id) {
	case HOWDY_BUTTON:
		photobooth_routine();
		return true;
	default:
		return false;
	}
}

void photobooth::full_screen() {
	full = !full;
	cnie::setFullscreen(cnie::base_window, full);
}

void photobooth::key_press(int key) {
	switch (key) {
	case VK_F12:
		PostQuitMessage(0);
		break;
	case VK_F11:
		full_screen();
		break;
	case VK_F10:
		camera_settings();
		break;
	case VK_F9:
		cnie::captureFrame(preview1);
		break;
	}
}

void photobooth::window_draw(HDC hdc) {
	RECT rect = { 0, 0, cnie::winWidth, cnie::winHeight };
	FillRect(hdc, &rect, static_cast<HBRUSH>(GetStockObject(BLACK_BRUSH)));
}

int APIENTRY wWinMain(HINSTANCE h_instance, HINSTANCE, LPWSTR, int)
{
	cnie::setup(h_instance, photobooth::startup, photobooth::resize, photobooth::button_click, photobooth::key_press, photobooth::window_draw);
}