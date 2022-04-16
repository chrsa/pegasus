#include "MainWindow.h"


MainWindow::MainWindow(void (*callback)(void*), void* obj, HINSTANCE hInst)
: clipBoardChanged_{false}
, clipboardWindow_{ ClipboardNotificationCallback , this}
, callback_{ callback }
, callbackObj_{ obj }
, enabled_{false}
, controlWindow_{ callback , obj, hInst}
{

    clipboardWindow_.create(hInst);
    controlWindow_.create(hInst);
}

bool MainWindow::ClipBoardChanged()
{
    bool clipBoardChanged = clipBoardChanged_;

    clipBoardChanged_ = false;

    return clipBoardChanged;
}

void MainWindow::ClipboardNotificationCallback(void* data)
{
    MainWindow* mainWindow = reinterpret_cast<MainWindow*>(data);
    
    mainWindow->clipBoardChanged_ = true;
    mainWindow->Invoke();
}

void MainWindow::Invoke()
{
    callback_(callbackObj_);
}


void MainWindow::show()
{
    clipboardWindow_.enable();
    controlWindow_.show();
}