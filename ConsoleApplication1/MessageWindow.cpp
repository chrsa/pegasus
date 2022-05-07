#include "MessageWindow.h"



LRESULT CALLBACK ClipboardMessageWindow::MessageWndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
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
        SetWindowLongPtr(hWnd, GWLP_USERDATA, (LONG_PTR)userdata);

        break;
    }
    case WM_TIMER:
    {

        switch (wParam)
        {
        case IDT_TIMER1:
        {
            KillTimer(hWnd, IDT_TIMER1);
            ClipboardMessageWindow* messageOnly = reinterpret_cast<ClipboardMessageWindow*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));
            
            if (messageOnly->enabled_)
            {
                messageOnly->clipBoardChanged_ = true;
                messageOnly->callbackCall();
            }

            break;
        }
        default:
            break;
        }
        break;
    }
    default:
        return DefWindowProc(hWnd, uMsg, wParam, lParam);
    }

    return 0;
}

void ClipboardMessageWindow::create(HINSTANCE hInst)
{
    const wchar_t* classname = L"MessageWindow";

    WNDCLASSEX wx;
    ZeroMemory(&wx, sizeof(WNDCLASSEX));
    wx.cbSize = sizeof(WNDCLASSEX);
    wx.lpfnWndProc = MessageWndProc;        // function which will handle messages
    wx.hInstance = hInst;
    wx.lpszClassName = classname;
    ATOM atom = RegisterClassEx(&wx);

    window_ = CreateWindowEx(0, classname, nullptr, 0, 0, 0, 0, 0, HWND_MESSAGE, nullptr, hInst, this);

    bool ok = AddClipboardFormatListener(window_);

}

HWND ClipboardMessageWindow::handler() const
{
    return window_;
}


void ClipboardMessageWindow::show(int cmdshow)
{
    ShowWindow(window_, cmdshow);
}

void ClipboardMessageWindow::enable()
{
    enabled_ = true;
}

void ClipboardMessageWindow::disable()
{
    enabled_ = false;
}

ClipboardMessageWindow::ClipboardMessageWindow(void (*callback)(void*), void *obj)
: callback_ { callback }
, callbackObj_{ obj }
, clipBoardChanged_{false}
, enabled_{false}
{

}

void ClipboardMessageWindow::callbackCall()
{
    callback_(callbackObj_);
}

ClipboardMessageWindow::~ClipboardMessageWindow()
{
    DestroyWindow(window_);
}
