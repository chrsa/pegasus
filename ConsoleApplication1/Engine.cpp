#include "Engine.h"
#include <Windows.h>
#include <winuser.h>
#include <fstream>
#include <iostream>

LRESULT CALLBACK Engine::GUIWndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
    case WM_CREATE:
    {
        CREATESTRUCT* pCreate = reinterpret_cast<CREATESTRUCT*>(lParam);
        int* userdata = reinterpret_cast<int*>(pCreate->lpCreateParams);
        SetWindowLongPtrA(hWnd, GWLP_USERDATA, (LONG_PTR)userdata);

        Engine* engine = reinterpret_cast<Engine*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));
        engine->loginWindow().setup(hWnd);
        break;
    }
    case WM_COMMAND:
    {
        Engine* engine = reinterpret_cast<Engine*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));

        if (HIWORD(wParam) == BN_CLICKED) {

            char user[256];
            GetWindowTextA(engine->loginWindow().userHandler(), user, sizeof(user));

            char pass[256];
            GetWindowTextA(engine->loginWindow().passHandler(), pass, sizeof(pass));

            int status = engine->client_.play().login(user, pass);
        }
        break;
    }
    default:
        break;
    }

    return DefWindowProcA(hWnd, uMsg, wParam, lParam);
}

LRESULT CALLBACK Engine::MessageWndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
    case WM_CLIPBOARDUPDATE:
    {
        SetTimer(hWnd,
            IDT_TIMER1,            
            500,                
            (TIMERPROC)NULL);

        break;
    }
    case WM_CREATE:
    {
        CREATESTRUCT* pCreate = reinterpret_cast<CREATESTRUCT*>(lParam);
        int* userdata = reinterpret_cast<int*>(pCreate->lpCreateParams);
        SetWindowLongPtrA(hWnd, GWLP_USERDATA, (LONG_PTR)userdata);

      

        break;
    }
    case WM_TIMER:
    {

        switch (wParam)
        {
        case IDT_TIMER1:
        {
            KillTimer(hWnd, IDT_TIMER1);

            mciSendStringA("stop myaudiofileplayht", NULL, 0, 0);
            mciSendStringA("close myaudiofileplayht", NULL, 0, 0);

            Engine* engine = reinterpret_cast<Engine*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));
            std::string newClipboard = engine->client_.clipboard().read();

            std::string mp3;
            int status = engine->client_.play().transcribe(newClipboard, mp3);
            if (status != 200)
            {
                break;
            }
            std::ofstream ofs{ "teste.mp3", std::ios::binary | std::ios_base::trunc };
            ofs << mp3;
            ofs.flush();
            ofs.close();

            MCIERROR err1 = mciSendStringA("open teste.mp3 type mpegvideo alias myaudiofileplayht", NULL, 0, 0);
            MCIERROR err2 = mciSendStringA("play myaudiofileplayht", NULL, 0, 0);
            
            break;
        }
        default:
            break;
        }
        break;
    }
    default:
        break;
    }

    return DefWindowProcA(hWnd, uMsg, wParam, lParam);
}

Engine::Engine(HINSTANCE hInst)
{
    messageWindow_.create(hInst, this, MessageWndProc);
    loginWindow_.create(hInst, this, GUIWndProc);
}

int Engine::login(const std::string &email, const std::string &password)
{
    return client_.play().login(email, password);
}


int Engine::loop(int cmdshow)
{
    loginWindow_.show(cmdshow);
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


LoginWindow& Engine::loginWindow()
{
    return loginWindow_;
}
