#include "stdafx.h"
#include "resource.h"
#include "cnie.h"

//
// DEFINE OUR FIELDS
//

HINSTANCE cnie::hInstance;
WCHAR cnie::szTitle[MAX_LOADSTRING];
WCHAR cnie::szWindowClass[MAX_LOADSTRING];
HWND cnie::base_window;
void(*cnie::startup)();
void(*cnie::onResize)();
bool(*cnie::onButtonClick)(int);
int cnie::winWidth;
int cnie::winHeight;
HHOOK cnie::keyHook;

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

		//TODO paint stuff

		EndPaint(hWnd, &ps);
	}
	break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	case WM_EXITSIZEMOVE:
		{
			getWindowSizes();
		}
		break;
	case WM_KEYUP:
		{
			
		}
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

ATOM cnie::myRegisterClass(HINSTANCE hInstance) {
	WNDCLASSEXW wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = wndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_GUITEST));
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_GUITEST);
	wcex.lpszClassName = szWindowClass;
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassExW(&wcex);
}

void cnie::InitWindow(int nCmdShow) {
	HWND hwnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

	base_window = hwnd;

	ShowWindow(hwnd, nCmdShow);
	UpdateWindow(hwnd);
}

int cnie::setup(HINSTANCE hi, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow, void(*stp)(), void(*onr)(), bool(*obc)(int)) {
	hInstance = hi;
	startup = stp;
	onResize = onr;
	onButtonClick = obc;

	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadStringW(hInstance, IDC_GUITEST, szWindowClass, MAX_LOADSTRING);
	myRegisterClass(hInstance);

	InitWindow(nCmdShow);

	keyHook = SetWindowsHookEx(WH_KEYBOARD_LL, hookProc, hInstance, 0);

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

LRESULT CALLBACK cnie::hookProc(int code, WPARAM wParam, LPARAM lParam) {
	KBDLLHOOKSTRUCT*  kbd = (KBDLLHOOKSTRUCT*)lParam;

	if (wParam == WM_KEYUP) {
		switch (kbd->vkCode) {
		case VK_F12:
			exit(0);
		}
	}

	return CallNextHookEx(keyHook, code, wParam, lParam);
}