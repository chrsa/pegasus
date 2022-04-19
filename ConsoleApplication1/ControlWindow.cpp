#include "ControlWindow.h"
#include <Windows.h>

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
        SetWindowLongPtrA(hWnd, GWLP_USERDATA, (LONG_PTR)pCreate->lpCreateParams);

        ControlWindow* controlWindow = reinterpret_cast<ControlWindow*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));

        CreateWindowExA(WS_EX_WINDOWEDGE,
            "BUTTON",
            "Source",
            WS_VISIBLE | WS_CHILD | BS_GROUPBOX,
            10, 10,
            250, 100,
            hWnd,
            (HMENU)5000,
            controlWindow->hInst_, NULL);
        CreateWindowExA(WS_EX_WINDOWEDGE,
            "BUTTON",
            "Clipboard",
            WS_VISIBLE | WS_CHILD | BS_AUTORADIOBUTTON | WS_GROUP, 
            20, 40,
            100, 20,
            hWnd, 
            (HMENU)5001,
            controlWindow->hInst_, NULL);
        controlWindow->textButton_ = CreateWindowExA(WS_EX_WINDOWEDGE,
            "BUTTON",
            "Text",
            WS_VISIBLE | WS_CHILD | BS_AUTORADIOBUTTON, 
            20, 60,
            100, 20,
            hWnd,
            (HMENU)5002,
            controlWindow->hInst_, NULL);

        CreateWindowExA(WS_EX_WINDOWEDGE,
            "BUTTON",
            "Voice",
            WS_VISIBLE | WS_CHILD | BS_GROUPBOX,
            10, 110,
            250, 150,
            hWnd,
            (HMENU)5000,
            controlWindow->hInst_, NULL);

        controlWindow->comboIdiom_ = CreateWindowExA(0, WC_COMBOBOXA, "",
            CBS_DROPDOWN | WS_CHILD | WS_VISIBLE,
            20, 150, 200, 100, hWnd, (HMENU)5011, controlWindow->hInst_,
            NULL);
        controlWindow->comboGenre_ = CreateWindowExA(WS_EX_WINDOWEDGE, WC_COMBOBOXA, "",
            CBS_DROPDOWN | CBS_HASSTRINGS | WS_CHILD | WS_OVERLAPPED | WS_VISIBLE,
            20, 180, 200, 100, hWnd, (HMENU)5012, controlWindow->hInst_,
            NULL);
        controlWindow->comboVoiceType_ = CreateWindowExA(WS_EX_WINDOWEDGE, WC_COMBOBOXA, "",
            CBS_DROPDOWN | CBS_HASSTRINGS | WS_CHILD | WS_OVERLAPPED | WS_VISIBLE,
            20, 210, 200, 100, hWnd, (HMENU)5013, controlWindow->hInst_,
            NULL);
        
        for (const auto elementI : controlWindow->voiceDb_.Voices())
        {
            SendMessageA(controlWindow->comboIdiom_, (UINT)CB_ADDSTRING, (WPARAM)0, (LPARAM)elementI.first.c_str());
        }
        SendMessageA(controlWindow->comboIdiom_, CB_SETCURSEL, (WPARAM)0, (LPARAM)0);
      
        for (const auto elementI : controlWindow->voiceDb_.Voices().begin()->second)
        {
            SendMessageA(controlWindow->comboGenre_, (UINT)CB_ADDSTRING, (WPARAM)0, (LPARAM)elementI.first.c_str());
        }
        SendMessageA(controlWindow->comboGenre_, CB_SETCURSEL, (WPARAM)0, (LPARAM)0);

        for (const auto elementI : controlWindow->voiceDb_.Voices().begin()->second.begin()->second)
        {
            SendMessageA(controlWindow->comboVoiceType_, (UINT)CB_ADDSTRING, (WPARAM)0, (LPARAM)elementI.first.c_str());
        }
        SendMessageA(controlWindow->comboVoiceType_, CB_SETCURSEL, (WPARAM)0, (LPARAM)0);

        controlWindow->voice_ = controlWindow->voiceDb_.Voices().begin()->second.begin()->second.begin()->second;
        

        CreateWindowExA(WS_EX_WINDOWEDGE,
            "BUTTON",
            "Play",
            WS_VISIBLE | WS_CHILD | BS_GROUPBOX,
            300, 90,
            250, 170,
            hWnd,
            (HMENU)5003,
            controlWindow->hInst_, NULL);
        controlWindow->playAutomaticallyButton_ = CreateWindowExA(WS_EX_WINDOWEDGE,
            "BUTTON",
            "Play automatically",
            WS_VISIBLE | WS_CHILD | BS_AUTOCHECKBOX | WS_GROUP,  
            310, 120,
            150, 20,
            hWnd,
            (HMENU)5004,
            controlWindow->hInst_, NULL);

        SendMessage(controlWindow->textButton_, BM_SETCHECK, BST_CHECKED, 1);
        SendMessage(controlWindow->playAutomaticallyButton_, BM_SETCHECK, BST_CHECKED, 1);

        controlWindow->playButton_ = CreateWindowA("button", "Play", WS_VISIBLE | WS_CHILD | WS_TABSTOP,
            310, 150, 80, 25, hWnd, (HMENU)5005, NULL, NULL);

        controlWindow->stopButton_ = CreateWindowA("button", "Stop", WS_VISIBLE | WS_CHILD | WS_TABSTOP,
            310, 185, 80, 25, hWnd, (HMENU)5006, NULL, NULL);

        controlWindow->saveAsButton_ = CreateWindowA("button", "Save...", WS_VISIBLE | WS_CHILD | WS_TABSTOP,
            310, 220, 80, 25, hWnd, (HMENU)5007, NULL, NULL);

        controlWindow->HideMp3Buttons();

        CreateWindowExA(WS_EX_WINDOWEDGE,
            "BUTTON",
            "Text",
            WS_VISIBLE | WS_CHILD | BS_GROUPBOX,
            10, 270,
            540, 220,
            hWnd,
            (HMENU)5008,
            controlWindow->hInst_, NULL);


        controlWindow->Text_ = CreateWindowExA(
            0,
            "EDIT",
            NULL,
            WS_CHILD | WS_BORDER | WS_VISIBLE | WS_VSCROLL | ES_LEFT | ES_MULTILINE | ES_AUTOVSCROLL | ES_WANTRETURN,
            20, 290, 520, 150,
            hWnd,
            (HMENU)5009,
            controlWindow->hInst_,
            NULL);

        controlWindow->OkTextButton_ = CreateWindowA("button", "Ok", WS_VISIBLE | WS_CHILD | WS_TABSTOP,
            220, 448, 80, 25, hWnd, (HMENU)5010, NULL, NULL);

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
        case 5001:
        {
            controlWindow->clickRadio(SourceType::CLIPBOARD);
            EnableWindow(controlWindow->OkTextButton_, false);
            break;
        }
        case 5002:
        {
            controlWindow->clickRadio(SourceType::TEXT);
            EnableWindow(controlWindow->OkTextButton_, true);
            break;
        }
        case 5004:
        {
            controlWindow->clickPlayAutomatically(IsDlgButtonChecked(hWnd, 5004));
            break;
        }
        case 5005:
        {
            controlWindow->clickAudioPlayCommand(AudioCommand::PLAY);
            break;
        }
        case 5006:
        {
            controlWindow->clickAudioPlayCommand(AudioCommand::STOP);
            break;
        }
        case 5007:
        {
            OPENFILENAMEA openFileName = {};
            ZeroMemory(&openFileName, sizeof(OPENFILENAMEA));
            openFileName.lStructSize = sizeof(OPENFILENAME);
            openFileName.hwndOwner = nullptr;
            char szFileName[MAX_PATH] = "";
            openFileName.lpstrFilter = "Mp3 Files (*.mp3)\0*.mp3\0All Files (*.*)\0*.*\0";
            openFileName.lpstrFile = szFileName;
            openFileName.nMaxFile = MAX_PATH;
            openFileName.Flags = OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY | OFN_NOCHANGEDIR;
            openFileName.lpstrDefExt = "mp3";


            if (GetSaveFileNameA(&openFileName))
            {
                CopyFileA("teste.mp3", szFileName, false);
            }
            break;
        }
        case 5010:
        {
            int cTxtLen = GetWindowTextLengthA(controlWindow->Text_);
            controlWindow->textStr_.resize(cTxtLen + 1);

            int n = GetWindowTextA(controlWindow->Text_, &controlWindow->textStr_[0], cTxtLen + 1);
            controlWindow->textStr_.resize(n);
            controlWindow->clickTextCommand();

            break;
        }
        case 5011:
        {
            if (HIWORD(wParam) == CBN_SELCHANGE)
            {
                int ItemIndex = SendMessageA((HWND)lParam, (UINT)CB_GETCURSEL, (WPARAM)0, (LPARAM)0);
                char ListItem[256];
                SendMessageA((HWND)lParam, (UINT)CB_GETLBTEXT, (WPARAM)ItemIndex, (LPARAM)ListItem);

                SendDlgItemMessageA(hWnd, 5012, CB_RESETCONTENT, 0, 0);
                for (const auto elementI : controlWindow->voiceDb_.Voices().at(ListItem))
                {
                    SendMessageA(controlWindow->comboGenre_, (UINT)CB_ADDSTRING, (WPARAM)0, (LPARAM)elementI.first.c_str());
                }
                SendMessageA(controlWindow->comboGenre_, CB_SETCURSEL, (WPARAM)0, (LPARAM)0);

                SendDlgItemMessageA(hWnd, 5013, CB_RESETCONTENT, 0, 0);
                for (const auto elementI : controlWindow->voiceDb_.Voices().at(ListItem).begin()->second)
                {
                    SendMessageA(controlWindow->comboVoiceType_, (UINT)CB_ADDSTRING, (WPARAM)0, (LPARAM)elementI.first.c_str());
                }
                SendMessageA(controlWindow->comboVoiceType_, CB_SETCURSEL, (WPARAM)0, (LPARAM)0);

                controlWindow->voice_ = controlWindow->voiceDb_.Voices().at(ListItem).begin()->second.begin()->second;
                    
            }
            break;

        }
        case 5012:
        {
            if (HIWORD(wParam) == CBN_SELCHANGE)
            {
                int ItemIndex = SendMessageA(controlWindow->comboIdiom_, (UINT)CB_GETCURSEL, (WPARAM)0, (LPARAM)0);
                char Idiom[256];
                SendMessageA((HWND)controlWindow->comboIdiom_, (UINT)CB_GETLBTEXT, (WPARAM)ItemIndex, (LPARAM)Idiom);

                ItemIndex = SendMessageA((HWND)lParam, (UINT)CB_GETCURSEL, (WPARAM)0, (LPARAM)0);
                char Genre[256];
                SendMessageA((HWND)lParam, (UINT)CB_GETLBTEXT, (WPARAM)ItemIndex, (LPARAM)Genre);

                SendDlgItemMessageA(hWnd, 5013, CB_RESETCONTENT, 0, 0);
                for (const auto elementI : controlWindow->voiceDb_.Voices().at(Idiom).at(Genre))
                {
                    SendMessageA(controlWindow->comboVoiceType_, (UINT)CB_ADDSTRING, (WPARAM)0, (LPARAM)elementI.first.c_str());
                }
                SendMessageA(controlWindow->comboVoiceType_, CB_SETCURSEL, (WPARAM)0, (LPARAM)0);

                controlWindow->voice_ = controlWindow->voiceDb_.Voices().at(Idiom).at(Genre).begin()->second;
            }
            break;
        }
        case 5013:
        {
            if (HIWORD(wParam) == CBN_SELCHANGE)
            {
                int ItemIndex = SendMessageA(controlWindow->comboIdiom_, (UINT)CB_GETCURSEL, (WPARAM)0, (LPARAM)0);
                char Idiom[256];
                SendMessageA((HWND)controlWindow->comboIdiom_, (UINT)CB_GETLBTEXT, (WPARAM)ItemIndex, (LPARAM)Idiom);

                ItemIndex = SendMessageA((HWND)controlWindow->comboGenre_, (UINT)CB_GETCURSEL, (WPARAM)0, (LPARAM)0);
                char Genre[256];
                SendMessageA((HWND)controlWindow->comboGenre_, (UINT)CB_GETLBTEXT, (WPARAM)ItemIndex, (LPARAM)Genre);

                ItemIndex = SendMessageA((HWND)controlWindow->comboVoiceType_, (UINT)CB_GETCURSEL, (WPARAM)0, (LPARAM)0);
                char Voice[256];
                SendMessageA((HWND)controlWindow->comboVoiceType_, (UINT)CB_GETLBTEXT, (WPARAM)ItemIndex, (LPARAM)Voice);

                controlWindow->voice_ = controlWindow->voiceDb_.Voices().at(Idiom).at(Genre).at(Voice);
            }
            break;
        }

        }

        break;
    }
    default:
    {
        return DefWindowProcA(hWnd, uMsg, wParam, lParam);
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
    const char* classname = "ControlWindow";

    WNDCLASSEXA wx;
    ZeroMemory(&wx, sizeof(WNDCLASSEXA));
    wx.cbSize = sizeof(WNDCLASSEXA);
    wx.lpfnWndProc = GUIWndProc;        // function which will handle messages
    wx.hInstance = hInst;
    wx.lpszClassName = classname;
    ATOM atom = RegisterClassExA(&wx);

    int width = 575;
    int height = 540;

    int screenX = GetSystemMetrics(SM_CXSCREEN);
    int screenY = GetSystemMetrics(SM_CYSCREEN);

    int xCtr = (screenX / 2) - (width / 2);
    int yCtr = (screenY / 2) - (height / 2);

    window_ = CreateWindowExA(
        0,
        classname,
        "Pegasus",
        WS_OVERLAPPEDWINDOW,
        xCtr, yCtr, width, height,
        NULL,       // Parent window    
        NULL,       // Menu
        hInst,      // Instance handle
        this        // Additional application data
    );

    logo_ = (HBITMAP)LoadImageA(hInst_, "logo.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
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
