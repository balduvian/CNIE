#include "stdafx.h"
#include "cnie.h"

HWND button1;

void guiTestStartup() {
	button1 = cnie::createBlankButton(10, 10, 100, 100, 0);
}

int APIENTRY wWinMain(HINSTANCE hi, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow)
{
	cnie::setup(hi, hPrevInstance, lpCmdLine, nCmdShow, guiTestStartup);
}
