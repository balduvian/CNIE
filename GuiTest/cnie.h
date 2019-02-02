#pragma once

#include "stdafx.h"
#include "GuiTest.h"
#include "resource.h"

namespace cnie {
	
	constexpr int MAX_LOADSTRING = 100;

	WCHAR szTitle[MAX_LOADSTRING];
	WCHAR szWindowClass[MAX_LOADSTRING];

	HINSTANCE hInstance;

	void setup();

	HWND createWindow(bool full);
	HWND createImageButton(int x, int y, int width, int height);
	HWND createTextButton(int x, int y, int width, int height, const wchar_t* text);

	HANDLE loadBitmap(int id);
	void setButtonImage(HWND button, HANDLE image);

}