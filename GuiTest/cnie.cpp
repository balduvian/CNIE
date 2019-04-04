#include "stdafx.h"
#include "cnie.h"

//DEFINE FIELDS
WNDPROC cnie::oldProc;

HWND cnie::createTextButton(int x, int y, int width, int height, int id, const wchar_t* text) {
	HWND button = CreateWindow(
		L"BUTTON",
		text,
		WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
		x,
		y,
		width,
		height,
		base_window,
		(HMENU)id,
		hInstance,
		NULL
	);

	return button;
}

HWND cnie::createBlankButton(int x, int y, int width, int height, int id) {

	HWND button = CreateWindow(
		L"BUTTON",
		NULL,
		WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
		x,
		y,
		width,
		height,
		base_window,
		(HMENU)id,
		hInstance,
		NULL
	);

	hInstance;
	base_window;

	return button;
}

HWND cnie::createImageButton(int x, int y, int width, int height, int id) {
	HWND button = CreateWindow(
		L"BUTTON",
		NULL,
		WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON | BS_BITMAP,
		x,
		y,
		width,
		height,
		base_window,
		(HMENU)id,
		hInstance,
		NULL
	);
	return button;
}

HANDLE cnie::loadBitmap(int id) {
	HANDLE ret = LoadImage(
		hInstance,
		MAKEINTRESOURCE(id),
		IMAGE_BITMAP,
		0,
		0,
		LR_DEFAULTCOLOR
	);

	return ret;
}

HANDLE cnie::loadBitmap(int id, int w, int h) {
	HANDLE ret = LoadImage(
		hInstance,
		MAKEINTRESOURCE(id),
		IMAGE_BITMAP,
		w,
		h,
		LR_DEFAULTCOLOR
	);

	return ret;
}

void cnie::setButtonImage(HWND button, HANDLE image) {
	SendMessage(button, BM_SETIMAGE, (WPARAM)IMAGE_BITMAP, (LPARAM)image);
}

void cnie::setButtonText(HWND button, const wchar_t* text) {
	SendMessage(button, WM_SETTEXT, NULL, (LPARAM)text);
}

void cnie::setButtonSize(HWND button, int x, int y, int width, int height) {
	MoveWindow(
		button,
		x,
		y,
		width,
		height,
		TRUE
	);
}