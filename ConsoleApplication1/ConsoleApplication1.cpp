// ConsoleApplication1.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <Windows.h>
#include <string>

#include <curl/curl.h>
#include <fstream>

#include "Clipboard.h"
#include "Http.h"
#include "Json.h"
#include "Play.h"


LRESULT CALLBACK MainWndProc(HWND, UINT messageID, WPARAM, LPARAM)
{
    switch (messageID)
    {
    case WM_CLIPBOARDUPDATE:
        std::cout << "break here" << std::endl;


        mciSendStringA("stop myaudiofileplayht", NULL, 0, 0);
        mciSendStringA("close myaudiofileplayht", NULL, 0, 0);

        std::string result = play.transcribe(newClipboard);
        std::ofstream ofs{ "teste.mp3", std::ios::binary | std::ios_base::trunc };
        ofs << result;
        ofs.flush();
        ofs.close();

        MCIERROR err1 = mciSendStringA("open teste.mp3 type mpegvideo alias myaudiofileplayht", NULL, 0, 0);
        MCIERROR err2 = mciSendStringA("play myaudiofileplayht", NULL, 0, 0);

        break;

    }
}


int APIENTRY WinMain(HINSTANCE hInst, HINSTANCE hInstPrev, PSTR cmdline, int cmdshow)
{
    
    const char* class_name = "DUMMY_CLASS";
    WNDCLASSEXA wx = {};
    wx.cbSize = sizeof(WNDCLASSEX);
    wx.lpfnWndProc = MainWndProc;        // function which will handle messages
    wx.hInstance = hInst;
    wx.lpszClassName = "DUMMY_CLASS";
    RegisterClassExA(&wx);
    HWND myWindow = CreateWindowExA(0, class_name, nullptr, 0, 0, 0, 0, 0, HWND_MESSAGE, NULL, NULL, NULL);
    

    bool ok = AddClipboardFormatListener(myWindow);
    DWORD error = GetLastError();

    Clipboard clipBoardReader;

    Play play;

    play.login("christiano.sa@outlook.com", "PlayHt01");

    std::string clipboard;
    for (;;)
    {
        std::string newClipboard = clipBoardReader.read();

        if (newClipboard != clipboard) {
            
            mciSendStringA("stop myaudiofileplayht", NULL, 0, 0);
            mciSendStringA("close myaudiofileplayht", NULL, 0, 0);

            std::string result = play.transcribe(newClipboard);
            std::ofstream ofs{ "teste.mp3", std::ios::binary | std::ios_base::trunc };
            ofs << result;
            ofs.flush();
            ofs.close();

            MCIERROR err1 = mciSendStringA("open teste.mp3 type mpegvideo alias myaudiofileplayht", NULL, 0, 0);
            MCIERROR err2 = mciSendStringA("play myaudiofileplayht", NULL, 0, 0);
            

            clipboard = std::move(newClipboard);
        }

        Sleep(100);
    }
    
   

    return 0;
}
