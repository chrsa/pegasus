#include "Clipboard.h"
#include <string>

#include <Windows.h>

std::string Clipboard::read()
{
    OpenClipboard(nullptr);
    HANDLE hData = GetClipboardData(CF_TEXT);

    char* pszText = static_cast<char*>(GlobalLock(hData));
    std::string text(pszText);

    GlobalUnlock(hData);
    CloseClipboard();

    return text;
}
