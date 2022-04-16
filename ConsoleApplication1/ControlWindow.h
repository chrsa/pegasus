#pragma once

#include <Windows.h>

class ControlWindow
{
public:

	ControlWindow(void (*callback)(void*), void* obj, HINSTANCE hInst);
	void create(HINSTANCE hInst);

	void show();

private:

	static LRESULT CALLBACK GUIWndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

	void (*callback_)(void*);
	void* callbackObj_;


	HBRUSH hbrBkgnd_;
	HWND window_;
};

