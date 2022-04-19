// ConsoleApplication1.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <Windows.h>
#include <string>

#include <curl/curl.h>
#include <fstream>

#include "Engine.h"


int APIENTRY WinMain(HINSTANCE hInst, HINSTANCE hInstPrev, PSTR cmdline, int cmdshow)
{
    

    Engine engine{ hInst };

    

    return engine.loop(cmdshow);
}

