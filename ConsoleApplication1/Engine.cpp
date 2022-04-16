#include "Engine.h"
#include <Windows.h>
#include <winuser.h>
#include <fstream>
#include <iostream>


void Engine::loginWindowCallBack(void* data)
{
    Engine* engine = reinterpret_cast<Engine*>(data);

    if (engine->login(engine->loginWindow_.getUsername(), engine->loginWindow_.getPassword()) != 200)
    {
        MessageBoxA(NULL, "Incorrect Username or Password.", "Error", MB_OK);
        return;
    }

    engine->loginWindow_.hide();
    engine->mainWindow_.show();
}

void Engine::mainWindowCallBack(void* data)
{
    Engine* engine = reinterpret_cast<Engine *>( data);

    if (engine->mainWindow_.ClipBoardChanged())
    {
        engine->player_.stop();
        std::string old = engine->clipboard_.last();
        std::string newClipboard = engine->clipboard_.read();

        if (newClipboard != old)
        {
            std::string mp3;
            int status = engine->play_.transcribe(newClipboard, mp3);
            if (status != 200)
            {
                return;
            }

            std::ofstream ofs{ "teste.mp3", std::ios::binary | std::ios_base::trunc };
            ofs << mp3;
            ofs.flush();
            ofs.close();
        }
        

        engine->player_.play();
    }
}

Engine::Engine(HINSTANCE hInst)
    : mainWindow_{ mainWindowCallBack , this, hInst }
    , loginWindow_{ loginWindowCallBack , this, hInst }
{
    
}

int Engine::login(const std::string &email, const std::string &password)
{
    return play_.login(email, password);
}


int Engine::loop( int cmdshow)
{
    loginWindow_.show();
 
    MSG msg;
    BOOL bRet;

    while ( (bRet = GetMessageA(&msg, NULL, 0, 0)) != 0)
    {
        if (bRet == -1)
        {
            break;
        }
        else
        {
            if (!msg.hwnd || !IsDialogMessageA(loginWindow_.windowHandler(), &msg)) {
                TranslateMessage(&msg);
                DispatchMessageA(&msg);
            }
        }
    }

    return msg.wParam;
}

