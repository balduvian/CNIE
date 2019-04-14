#include "stdafx.h"

namespace photobooth {

	#define HOWDY_BUTTON 100
	#define SETTINGS_WINDOW 101

	extern const WCHAR settingsWindowClass[MAX_LOADSTRING];

	extern int num_images;
	extern HBITMAP* photobooth_images;
	extern int strip_border;

	extern HWND preview1;
	extern HWND button1;
	extern HWND settings_window;

	extern int preview_width;
	extern int preview_height;

	extern bool full;

	void startup();
	void resize();
	void camera_settings();
	void start_capture();
	//void stop_capture();
	void draw_strip(int w, int h, int b, HBITMAP* images, int ni, HBITMAP& bo, HDC& co);
	void photobooth_routine();
	void full_screen();

	bool button_click(int);
	void key_press(int);
	void window_draw(HWND, HDC);

	LRESULT CALLBACK settingsProc(HWND, UINT, WPARAM, LPARAM);
}
