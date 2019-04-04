#include "stdafx.h"
#include "cnie.h"
#include <string>
#include <iostream>

#define HOWDY_BUTTON 100

HWND preview1;
HWND button1;
int switcher = 0;

void doSomething() {
	std::wstring s = std::to_wstring(switcher);
	cnie::setButtonText(button1, s.c_str());
	++switcher;
	cnie::startCapture(preview1);

	cnie::getCaps(preview1);
}

void guiTestStartup() {
	button1 = cnie::createTextButton(10, 10, 400, 100, HOWDY_BUTTON, L"HOWDY");
	//cnie::registerProc(button1, button1Proc);

	preview1 = cnie::createCaptureWindow(10, 120, 400, 400);
	cnie::connectWebcam(preview1);
}

void guiResize() {
	cnie::setButtonSize(button1, 10, 10, cnie::winWidth - 20, 100);
	cnie::setButtonSize(preview1, 10, 120, cnie::winWidth - 20, cnie::winHeight - 130);
}

bool guiButtonClick(int id) {
	switch (id) {
	case HOWDY_BUTTON:
		doSomething();
		return true;
	default:
		return false;
	}
}

int APIENTRY wWinMain(HINSTANCE hi, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow)
{
	cnie::setup(hi, hPrevInstance, lpCmdLine, nCmdShow, guiTestStartup, guiResize, guiButtonClick);
}