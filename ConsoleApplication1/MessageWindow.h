#pragma once
#include <Windows.h>

class ClipboardMessageWindow
{

public:
	ClipboardMessageWindow( void (*callback)(void *), void *obj);

	void create(HINSTANCE hInst);

	HWND handler() const;
	
	void show(int cmdshow);

	bool ClipBoardChanged();

	void enable();

	void disable();

	~ClipboardMessageWindow();

private:

	static LRESULT CALLBACK MessageWndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

	static constexpr int IDT_TIMER1 = 1001;

	void callbackCall();

	HWND window_;

	bool clipBoardChanged_;

	void (*callback_)(void *);
	void *callbackObj_;

	bool enabled_;
};


