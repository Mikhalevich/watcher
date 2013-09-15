#ifndef PLUGINEXAMPLE_H
#define PLUGINEXAMPLE_H

#include <QWidget>
#include <QtPlugin>
#include <QScopedPointer>

#include "basewidget.h"

class ClipboardWindowPrivate;

class ClipboardWindow : public BaseWidget
{
    Q_OBJECT
    Q_INTERFACES(BaseWidget)
    Q_PLUGIN_METADATA(IID BaseWidget_IID)

public:
    explicit ClipboardWindow(QWidget *parent = 0);
    virtual ~ClipboardWindow();

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
    void lastClipboard();
    void clearModel();

    /* read data incoming from server */
    virtual void readData(const AbstractData& text);

    /* internationalization */
    virtual void retranslateUi();

private:
    QScopedPointer<ClipboardWindowPrivate> d_ptr;
};

#endif // PLUGINEXAMPLE_H
