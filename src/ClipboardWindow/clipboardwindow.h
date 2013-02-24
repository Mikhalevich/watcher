#ifndef PLUGINEXAMPLE_H
#define PLUGINEXAMPLE_H

#include <QWidget>

#include "basewidget.h"

class ClipboardWindow : public BaseWidget
{
    Q_OBJECT
    Q_INTERFACES(BaseWidget)

public:

    explicit ClipboardWindow(QWidget *parent = 0);

    /* create object of itself */
    virtual BaseWidget* clone() const
    {
        return new ClipboardWindow();
    }

    /* icon to menu and toolbar */
    virtual QIcon icon() const;

    /* text to menu and toolbar */
    virtual QString text() const;

    /* tip to status bar */
    virtual QString statusTip() const;

public slots:
    /* read data incoming from server */
    virtual void readData(const AbstractData& data);

    /* internationalization */
    virtual void retranslateUi();
};

#endif // PLUGINEXAMPLE_H
