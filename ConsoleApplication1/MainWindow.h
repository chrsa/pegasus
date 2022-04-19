#pragma once

#include "MessageWindow.h"
#include "ControlWindow.h"
#include "MP3Player.h"

class MainWindow
{
public:

	MainWindow(void (*callback)(void*), void* obj, HINSTANCE hInst);
	bool ClipBoardChanged();

	void show();

	bool isPlayAutomatically() const;
	bool PlayAutomaticallyChanged();

	bool NewAudioCommand();

	AudioCommand ReadAudioCommand() const;

	const std::string& TextToSpeech() const;
	bool NewTextToSpeechCommand();

	void ShowMp3Buttons();

	void HideMp3Buttons();

	const std::string& Voice() const;

private:

	static void ClipboardNotificationCallback(void* data);
	static void ControlWindowNotificationCallback(void* data);

	void Invoke();

	ClipboardMessageWindow clipboardWindow_;
	ControlWindow controlWindow_;
	bool clipBoardChanged_;

	void (*callback_)(void*);
	void* callbackObj_;

	bool playAutomaticallyChanged_;

	bool newAudioCommand_;

	bool textToSpeechCommand_;
};

