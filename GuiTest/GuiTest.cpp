#include "stdafx.h"
#include "cnie.h"
#include <string>
#include <iostream>

HWND button1;
int switcher = 0;

void doSomething() {
	std::wstring s = std::to_wstring(switcher);
	cnie::setButtonText(button1, s.c_str());
	++switcher;
}

buttonProc(button1Proc, doSomething)

void guiTestStartup() {
	button1 = cnie::createTextButton(10, 10, 400, 100, L"HOWDY");
	cnie::registerProc(button1, button1Proc);
}

void guiResize() {
	cnie::setButtonSize(button1, 10, 10, cnie::winWidth - 20, 100);
}

int APIENTRY wWinMain(HINSTANCE hi, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow)
{
	cnie::setup(hi, hPrevInstance, lpCmdLine, nCmdShow, guiTestStartup, guiResize);
}