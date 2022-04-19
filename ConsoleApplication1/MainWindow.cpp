#include "MainWindow.h"


MainWindow::MainWindow(void (*callback)(void*), void* obj, HINSTANCE hInst)
: clipBoardChanged_{false}
, clipboardWindow_{ ClipboardNotificationCallback , this}
, callback_{ callback }
, callbackObj_{ obj }
, controlWindow_{ ControlWindowNotificationCallback , this, hInst}
, playAutomaticallyChanged_{false}
, newAudioCommand_{false}
, textToSpeechCommand_{false}
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

void MainWindow::ControlWindowNotificationCallback(void* data)
{
    MainWindow* mainWindow = reinterpret_cast<MainWindow*>(data);

    if (mainWindow->controlWindow_.sourceRadioChanged())
    {
        if (mainWindow->controlWindow_.source() == SourceType::CLIPBOARD)
        {
            mainWindow->clipboardWindow_.enable();
        }
        else if (mainWindow->controlWindow_.source() == SourceType::TEXT)
        {
            mainWindow->clipboardWindow_.disable();
        }
    }
    else if (mainWindow->controlWindow_.PlayAutomaticallyChanged())
    {
        mainWindow->playAutomaticallyChanged_ = true;
        mainWindow->Invoke();
    }
    else if (mainWindow->controlWindow_.NewAudioCommand())
    {
        mainWindow->newAudioCommand_ = true;
        mainWindow->Invoke();
    }
    else if (mainWindow->controlWindow_.NewTextToSpeechCommand())
    {
        mainWindow->textToSpeechCommand_ = true;
        mainWindow->Invoke();
    }
}


void MainWindow::HideMp3Buttons()
{
    controlWindow_.HideMp3Buttons();
}

void MainWindow::ShowMp3Buttons()
{
    controlWindow_.ShowMp3Buttons();
}

bool MainWindow::PlayAutomaticallyChanged()
{
    bool playAutomaticallyChanged = playAutomaticallyChanged_;
    playAutomaticallyChanged_ = false;

    return playAutomaticallyChanged;
}

bool MainWindow::isPlayAutomatically() const
{
    return controlWindow_.isPlayAutomatically();
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
    controlWindow_.show();
}

bool MainWindow::NewAudioCommand()
{
    bool newAudioCommand = newAudioCommand_;
    newAudioCommand_ = false;

    return newAudioCommand;
}

AudioCommand MainWindow::ReadAudioCommand() const
{
    return controlWindow_.ReadAudioCommand();
}

bool MainWindow::NewTextToSpeechCommand()
{
    bool textToSpeechCommand = textToSpeechCommand_;
    textToSpeechCommand_ = false;

    return textToSpeechCommand;
}


const std::string& MainWindow::TextToSpeech() const
{
    return controlWindow_.TextToSpeech();
}

const std::string& MainWindow::Voice() const
{
    return controlWindow_.Voice();
}
