#include "stdafx.h"
#include "resource.h"

HWND button1;

#define MAX_LOADSTRING 100

HINSTANCE hInstance;
WCHAR szTitle[MAX_LOADSTRING];
WCHAR szWindowClass[MAX_LOADSTRING];
HWND base_window;
void(*startup)();

// Message handler for about box.
INT_PTR CALLBACK about(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
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
LRESULT CALLBACK wndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message) {
	case WM_CREATE:
	{
		startup();
	}
	break;
	case WM_COMMAND:
	{
		int wmId = LOWORD(wParam);
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
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

ATOM myRegisterClass(HINSTANCE hInstance)
{
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

BOOL InitInstance(bool full) {

	HWND hwnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

	if (!hwnd)
	{
		return FALSE;
	}
	else {
		base_window = hwnd;
	}

	ShowWindow(hwnd, full ? SW_SHOWMAXIMIZED : SW_SHOW);
	UpdateWindow(hwnd);

	return TRUE;
}

int setup(
	_In_ HINSTANCE hi,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR    lpCmdLine,
	_In_ int       nCmdShow,
	void(*stp)()
)
{
	hInstance = hi;
	startup = stp;

	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	// TODO: Place code here.

	// Initialize global strings
	LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadStringW(hInstance, IDC_GUITEST, szWindowClass, MAX_LOADSTRING);
	myRegisterClass(hInstance);

	// Perform application initialization:
	InitInstance(false);

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

HWND createTextButton(int x, int y, int width, int height, const wchar_t* text) {
	HWND button = CreateWindow(
		L"BUTTON",  
		L"YO WHO UP",     
		WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,  
		10,          
		10,         
		300,       
		100,       
		base_window,     
		(HMENU)0,
		hInstance,
		NULL
	);      

	return button;
}

HWND createBlankButton(int x, int y, int width, int height) {

	HWND button = CreateWindow(
		L"BUTTON",  
		NULL,      
		WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,  
		x,
		y,
		width,
		height,
		base_window,
		(HMENU)0,
		hInstance,
		NULL
	);

	return button;
}

HWND createImageButton(int x, int y, int width, int height) {
	HWND button = CreateWindow(
		L"BUTTON",
		NULL,
		WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON | BS_BITMAP,
		10,
		10,
		300,
		100,
		base_window,
		(HMENU)0,
		hInstance,
		NULL
	);
	return button;
}

HANDLE loadBitmap(int id) {
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

HANDLE loadBitmap(int id, int w, int h) {
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

void setButtonImage(HWND button, HANDLE image) {
	SendMessage(button, BM_SETIMAGE, (WPARAM)IMAGE_BITMAP, (LPARAM)image);
}
void guiTestStartup() {
	button1 = createBlankButton(10, 10, 100, 100);
}

int APIENTRY wWinMain(_In_ HINSTANCE hi,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
	setup(hi, hPrevInstance, lpCmdLine, nCmdShow, guiTestStartup);
}
