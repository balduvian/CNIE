#include "stdafx.h"
#include "GuiTest.h"
#include "resource.h"
#include "cnie.h"

HWND cnie::createWindow(bool full) {

	HWND window = CreateWindowW(
		szWindowClass,
		szTitle,
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT,
		0,
		CW_USEDEFAULT,
		0,
		nullptr,
		nullptr,
		hInstance,
		nullptr
	);

	if (!window)
	{
		return nullptr;
	}

	if (full) {
		ShowWindow(window, SW_SHOWMAXIMIZED);
	}
	else {
		ShowWindow(window, SW_SHOW);
	}
	UpdateWindow(window);

	return window;
}

HWND cnie::createTextButton(int x, int y, int width, int height, const wchar_t* text) {
	HWND button = CreateWindow(
		L"BUTTON",  // Predefined class; Unicode assumed 
		L"YO WHO UP",      // Button text 
		WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,  // Styles 
		10,         // x position 
		10,         // y position 
		300,        // Button width
		100,        // Button height
		hWnd,     // Parent window
		NULL,       // No menu.
		(HINSTANCE)GetWindowLong(hWnd, GWL_HINSTANCE),
		NULL);      // Pointer not needed.

	return button;
}
