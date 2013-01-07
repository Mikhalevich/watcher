#ifndef LIBRARY2_H
#define LIBRARY2_H

#ifdef WIN32

#include <vector>

#include <windows.h>

#include <QStringList>
#include <QVector>
#include <QPair>

#include "library2_global.h"

class LIBRARY2SHARED_EXPORT Library2 {
public:
    Library2();

    typedef QVector<QPair<QString, QString> > Conteiner;

    static void findAllWindows(Conteiner& windows);
    static void findAllControlsInWindow(const QString& window, Conteiner& controls);
    static void setControlText(const int index, const QString& text);
    static LRESULT setControlText(const int index, int message, WPARAM lParam, LPARAM wParam);

private:
    // functions
    static BOOL CALLBACK enumWindowsProc(HWND hwnd, LPARAM lParam);


    // variables
    static Conteiner windowsList_;
    static Conteiner controlsList_;
    static std::vector<HWND> controlsHandleList_;
};
#endif // WIN32

#endif // LIBRARY2_H
