#include "stdafx.h"
#include "resource.h"
#include "cnie.h"
#include <functional>

/*
 * DEFINE OUR FIELDS
 */

HINSTANCE cnie::hInstance;
WCHAR cnie::szTitle[MAX_LOADSTRING];
WCHAR cnie::szWindowClass[MAX_LOADSTRING];
HWND cnie::base_window;

std::function<void()> cnie::startup;
std::function<void()> cnie::onResize;
std::function<bool(int)> cnie::onButtonClick;
std::function<void(int)> cnie::onKeyPress;
std::function<void(HDC)> cnie::onDraw;

int cnie::winWidth;
int cnie::winHeight;
HHOOK cnie::keyHook;
bool cnie::fullscreen;
long cnie::defaultStyle;

// Message handler for about box.
INT_PTR CALLBACK cnie::about(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE: Processes messages for the main window.
//
//  WM_COMMAND  - process the application menu
//  WM_PAINT    - Paint the main window
//  WM_DESTROY  - post a quit message and return
//
//
LRESULT CALLBACK cnie::wndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message) {
	case WM_COMMAND:
	{
		int wmId = LOWORD(wParam);
		if (!onButtonClick(wmId)) {
			// Parse the menu selections:
			switch (wmId) {
			case IDM_ABOUT:
				DialogBox(hInstance, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, about);
				break;
			case IDM_EXIT:
				DestroyWindow(hWnd);
				break;
			default:
				return DefWindowProc(hWnd, message, wParam, lParam);
				break;
			}
		}
	}
	break;
	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hWnd, &ps);

		onDraw(hdc);

		EndPaint(hWnd, &ps);
	}
	break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	case WM_SIZE:
		{
			getWindowSizes();
		}
		break;
	case WM_SYSKEYUP:
		{
			onKeyPress(wParam);
		}
		break;
	case WM_KEYUP:
		{
			onKeyPress(wParam);
		}
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

ATOM cnie::registerClass(WNDPROC proc, const WCHAR className[]) {
	WNDCLASSEXW wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = proc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_GUITEST));
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_GUITEST);
	wcex.lpszClassName = className;
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	ATOM ret = RegisterClassExW(&wcex);

	return ret;
}

HWND cnie::create_window(const wchar_t className[], const wchar_t title[], long& style) {
	HWND ret = CreateWindowW(
		className,
		title,
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

	style = GetWindowLong(ret, GWL_STYLE);

	showWindow(ret, true);

	return ret;
}

HWND cnie::create_extended_window(const wchar_t className[], const wchar_t title[], long& style) {
	HWND ret = CreateWindowEx(
		0,
		className,
		title,
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

	style = GetWindowLong(ret, GWL_STYLE);

	return ret;
}

int cnie::setup(
	HINSTANCE hi, 
	std::function<void()> stp,
	std::function<void()> onr,
	std::function<bool(int)> obc, 
	std::function<void(int)> okp,
	std::function<void(HDC)> odw
) {
	hInstance = hi;
	startup = stp;
	onResize = onr;
	onButtonClick = obc;
	onKeyPress = okp;
	onDraw = odw;

	LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadStringW(hInstance, IDC_GUITEST, szWindowClass, MAX_LOADSTRING);

	registerClass(wndProc, szWindowClass);

	base_window = create_window(szWindowClass, szTitle, defaultStyle);

	startup();

	getWindowSizes();

	HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_GUITEST));

	MSG msg;

	// Main message loop:
	while (GetMessage(&msg, nullptr, 0, 0))
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{

			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return (int)msg.wParam;
}

void cnie::getWindowSizes() {
	RECT rect;
	GetClientRect(base_window, &rect);
	winWidth = (rect.right - rect.left);
	winHeight = (rect.bottom - rect.top);
	onResize();
}

void cnie::showWindow(HWND window, bool show) {
	if (show) {
		ShowWindow(window, SW_SHOW);
	}
	else {
		ShowWindow(window, SW_HIDE);
	}
	UpdateWindow(window);
	getWindowSizes();
}

void cnie::setFullscreen(HWND window, bool full) {
	if (full) {
		int width = GetSystemMetrics(SM_CXFULLSCREEN);
		int height = GetSystemMetrics(SM_CYFULLSCREEN);

		SetWindowPos(window, NULL, 0, 0, width, height, SWP_NOZORDER | SWP_NOACTIVATE | SWP_FRAMECHANGED);
		SetWindowLong(window, GWL_STYLE, GetWindowLong(window, GWL_STYLE) & ~(WS_CAPTION | WS_THICKFRAME));

		SendMessage(window, WM_SYSCOMMAND, SC_MAXIMIZE, 0);
	}
	else {
		int width = GetSystemMetrics(SM_CXFULLSCREEN);
		int height = GetSystemMetrics(SM_CYFULLSCREEN);

		SendMessage(window, WM_SYSCOMMAND, SC_RESTORE, 0);

		SetWindowPos(window, NULL, width / 4, height / 4, width / 2, height / 2, ~(SWP_NOZORDER | SWP_NOACTIVATE | SWP_FRAMECHANGED));
		SetWindowLong(window, GWL_STYLE, defaultStyle);

	}
}