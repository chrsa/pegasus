#include "MessageWindow.h"

void MessageWindow::create(HINSTANCE hInst, LPVOID data, WNDPROC proc)
{
    const char* classname = "MessageWindow";

    WNDCLASSEXA wx;
    ZeroMemory(&wx, sizeof(WNDCLASSEXA));
    wx.cbSize = sizeof(WNDCLASSEXA);
    wx.lpfnWndProc = proc;        // function which will handle messages
    wx.hInstance = hInst;
    wx.lpszClassName = classname;
    ATOM atom = RegisterClassExA(&wx);

    window_ = CreateWindowExA(0, classname, nullptr, 0, 0, 0, 0, 0, HWND_MESSAGE, nullptr, hInst, data);

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

