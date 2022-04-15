#include "Clipboard.h"
#include <string>

#include <Windows.h>

std::string Clipboard::read()
{
    std::string text;

    if (OpenClipboard(NULL))
    {
        HANDLE hClipboardData = GetClipboardData(CF_TEXT);
        if (hClipboardData)
        {
            CHAR* pchData = (CHAR*)GlobalLock(hClipboardData);
            if (pchData)
            {
                text = pchData;
                GlobalUnlock(hClipboardData);
            }
        }
        CloseClipboard();
    }

    lastText_ = text;

    return text;
}


std::string Clipboard::last()
{
    return lastText_;
}