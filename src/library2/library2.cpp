
#ifdef WIN32

#include <QStringList>

#include "library2.h"

// variables
Library2::Conteiner Library2::windowsList_;
Library2::Conteiner Library2::controlsList_;
std::vector<HWND> Library2::controlsHandleList_;


// methods
Library2::Library2()
{
}

void Library2::findAllWindows(Conteiner& windows)
{
    windowsList_.clear();

    int windowCount = 0;
    HWND winList[65536];

    HWND win = GetWindow(GetDesktopWindow(), GW_CHILD);

    while (win != 0)
    {
        if (IsWindowVisible(win))
        {
            winList[windowCount++] = win;
        }

        win = GetWindow(win, GW_HWNDNEXT);
    }

    for (int i = 0; i < windowCount; ++i)
    {
        TCHAR windowName[256];
        TCHAR windowClass[256];
        HWND hwnd = winList[i];

        SendMessage(hwnd, WM_GETTEXT, 256, (LPARAM)windowName);
        GetClassName(hwnd, windowClass, 256);

        QPair<QString, QString> windowNameAndClass(QString::fromWCharArray(windowName),
                                                   QString::fromWCharArray(windowClass));
        windowsList_.push_back(windowNameAndClass);
    }

    windows = windowsList_;
}

void Library2::findAllControlsInWindow(const QString& window, Conteiner& controls)
{
    controlsHandleList_.clear();
    controlsList_.clear();

    TCHAR windowName[256];
    window.toWCharArray(windowName);
    windowName[window.length()] = L'\0';

    HWND hwnd = FindWindow(NULL, windowName);
    if (hwnd)
    {
        EnumChildWindows(hwnd, enumWindowsProc, 0);
    }

    controls = controlsList_;
}

void Library2::setControlText(const int index, const QString& text)
{
    TCHAR contentField[256];

    text.toWCharArray(contentField);
    contentField[text.length()] = L'\0';

    HWND hwnd = controlsHandleList_.at(index);
    SendMessage(hwnd, WM_SETTEXT, 0, (LPARAM)contentField);
    InvalidateRect(hwnd, 0, TRUE);
}

LRESULT Library2::setControlText(const int index, int message, WPARAM wParam, LPARAM lParam)
{
    HWND hwnd = controlsHandleList_.at(index);
    LRESULT retValue = SendMessage(hwnd, message, wParam, lParam);
    InvalidateRect(hwnd, 0, TRUE);
    return retValue;
}

BOOL CALLBACK Library2::enumWindowsProc(HWND hwnd, LPARAM lParam)
{
    TCHAR windowText[256];
    TCHAR windowClass[256];
    SendMessage(hwnd, WM_GETTEXT, 256, (LPARAM)windowText);
    //GetWindowText(hwnd, str, 256);
    GetClassName(hwnd, windowClass, 256);

    QPair<QString, QString> winTextAndClass(QString::fromWCharArray(windowText),
                                            QString::fromWCharArray(windowClass));
    controlsList_.push_back(winTextAndClass);
    controlsHandleList_.push_back(hwnd);

    return TRUE;
}

#endif // WIN32
