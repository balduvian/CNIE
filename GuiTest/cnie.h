#pragma once

#include "stdafx.h"
#include "GuiTest.h"
#include "resource.h"

namespace cnie {
	
	constexpr int MAX_LOADSTRING = 100;

	HINSTANCE hInstance;
	WCHAR szTitle[MAX_LOADSTRING];
	WCHAR szWindowClass[MAX_LOADSTRING];

	ATOM                MyRegisterClass(HINSTANCE hInstance);
	LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
	INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

	HWND window;

	int setup(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nCmdShow);

	HWND createWindow(bool full);
	HWND createImageButton(int x, int y, int width, int height);
	HWND createTextButton(int x, int y, int width, int height, const wchar_t* text);

	HANDLE loadBitmap(int id);
	void setButtonImage(HWND button, HANDLE image);

}