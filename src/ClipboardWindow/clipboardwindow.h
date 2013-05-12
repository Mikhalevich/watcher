#ifndef PLUGINEXAMPLE_H
#define PLUGINEXAMPLE_H

#include <QWidget>
#include <QtPlugin>
#include <QAbstractListModel>

#include "basewidget.h"

namespace clipboardmodel 
{
    class ClipboardModel : public QAbstractListModel
    {
        Q_OBJECT
        
    public:
        ClipboardModel(QObject* parent = 0);

        typedef QVariant ClipboardElement;
        typedef QVector<ClipboardElement> ClipboardData;

        void addClipboardData(const ClipboardElement& element);
        void clearClipboardData();

        virtual int rowCount(const QModelIndex& parentIndex = QModelIndex()) const;
        virtual QVariant data(const QModelIndex& index, int role) const;

    private:
        ClipboardData clipboardData_;
    };

} // clipboardmodel

class ClipboardWindow : public BaseWidget
{
    Q_OBJECT
    Q_INTERFACES(BaseWidget)
    Q_PLUGIN_METADATA(IID BaseWidget_IID)

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
    void lastClipboard();
    void clearModel();

    /* read data incoming from server */
    virtual void readData(const AbstractData& text);

    /* internationalization */
    virtual void retranslateUi();

private:
    clipboardmodel::ClipboardModel clipboardModel_;
};

#endif // PLUGINEXAMPLE_H
