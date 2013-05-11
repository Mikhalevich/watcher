#ifndef BASEWIDGET_H
#define BASEWIDGET_H

#include <QIcon>
#include <QString>

#include "abstractwidget.h"

using namespace clientsocket::responcedata;

#define BaseWidget_IID "com.michael.PluginExample.BaseWidget"

class BaseWidget : public clientsocket::AbstractWidget
{
    Q_OBJECT

public:
    BaseWidget(QWidget *parent = 0, Qt::WindowFlags f = 0)
        : AbstractWidget(parent, f)
    {
    }

    // return a icon to menu and toolbar
    virtual QIcon icon() const
    {
        return QIcon();
    }

    // text to menu and toolbar
    virtual QString text() const
    {
        return QString();
    }

    // return tip to status bar
    virtual QString statusTip() const
    {
        return QString();
    }

    // copy/create ourself/
    virtual BaseWidget* clone() const = 0;

public slots:
    // read data from socket
    virtual void readData(const clientsocket::responcedata::AbstractData& data) = 0;

    // translate all messages
    virtual void retranslateUi() = 0;
};

Q_DECLARE_INTERFACE(BaseWidget, BaseWidget_IID)

#endif // BASEWIDGET_H
