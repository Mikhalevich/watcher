
#include <QtPlugin>

#include "clipboardwindow.h"

ClipboardWindow::ClipboardWindow(QWidget *parent) :
    BaseWidget(parent)
{   
    /* retranslte all visible strings */
    retranslateUi();
}

QIcon ClipboardWindow::icon() const
{
    return QIcon(":/icons/resources/icons/scissors_32x32.png");
}

QString ClipboardWindow::text() const
{
    return tr("Clipboard Window");
}

QString ClipboardWindow::statusTip() const
{
    return tr("Clipboard Window");
}


void ClipboardWindow::retranslateUi()
{
}


void ClipboardWindow::readData(const AbstractData &data)
{
    switch (data.type())
    {
    /*case GETALLWINDOWS:
    {
        const WindowData& windowData = static_cast<const WindowData&>(data);
    }
        break;

    case GETALLCONTROLS:
    {
        const ControlData& controlData = static_cast<const ControlData&>(data);
    }
        break;

    case GETCONTROLINFO:
    {
        const ControlInfoData& controlInfoData = static_cast<const ControlInfoData&>(data);
    }
        break;
        */

    default:
        Q_ASSERT("You should never be here" && false);
    }
}

/* export plugin */
/* first parameter plugin name - see TARGET in .pro file */
/* second parameter plugin class - see class name in .h file */
Q_EXPORT_PLUGIN2(ClipboardWindow, ClipboardWindow)
