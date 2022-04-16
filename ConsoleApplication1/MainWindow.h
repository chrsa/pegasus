#pragma once

#include "MessageWindow.h"
#include "ControlWindow.h"
#include "MP3Player.h"

class MainWindow
{
public:

	MainWindow(void (*callback)(void*), void* obj, HINSTANCE hInst);
	bool ClipBoardChanged();

	static void ClipboardNotificationCallback(void* data);

	void show();

private:

	void Invoke();

	ClipboardMessageWindow clipboardWindow_;
	ControlWindow controlWindow_;
	bool clipBoardChanged_;

	void (*callback_)(void*);
	void* callbackObj_;

	bool enabled_;
};

