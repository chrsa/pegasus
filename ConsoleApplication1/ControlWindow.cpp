#include "ControlWindow.h"
#include <Windows.h>
#include "Unicode.h"
#include <Commctrl.h>

ControlWindow::ControlWindow(void (*callback)(void*), void* obj, HINSTANCE hInst)
	: callback_{ callback }
	, callbackObj_{ obj }
    , hInst_{ hInst }
    , source_{ SourceType::TEXT }
    , checkedPlayAutomatically_{true}
    , sourceRadioChanged_{false}
    , playAutomaticallyChanged_{false}
    , newAudioCommand_{false}
    , audioCommand_{AudioCommand::PLAY}
    , newTextCommand_{false}
{
    hbrBkgnd_ = CreateSolidBrush(RGB(255, 255, 255));
}

LRESULT CALLBACK ControlWindow::GUIWndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
    case WM_CTLCOLORSTATIC:
    {
        ControlWindow* controlWindow = reinterpret_cast<ControlWindow*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));

        HDC hdcStatic = (HDC)wParam;
        SetTextColor(hdcStatic, RGB(0, 0, 0));
        SetBkColor(hdcStatic, RGB(255, 255, 255));

        return (INT_PTR)controlWindow->hbrBkgnd_;
    }
    case WM_CLOSE:
    {
        DestroyWindow(hWnd);
        break;
    }
    case WM_ERASEBKGND:
    {
        ControlWindow* controlWindow = reinterpret_cast<ControlWindow*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));

        HBRUSH brush = CreateSolidBrush(RGB(255, 255, 255));
        SetClassLongPtr(controlWindow->window_, GCLP_HBRBACKGROUND, (LONG_PTR)brush);
        DeleteObject(brush);
        break;
    }
    case WM_DESTROY:
    {
        PostQuitMessage(0);
        break;
    }
    case WM_CREATE:
    {
        CREATESTRUCT* pCreate = reinterpret_cast<CREATESTRUCT*>(lParam);
        SetWindowLongPtr(hWnd, GWLP_USERDATA, (LONG_PTR)pCreate->lpCreateParams);

        ControlWindow* controlWindow = reinterpret_cast<ControlWindow*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));

        CreateWindowEx(WS_EX_WINDOWEDGE,
            L"BUTTON",
            L"Source",
            WS_VISIBLE | WS_CHILD | BS_GROUPBOX,
            10, 10,
            250, 100,
            hWnd,
            (HMENU)NULL,
            controlWindow->hInst_, NULL);
        CreateWindowEx(WS_EX_WINDOWEDGE,
            L"BUTTON",
            L"Clipboard",
            WS_VISIBLE | WS_CHILD | BS_AUTORADIOBUTTON | WS_GROUP, 
            20, 40,
            100, 20,
            hWnd, 
            (HMENU)CLICK_RADIO_CLIPBOARD_ID,
            controlWindow->hInst_, NULL);
        controlWindow->textButton_ = CreateWindowEx(WS_EX_WINDOWEDGE,
            L"BUTTON",
            L"Text",
            WS_VISIBLE | WS_CHILD | BS_AUTORADIOBUTTON, 
            20, 60,
            100, 20,
            hWnd,
            (HMENU)CLICK_RADIO_TEXT_ID,
            controlWindow->hInst_, NULL);

        CreateWindowEx(WS_EX_WINDOWEDGE,
            L"BUTTON",
            L"Voice",
            WS_VISIBLE | WS_CHILD | BS_GROUPBOX,
            10, 110,
            250, 150,
            hWnd,
            (HMENU)NULL,
            controlWindow->hInst_, NULL);

        controlWindow->comboIdiom_ = CreateWindowEx(0, WC_COMBOBOX, L"",
            CBS_DROPDOWN | WS_CHILD | WS_VISIBLE,
            20, 150, 200, 100, hWnd, (HMENU)LANGUAGE_ID, controlWindow->hInst_,
            NULL);
        controlWindow->comboGenre_ = CreateWindowEx(WS_EX_WINDOWEDGE, WC_COMBOBOX, L"",
            CBS_DROPDOWN | CBS_HASSTRINGS | WS_CHILD | WS_OVERLAPPED | WS_VISIBLE,
            20, 180, 200, 100, hWnd, (HMENU)GENRE_ID, controlWindow->hInst_,
            NULL);
        controlWindow->comboVoiceType_ = CreateWindowEx(WS_EX_WINDOWEDGE, WC_COMBOBOX, L"",
            CBS_DROPDOWN | CBS_HASSTRINGS | WS_CHILD | WS_OVERLAPPED | WS_VISIBLE,
            20, 210, 200, 100, hWnd, (HMENU)VOICE_ID, controlWindow->hInst_,
            NULL);
        
        for (const auto elementI : controlWindow->voiceDb_.Voices())
        {
            SendMessage(controlWindow->comboIdiom_, (UINT)CB_ADDSTRING, (WPARAM)0, (LPARAM)Unicode{}.utf8_decode(elementI.first).c_str());
        }
        SendMessage(controlWindow->comboIdiom_, CB_SETCURSEL, (WPARAM)0, (LPARAM)0);
      
        for (const auto elementI : controlWindow->voiceDb_.Voices().begin()->second)
        {
            SendMessage(controlWindow->comboGenre_, (UINT)CB_ADDSTRING, (WPARAM)0, (LPARAM)Unicode {}.utf8_decode(elementI.first).c_str());
        }
        SendMessage(controlWindow->comboGenre_, CB_SETCURSEL, (WPARAM)0, (LPARAM)0);

        for (const auto elementI : controlWindow->voiceDb_.Voices().begin()->second.begin()->second)
        {
            SendMessage(controlWindow->comboVoiceType_, (UINT)CB_ADDSTRING, (WPARAM)0, (LPARAM)Unicode {}.utf8_decode(elementI.first).c_str());
        }
        SendMessage(controlWindow->comboVoiceType_, CB_SETCURSEL, (WPARAM)0, (LPARAM)0);

        controlWindow->voice_ = controlWindow->voiceDb_.Voices().begin()->second.begin()->second.begin()->second;
        

        CreateWindowEx(WS_EX_WINDOWEDGE,
            L"BUTTON",
            L"Play",
            WS_VISIBLE | WS_CHILD | BS_GROUPBOX,
            300, 90,
            250, 170,
            hWnd,
            (HMENU)NULL,
            controlWindow->hInst_, NULL);
        controlWindow->playAutomaticallyButton_ = CreateWindowEx(WS_EX_WINDOWEDGE,
            L"BUTTON",
            L"Play automatically",
            WS_VISIBLE | WS_CHILD | BS_AUTOCHECKBOX | WS_GROUP,  
            310, 120,
            150, 20,
            hWnd,
            (HMENU)AUTOMATIC_ID,
            controlWindow->hInst_, NULL);

        SendMessage(controlWindow->textButton_, BM_SETCHECK, BST_CHECKED, 1);
        SendMessage(controlWindow->playAutomaticallyButton_, BM_SETCHECK, BST_CHECKED, 1);

        controlWindow->playButton_ = CreateWindow(L"button", L"Play", WS_VISIBLE | WS_CHILD | WS_TABSTOP,
            310, 150, 80, 25, hWnd, (HMENU)AUDIO_PLAY_ID, NULL, NULL);

        controlWindow->stopButton_ = CreateWindow(L"button", L"Stop", WS_VISIBLE | WS_CHILD | WS_TABSTOP,
            310, 185, 80, 25, hWnd, (HMENU)AUDIO_STOP_ID, NULL, NULL);

        controlWindow->saveAsButton_ = CreateWindow(L"button", L"Save...", WS_VISIBLE | WS_CHILD | WS_TABSTOP,
            310, 220, 80, 25, hWnd, (HMENU)SAVE_AS_ID, NULL, NULL);

        controlWindow->HideMp3Buttons();

        CreateWindowEx(WS_EX_WINDOWEDGE,
            L"BUTTON",
            L"Text",
            WS_VISIBLE | WS_CHILD | BS_GROUPBOX,
            10, 270,
            540, 220,
            hWnd,
            (HMENU)NULL,
            controlWindow->hInst_, NULL);


        controlWindow->Text_ = CreateWindowEx(
            0,
            L"EDIT",
            NULL,
            WS_CHILD | WS_BORDER | WS_VISIBLE | WS_VSCROLL | ES_LEFT | ES_MULTILINE | ES_AUTOVSCROLL | ES_WANTRETURN,
            20, 290, 520, 150,
            hWnd,
            (HMENU)NULL,
            controlWindow->hInst_,
            NULL);

        controlWindow->OkTextButton_ = CreateWindow(L"button", L"Ok", WS_VISIBLE | WS_CHILD | WS_TABSTOP,
            220, 448, 80, 25, hWnd, (HMENU)OK_TEXT_ID, NULL, NULL);

        break;
    }
    case WM_PAINT:
    {
        ControlWindow* controlWindow = reinterpret_cast<ControlWindow*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));
        PAINTSTRUCT ps;
        HDC hdcDestination = BeginPaint(hWnd, &ps);
        BitBlt(hdcDestination, 300, 0, 500, 500, controlWindow->hdcSource_, 0, 0, SRCCOPY);

        EndPaint(hWnd, &ps);

        break;
    }
    case WM_COMMAND:
    {
        ControlWindow* controlWindow = reinterpret_cast<ControlWindow*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));

        int controlId = wParam & 0xFFFF;

        switch (controlId)
        {
        case CLICK_RADIO_CLIPBOARD_ID:
        {
            controlWindow->clickRadio(SourceType::CLIPBOARD);
            EnableWindow(controlWindow->OkTextButton_, false);
            break;
        }
        case CLICK_RADIO_TEXT_ID:
        {
            controlWindow->clickRadio(SourceType::TEXT);
            EnableWindow(controlWindow->OkTextButton_, true);
            break;
        }
        case AUTOMATIC_ID:
        {
            controlWindow->clickPlayAutomatically(IsDlgButtonChecked(hWnd, AUTOMATIC_ID));
            break;
        }
        case AUDIO_PLAY_ID:
        {
            controlWindow->clickAudioPlayCommand(AudioCommand::PLAY);
            break;
        }
        case AUDIO_STOP_ID:
        {
            controlWindow->clickAudioPlayCommand(AudioCommand::STOP);
            break;
        }
        case SAVE_AS_ID:
        {
            OPENFILENAME openFileName = {};
            ZeroMemory(&openFileName, sizeof(OPENFILENAME));
            openFileName.lStructSize = sizeof(OPENFILENAME);
            openFileName.hwndOwner = nullptr;
            wchar_t szFileName[MAX_PATH] = L"";
            openFileName.lpstrFilter = L"Mp3 Files (*.mp3)\0*.mp3\0All Files (*.*)\0*.*\0";
            openFileName.lpstrFile = szFileName;
            openFileName.nMaxFile = MAX_PATH;
            openFileName.Flags = OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY | OFN_NOCHANGEDIR;
            openFileName.lpstrDefExt = L"mp3";


            if (GetSaveFileName(&openFileName))
            {
                CopyFile(L"teste.mp3", szFileName, false);
            }
            break;
        }
        case OK_TEXT_ID:
        {
            std::wstring text;

            int cTxtLen = GetWindowTextLength(controlWindow->Text_);
            text.resize(2*(cTxtLen) + 1);
            GetWindowText(controlWindow->Text_, &text[0], 2 * (cTxtLen)+1);
            text.resize(wcslen(&text[0]));
            controlWindow->textStr_ = Unicode{}.utf8_encode(text);
            controlWindow->clickTextCommand();

            break;
        }
        case LANGUAGE_ID:
        {
            if (HIWORD(wParam) == CBN_SELCHANGE)
            {

                int ItemIndex = SendMessage((HWND)lParam, (UINT)CB_GETCURSEL, (WPARAM)0, (LPARAM)0);
                wchar_t ListItem[256];
                SendMessage((HWND)lParam, (UINT)CB_GETLBTEXT, (WPARAM)ItemIndex, (LPARAM)ListItem);

                std::string ListItemUtf8{ Unicode{}.utf8_encode(ListItem) };

                SendDlgItemMessage(hWnd, GENRE_ID, CB_RESETCONTENT, 0, 0);
                for (const auto elementI : controlWindow->voiceDb_.Voices().at(ListItemUtf8))
                {
                    SendMessage(controlWindow->comboGenre_, (UINT)CB_ADDSTRING, (WPARAM)0, (LPARAM)Unicode {}.utf8_decode(elementI.first).c_str());
                }
                SendMessage(controlWindow->comboGenre_, CB_SETCURSEL, (WPARAM)0, (LPARAM)0);

                SendDlgItemMessage(hWnd, VOICE_ID, CB_RESETCONTENT, 0, 0);
                for (const auto elementI : controlWindow->voiceDb_.Voices().at(ListItemUtf8).begin()->second)
                {
                    SendMessage(controlWindow->comboVoiceType_, (UINT)CB_ADDSTRING, (WPARAM)0, (LPARAM)Unicode {}.utf8_decode(elementI.first).c_str());
                }
                SendMessage(controlWindow->comboVoiceType_, CB_SETCURSEL, (WPARAM)0, (LPARAM)0);

                controlWindow->voice_ = controlWindow->voiceDb_.Voices().at(ListItemUtf8).begin()->second.begin()->second;
                    
            }
            break;

        }
        case GENRE_ID:
        {
            if (HIWORD(wParam) == CBN_SELCHANGE)
            {
                int ItemIndex = SendMessage(controlWindow->comboIdiom_, (UINT)CB_GETCURSEL, (WPARAM)0, (LPARAM)0);
                wchar_t Idiom[256];
                SendMessage((HWND)controlWindow->comboIdiom_, (UINT)CB_GETLBTEXT, (WPARAM)ItemIndex, (LPARAM)Idiom);
                std::string idiomUtf8{ Unicode{}.utf8_encode(Idiom) };


                ItemIndex = SendMessage((HWND)lParam, (UINT)CB_GETCURSEL, (WPARAM)0, (LPARAM)0);
                wchar_t Genre[256];
                SendMessage((HWND)lParam, (UINT)CB_GETLBTEXT, (WPARAM)ItemIndex, (LPARAM)Genre);
                std::string genreUtf8{ Unicode{}.utf8_encode(Genre) };

                SendDlgItemMessage(hWnd, VOICE_ID, CB_RESETCONTENT, 0, 0);
                for (const auto elementI : controlWindow->voiceDb_.Voices().at(idiomUtf8).at(genreUtf8))
                {
                    SendMessage(controlWindow->comboVoiceType_, (UINT)CB_ADDSTRING, (WPARAM)0, (LPARAM)Unicode {}.utf8_decode(elementI.first).c_str());
                }
                SendMessage(controlWindow->comboVoiceType_, CB_SETCURSEL, (WPARAM)0, (LPARAM)0);

                controlWindow->voice_ = controlWindow->voiceDb_.Voices().at(idiomUtf8).at(genreUtf8).begin()->second;
            }
            break;
        }
        case VOICE_ID:
        {
            if (HIWORD(wParam) == CBN_SELCHANGE)
            {
                int ItemIndex = SendMessage(controlWindow->comboIdiom_, (UINT)CB_GETCURSEL, (WPARAM)0, (LPARAM)0);
                wchar_t Idiom[256];
                SendMessage((HWND)controlWindow->comboIdiom_, (UINT)CB_GETLBTEXT, (WPARAM)ItemIndex, (LPARAM)Idiom);
                std::string idiomUtf8{ Unicode{}.utf8_encode(Idiom) };

                ItemIndex = SendMessage((HWND)controlWindow->comboGenre_, (UINT)CB_GETCURSEL, (WPARAM)0, (LPARAM)0);
                wchar_t Genre[256];
                SendMessage((HWND)controlWindow->comboGenre_, (UINT)CB_GETLBTEXT, (WPARAM)ItemIndex, (LPARAM)Genre);
                std::string genreUtf8{ Unicode{}.utf8_encode(Genre) };

                ItemIndex = SendMessage((HWND)controlWindow->comboVoiceType_, (UINT)CB_GETCURSEL, (WPARAM)0, (LPARAM)0);
                wchar_t Voice[256];
                SendMessage((HWND)controlWindow->comboVoiceType_, (UINT)CB_GETLBTEXT, (WPARAM)ItemIndex, (LPARAM)Voice);
                std::string voiceUtf8{ Unicode{}.utf8_encode(Voice) };

                controlWindow->voice_ = controlWindow->voiceDb_.Voices().at(idiomUtf8).at(genreUtf8).at(voiceUtf8);
            }
            break;
        }

        }

        break;
    }
    default:
    {
        return DefWindowProc(hWnd, uMsg, wParam, lParam);
    }
    }

    return 0;
}

void ControlWindow::ShowMp3Buttons()
{
    EnableWindow(playButton_, true);
    EnableWindow(stopButton_, true);
    EnableWindow(saveAsButton_, true);
}

void ControlWindow::HideMp3Buttons()
{
    EnableWindow(playButton_, false);
    EnableWindow(stopButton_, false);
    EnableWindow(saveAsButton_, false);
}

void ControlWindow::clickRadio(SourceType source)
{
    source_ = source;
    sourceRadioChanged_ = true;
    Invoke();
}

bool ControlWindow::sourceRadioChanged()
{
    bool sourceRadioChanged = sourceRadioChanged_;
    sourceRadioChanged_ = false;
    
    return sourceRadioChanged;
}

bool ControlWindow::PlayAutomaticallyChanged()
{
    bool playAutomaticallyChanged = playAutomaticallyChanged_;
    playAutomaticallyChanged_ = false;

    return playAutomaticallyChanged;
}

void ControlWindow::clickPlayAutomatically(bool checked)
{
    checkedPlayAutomatically_ = checked;
    playAutomaticallyChanged_ = true;
    Invoke();
}

SourceType ControlWindow::source() const
{
    return source_;
}

bool ControlWindow::isPlayAutomatically() const
{
    return checkedPlayAutomatically_;
}

void ControlWindow::Invoke()
{
    callback_(callbackObj_);
}

void ControlWindow::create(HINSTANCE hInst)
{
    const wchar_t* classname = L"ControlWindow";

    WNDCLASSEX wx;
    ZeroMemory(&wx, sizeof(WNDCLASSEX));
    wx.cbSize = sizeof(WNDCLASSEX);
    wx.lpfnWndProc = GUIWndProc;        // function which will handle messages
    wx.hInstance = hInst;
    wx.lpszClassName = classname;
    ATOM atom = RegisterClassEx(&wx);

    int width = 575;
    int height = 540;

    int screenX = GetSystemMetrics(SM_CXSCREEN);
    int screenY = GetSystemMetrics(SM_CYSCREEN);

    int xCtr = (screenX / 2) - (width / 2);
    int yCtr = (screenY / 2) - (height / 2);

    window_ = CreateWindowEx(
        0,
        classname,
        L"Pegasus",
        WS_OVERLAPPEDWINDOW,
        xCtr, yCtr, width, height,
        NULL,       // Parent window    
        NULL,       // Menu
        hInst,      // Instance handle
        this        // Additional application data
    );

    logo_ = (HBITMAP)LoadImage(hInst_, L"logo.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
    hdcSource_ = CreateCompatibleDC(GetDC(0));
    SelectObject(hdcSource_, logo_);

}

void ControlWindow::show()
{
    ShowWindow(window_, SW_SHOWNORMAL);
}


bool ControlWindow::NewAudioCommand()
{
    bool newAudioCommand = newAudioCommand_;
    newAudioCommand_ = false;

    return newAudioCommand;
}

AudioCommand ControlWindow::ReadAudioCommand() const
{
    return audioCommand_;
}

void ControlWindow::clickAudioPlayCommand(AudioCommand command)
{
    newAudioCommand_ = true;
    audioCommand_ = command;
    Invoke();
}

void ControlWindow::clickTextCommand()
{
    newTextCommand_ = true;
    Invoke();
}

const std::string& ControlWindow::TextToSpeech() const
{
    return textStr_;
}

bool ControlWindow::NewTextToSpeechCommand()
{
    bool NewTextToSpeechCommand = newTextCommand_;
    newTextCommand_ = false;

    return NewTextToSpeechCommand;
}

const std::string& ControlWindow::Voice() const
{
    return voice_;
}
