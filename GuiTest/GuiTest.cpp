#include "stdafx.h"
#include "cnie.h"
#include <string>
#include <iostream>

#define HOWDY_BUTTON 100
#define PHOTOBOOTH_FRAMEBUFFER 101

int numImages = 4;
HBITMAP* photobooth_images;
HWND frameBuffer;
int stripBorder = 10;

HWND preview1;
HWND button1;
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

void drawPhotoboothImages(HWND frameBuffer, HBITMAP* images) {
	PAINTSTRUCT ps;
	HDC paint = BeginPaint(frameBuffer, &ps);
	HDC imageMem;

	//draw images onto framebuffer
	for (int i = 0; i < numImages; ++i) {
		imageMem = CreateCompatibleDC(paint);
		// Select the new bitmap
		SelectObject(imageMem, images[i]);

		// Copy the bits from the memory DC into the current dc
		BitBlt(paint, stripBorder, stripBorder + (previewHeight + stripBorder) * i, previewWidth, previewHeight, imageMem, 0, 0, SRCCOPY);

		// Restore the old bitmap
		DeleteDC(imageMem);
		DeleteObject(images[i]);
	}

	HDC bufferMem = CreateCompatibleDC(paint);
	RECT bufferRect;
	GetClientRect(frameBuffer, &bufferRect);
	HBITMAP bufferImage = CreateCompatibleBitmap(bufferMem, bufferRect.right, bufferRect.bottom);

	PBITMAPINFO pbi = cnie::CreateBitmapInfoStruct(cnie::base_window, bufferImage);
	cnie::CreateBMPFile(frameBuffer, (LPTSTR)L"Strip.bmp", pbi, bufferImage, bufferMem);

	EndPaint(frameBuffer, &ps);
}

HWND createFrameBuffer(int w, int h) {
	return CreateWindow(
		L"STATIC",
		NULL,
		NULL,
		0,
		0,
		w,
		h,
		cnie::base_window,
		(HMENU)PHOTOBOOTH_FRAMEBUFFER,
		cnie::hInstance,
		NULL
	);
}

void photoboothBegin() {
	photobooth_images = new HBITMAP[numImages];

	photobooth_images[0] = cnie::captureBitmap(preview1);
	photobooth_images[1] = cnie::captureBitmap(preview1);
	photobooth_images[2] = cnie::captureBitmap(preview1);
	photobooth_images[3] = cnie::captureBitmap(preview1);

	drawPhotoboothImages(frameBuffer, photobooth_images);

	delete photobooth_images;
}

void guiTestStartup() {
	button1 = cnie::createTextButton(10, 10, 400, 100, HOWDY_BUTTON, L"HOWDY");

	preview1 = cnie::createCaptureWindow(10, 120, 400, 400);

	guiCaptureStart();

	frameBuffer = createFrameBuffer(previewWidth + 2 * stripBorder, previewHeight * numImages + stripBorder * (numImages + 1));
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