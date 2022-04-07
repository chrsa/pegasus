#include "LoginWindow.h"

void LoginWindow::create(HINSTANCE hInst, LPVOID data, WNDPROC proc)
{
    const char* classname = "LoginWindow";

    WNDCLASSEXA wx;
    ZeroMemory(&wx, sizeof(WNDCLASSEXA));
    wx.cbSize = sizeof(WNDCLASSEXA);
    wx.lpfnWndProc = proc;        // function which will handle messages
    wx.hInstance = hInst;
    wx.lpszClassName = classname;
    ATOM atom = RegisterClassExA(&wx);

  
    window_ = CreateWindowExA(
        0,
        classname,
        "PlayHt 1.0",
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
        NULL,       // Parent window    
        NULL,       // Menu
        hInst,      // Instance handle
        data        // Additional application data
    );
}


void LoginWindow::setup(HWND hwnd)
{
    user_ = CreateWindowA("Edit", NULL, WS_CHILD | WS_VISIBLE | WS_BORDER,
        50, 50, 250, 20, hwnd, (HMENU)1002, NULL, NULL);

    pass_ = CreateWindowA("Edit", NULL, WS_CHILD | WS_VISIBLE | WS_BORDER | ES_PASSWORD,
        50, 100, 250, 20, hwnd, (HMENU)1003, NULL, NULL);

    ok_ = CreateWindowA("button", "Ok", WS_VISIBLE | WS_CHILD,
        50, 150, 80, 25, hwnd, (HMENU)1004, NULL, NULL);

}

void LoginWindow::show(int cmdshow)
{
    ShowWindow(window_, cmdshow);
}


HWND LoginWindow::handler() const
{
    return window_;
}

HWND LoginWindow::userHandler() const
{
    return user_;
}
HWND LoginWindow::passHandler() const
{
    return pass_;
}
HWND LoginWindow::okHandler() const
{
    return ok_;
}