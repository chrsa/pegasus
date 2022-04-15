#pragma once

#include "Client.h"
#include <Windows.h>
//#include "LoginWindow.h"
#include "MessageWindow.h"
#include "MP3Player.h"

class Engine
{
public:
	Engine(HINSTANCE hInst);

	int login(const std::string& email, const std::string& password);

	int loop(int cmdshow);

	//LoginWindow& loginWindow();

private:

	static void EngineCallBack(void*);

	Client client_;
	HWND myWindow_;
//	LoginWindow loginWindow_;
	MessageWindow messageWindow_;
	MP3Player player_;
	
};

