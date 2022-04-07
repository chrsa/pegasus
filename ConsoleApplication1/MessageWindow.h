#pragma once
#include <Windows.h>

class MessageWindow
{

public:
	void create(HINSTANCE hInst, LPVOID data, WNDPROC proc);

	HWND handler() const;
	
	void show(int cmdshow);


private:

	HWND window_;
};


