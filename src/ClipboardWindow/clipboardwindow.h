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

signals:
    void textData(const QString& text);
    void imageData(const QVariant& image);

private slots:
    void clipboard();
    void setClipboard(const QString& data);

    /* read data incoming from server */
    virtual void readData(const AbstractData& text);

    /* internationalization */
    virtual void retranslateUi();
};

#endif // PLUGINEXAMPLE_H
