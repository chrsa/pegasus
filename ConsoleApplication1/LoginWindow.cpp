#include "LoginWindow.h"
#include "Unicode.h"

LoginWindow::LoginWindow(void (*callback)(void*), void* obj, HINSTANCE hInst)
: callback_{ callback }
, callbackObj_{ obj }
, hInst_{ hInst }
{
    hbrBkgnd_ = CreateSolidBrush(RGB(255, 255, 255));

    create(hInst, this, GUIWndProc);
}

const std::string& LoginWindow::getUsername()
{
    return userbox_;
}

const std::string& LoginWindow::getPassword()
{
    return passwordbox_;
}

LRESULT CALLBACK LoginWindow::GUIWndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
    case WM_CTLCOLORSTATIC:
    {
        LoginWindow* loginWindow = reinterpret_cast<LoginWindow*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));

        HDC hdcStatic = (HDC)wParam;
        SetTextColor(hdcStatic, RGB(0, 0, 0));
        SetBkColor(hdcStatic, RGB(255, 255, 255));

        return (INT_PTR)loginWindow->hbrBkgnd_;
    }
    case WM_ERASEBKGND:
    {
        LoginWindow* loginWindow = reinterpret_cast<LoginWindow*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));

        HBRUSH brush = CreateSolidBrush(RGB(255, 255, 255));
        SetClassLongPtr(loginWindow->window_, GCLP_HBRBACKGROUND, (LONG_PTR)brush);
        DeleteObject(brush);
        break;
    }
    case WM_CLOSE:
    {
        LoginWindow* loginWindow = reinterpret_cast<LoginWindow*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));
        DestroyWindow(hWnd);
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

        LoginWindow* loginWindow = reinterpret_cast<LoginWindow*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));
        loginWindow->setup(hWnd);

        break;
    }
    case WM_PAINT:
    {
        LoginWindow* loginWindow = reinterpret_cast<LoginWindow*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));
        PAINTSTRUCT ps;
        HDC hdcDestination = BeginPaint(hWnd, &ps);
        BitBlt(hdcDestination, 0, 0, 500, 500, loginWindow->hdcSource_, 0, 0, SRCCOPY);

        EndPaint(hWnd, &ps);

        break;
    }
    case WM_COMMAND:
    {
        LoginWindow* loginWindow = reinterpret_cast<LoginWindow*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));

        if (HIWORD(wParam) == BN_CLICKED) {

            std::wstring userStr;
            std::wstring passStr;

            int userLen = GetWindowTextLength(loginWindow->userHandler());
            userStr.resize(2 * (userLen)+1);
            GetWindowText(loginWindow->userHandler(), &userStr[0], 2 * (userLen)+1);
            userStr.resize(wcslen(&userStr[0]));

            int passLen = GetWindowTextLength(loginWindow->passHandler());
            passStr.resize(2 * (passLen)+1);
            GetWindowText(loginWindow->passHandler(), &passStr[0], 2 * (passLen)+1);
            passStr.resize(wcslen(&passStr[0]));

            loginWindow->userbox_ = Unicode{}.utf8_encode(userStr);
            loginWindow->passwordbox_ = Unicode{}.utf8_encode(passStr);

            loginWindow->Invoke();
        }
        break;
    }
    default:
        return DefWindowProc(hWnd, uMsg, wParam, lParam);
    }

    return 0;
}

void LoginWindow::create(HINSTANCE hInst, LPVOID data, WNDPROC proc)
{
    const wchar_t* classname = L"LoginWindow";

    WNDCLASSEX wx;
    ZeroMemory(&wx, sizeof(WNDCLASSEX));
    wx.cbSize = sizeof(WNDCLASSEX);
    wx.lpfnWndProc = proc;        // function which will handle messages
    wx.hInstance = hInst;
    wx.lpszClassName = classname;
    ATOM atom = RegisterClassEx(&wx);

  
    int width = 400; 
    int height = 300;

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
        data        // Additional application data
    );
}



void LoginWindow::setup(HWND hwnd)
{
    user_ = CreateWindow(L"Edit", NULL, WS_CHILD | WS_VISIBLE | WS_BORDER | WS_TABSTOP,
        130, 100, 220, 20, hwnd, (HMENU)NULL, NULL, NULL);

    CreateWindow(L"Static", L"Username", WS_CHILD | WS_VISIBLE ,
        50, 100, 80, 20, hwnd, (HMENU)NULL, NULL, NULL);

    pass_ = CreateWindow(L"Edit", NULL, WS_CHILD | WS_VISIBLE | WS_BORDER | ES_PASSWORD | WS_TABSTOP,
        130, 150, 220, 20, hwnd, (HMENU)NULL, NULL, NULL);

    CreateWindow(L"Static", L"Password", WS_CHILD | WS_VISIBLE ,
        50, 150, 80, 20, hwnd, (HMENU)NULL, NULL, NULL);

    ok_ = CreateWindow(L"button", L"Ok", WS_VISIBLE | WS_CHILD | WS_TABSTOP,
        160, 200, 80, 25, hwnd, (HMENU)NULL, NULL, NULL);

  
    
    logo_ = (HBITMAP)LoadImage(hInst_, L"logo.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
    hdcSource_ = CreateCompatibleDC(GetDC(0));
    SelectObject(hdcSource_, logo_);

}

HWND LoginWindow::windowHandler()
{
    return window_;
}

void LoginWindow::show()
{
    ShowWindow(window_, SW_SHOWNORMAL);
}

void LoginWindow::hide()
{
    ShowWindow(window_, SW_HIDE);
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

void LoginWindow::Invoke()
{
    callback_(callbackObj_);
}
