#pragma once

#include <Windows.h>
#include <string>
#include "VoiceDataBase.h"

enum class SourceType
{
	CLIPBOARD,
	TEXT
};

enum class AudioCommand
{
	PLAY,
	STOP
};

class ControlWindow
{
public:

	ControlWindow(void (*callback)(void*), void* obj, HINSTANCE hInst);
	void create(HINSTANCE hInst);

	void show();

	SourceType source() const;

	bool isPlayAutomatically() const;

	bool sourceRadioChanged();

	bool PlayAutomaticallyChanged();

	bool NewAudioCommand();

	AudioCommand ReadAudioCommand() const;

	
	bool NewTextToSpeechCommand();

	const std::string& TextToSpeech() const;

	void ShowMp3Buttons();
	void HideMp3Buttons();

	const std::string& Voice() const;

private:

	void clickTextCommand();
	void clickAudioPlayCommand(AudioCommand);

	void Invoke();

	static LRESULT CALLBACK GUIWndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);


	void clickRadio(SourceType source);
	void clickPlayAutomatically(bool checked);

	void (*callback_)(void*);
	void* callbackObj_;

	HDC hdcSource_;
	HBITMAP logo_;
	HBRUSH hbrBkgnd_;

	HWND window_;
	HWND textButton_;
	HWND playAutomaticallyButton_;
	HWND playButton_;
	HWND stopButton_;
	HWND saveAsButton_;
	HWND OkTextButton_;
	HWND Text_;

	HWND comboIdiom_;
	HWND comboGenre_;
	HWND comboVoiceType_;

	HINSTANCE hInst_;

	SourceType source_;
	bool checkedPlayAutomatically_;
	bool playAutomaticallyChanged_;
	bool sourceRadioChanged_;
	bool newAudioCommand_;
	AudioCommand audioCommand_;

	std::string textStr_;
	bool newTextCommand_;

	VoiceDataBase voiceDb_;

	std::string voice_;
};

