#include "Engine.h"
#include <Windows.h>
#include <winuser.h>
#include <fstream>
#include <iostream>

void Engine::EngineCallBack(void* data)
{
    Engine* engine = reinterpret_cast<Engine *>( data);

    if (engine->messageWindow_.ClipBoardChanged())
    {
        engine->player_.stop();
        std::string old = engine->client_.clipboard().last();
        std::string newClipboard = engine->client_.clipboard().read();

        if (newClipboard != old)
        {
            std::string mp3;
            int status = engine->client_.play().transcribe(newClipboard, mp3);
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
: messageWindow_{ EngineCallBack, this }
{
    messageWindow_.create(hInst);
    //loginWindow_.create(hInst);
}

int Engine::login(const std::string &email, const std::string &password)
{
    return client_.play().login(email, password);
}


int Engine::loop(int cmdshow)
{
    //loginWindow_.show(cmdshow);
    messageWindow_.show(cmdshow);

    MSG msg;
    BOOL bRet;

    while ((bRet = GetMessageA(&msg, NULL, 0, 0)) != 0)
    {
        if (bRet == -1)
        {
            break;
        }
        else
        {
            TranslateMessage(&msg);
            DispatchMessageA(&msg);
        }
    }

    return msg.wParam;
}


/*LoginWindow& Engine::loginWindow()
{
    return loginWindow_;
}
*/