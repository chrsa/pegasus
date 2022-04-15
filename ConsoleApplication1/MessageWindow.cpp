#include "MessageWindow.h"



LRESULT CALLBACK MessageWindow::MessageWndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
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
            MessageWindow* messageOnly = reinterpret_cast<MessageWindow*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));
            messageOnly->clipBoardChanged_ = true;
            messageOnly->callbackCall();

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

void MessageWindow::create(HINSTANCE hInst)
{
    const char* classname = "MessageWindow";

    WNDCLASSEXA wx;
    ZeroMemory(&wx, sizeof(WNDCLASSEXA));
    wx.cbSize = sizeof(WNDCLASSEXA);
    wx.lpfnWndProc = MessageWndProc;        // function which will handle messages
    wx.hInstance = hInst;
    wx.lpszClassName = classname;
    ATOM atom = RegisterClassExA(&wx);

    window_ = CreateWindowExA(0, classname, nullptr, 0, 0, 0, 0, 0, HWND_MESSAGE, nullptr, hInst, this);

    bool ok = AddClipboardFormatListener(window_);

}

HWND MessageWindow::handler() const
{
    return window_;
}


void MessageWindow::show(int cmdshow)
{
    ShowWindow(window_, cmdshow);
}



MessageWindow::MessageWindow(void (*callback)(void*), void *obj)
: callback_ { callback }
, callbackObj_{ obj }
, clipBoardChanged_{false}
{

}

void MessageWindow::callbackCall()
{
    callback_(callbackObj_);
}


bool MessageWindow::ClipBoardChanged()
{
    bool clipBoardChanged = clipBoardChanged_;

    clipBoardChanged_ = false;

    return clipBoardChanged;
}