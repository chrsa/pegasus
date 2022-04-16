#include "ControlWindow.h"

ControlWindow::ControlWindow(void (*callback)(void*), void* obj, HINSTANCE hInst)
	: callback_{ callback }
	, callbackObj_{ obj }
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
    case WM_DESTROY:
    {
        PostQuitMessage(0);
        break;
    }
    default:
    {
        return DefWindowProcA(hWnd, uMsg, wParam, lParam);

    }
    }

    return 0;
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

    int width = 800;
    int height = 600;

    int screenX = GetSystemMetrics(SM_CXSCREEN);
    int screenY = GetSystemMetrics(SM_CYSCREEN);

    int xCtr = (screenX / 2) - (width / 2);
    int yCtr = (screenY / 2) - (height / 2);

    window_ = CreateWindowExA(
        0,
        classname,
        "PlayHt 1.0",
        WS_OVERLAPPEDWINDOW,
        xCtr, yCtr, width, height,
        NULL,       // Parent window    
        NULL,       // Menu
        hInst,      // Instance handle
        this        // Additional application data
    );

}

void ControlWindow::show()
{
    ShowWindow(window_, SW_SHOWNORMAL);
}
