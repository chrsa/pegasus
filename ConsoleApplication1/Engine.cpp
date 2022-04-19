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
        TextToSpeech(engine, engine->clipboard_.read());
    }
    else if (engine->mainWindow_.PlayAutomaticallyChanged())
    {
        if (engine->mainWindow_.isPlayAutomatically())
        {
            engine->player_.enableAutomatic();
        }
        else
        {
            engine->player_.disableAutomatic();
        }
    }
    else if (engine->mainWindow_.NewAudioCommand())
    {
        AudioCommand command = engine->mainWindow_.ReadAudioCommand();

        if (command == AudioCommand::PLAY)
        {
            engine->player_.stop();
            engine->player_.play();
        }
        else if (command == AudioCommand::STOP)
        {
            engine->player_.stop();
        }
    }
    else if (engine->mainWindow_.NewTextToSpeechCommand())
    {
        TextToSpeech(engine, engine->mainWindow_.TextToSpeech());
    }
   
}

void Engine::TextToSpeech(Engine *engine, const std::string &input)
{
    engine->play_.ConfigureVoice(engine->mainWindow_.Voice());
    engine->mainWindow_.HideMp3Buttons();
    engine->player_.stop();
    const std::string& TextToSpeech = input;
    const std::string& Voice = engine->mainWindow_.Voice();
    if (TextToSpeech != engine->lastText_ || Voice != engine->lastVoice_)
    {
        std::string mp3;
        int status = engine->play_.transcribe(TextToSpeech, mp3);
        if (status != 200)
        {
            return;
        }

        std::ofstream ofs{ "teste.mp3", std::ios::binary | std::ios_base::trunc };
        ofs << mp3;
        ofs.flush();
        ofs.close();
    }

    engine->lastText_ = TextToSpeech;
    engine->lastVoice_ = Voice;

    if (engine->player_.automatic())
    {
        engine->player_.play();
    }

    engine->mainWindow_.ShowMp3Buttons();
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

