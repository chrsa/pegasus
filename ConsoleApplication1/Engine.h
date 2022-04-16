#pragma once

#include <Windows.h>
//#include "LoginWindow.h"
#include "MP3Player.h"
#include "Play.h"
#include "Clipboard.h"
#include "MainWindow.h"
#include "LoginWindow.h"

class Engine
{
public:
	Engine(HINSTANCE hInst);

	int login(const std::string& email, const std::string& password);

	int loop(int cmdshow);

private:

	static void mainWindowCallBack(void*);
	static void loginWindowCallBack(void*);

	Play play_;
	Clipboard clipboard_;
	
	MainWindow mainWindow_;
	LoginWindow loginWindow_;

	MP3Player player_;

	HINSTANCE hinst_;
	
};

