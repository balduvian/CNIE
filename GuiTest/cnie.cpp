#include "stdafx.h"
#include "GuiTest.h"
#include "resource.h"
#include "cnie.h"

void cnie::setup() {

}

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


}