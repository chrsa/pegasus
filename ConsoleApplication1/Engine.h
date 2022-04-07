#pragma once

#include "Client.h"
#include <Windows.h>
#include "LoginWindow.h"
#include "MessageWindow.h"

class Engine
{
public:
	Engine(HINSTANCE hInst);

	int login(const std::string& email, const std::string& password);

	int loop(int cmdshow);

	LoginWindow& loginWindow();

private:

	static LRESULT CALLBACK MessageWndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	static LRESULT CALLBACK GUIWndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

	Client client_;
	HWND myWindow_;
	LoginWindow loginWindow_;
	MessageWindow messageWindow_;

	static constexpr int IDT_TIMER1 = 1001;
};

