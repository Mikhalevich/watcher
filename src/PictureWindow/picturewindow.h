#ifndef PLUGINEXAMPLE_H
#define PLUGINEXAMPLE_H

#include <QWidget>

#include "basewidget.h"
#include "propertydialog.h"

class PicturesWidget;
class QPixmap;

/* form name */
namespace Ui {
    class PluginExample;
}


/* change PluginExample to you class name */
class PluginExample : public BaseWidget
{
    Q_OBJECT
    Q_INTERFACES(BaseWidget)

public:

    explicit PluginExample(QWidget *parent = 0);
    ~PluginExample();

    /* create object of itself */
    virtual BaseWidget* clone() const
    {
        return new PluginExample();
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

private slots:
    void on_takePictureButton_clicked();
    void on_clearPicturesButton_clicked();
    void on_savePictureButton_clicked();
    void on_historyButton_clicked();
    void on_pbSizeOneToOne_clicked();
    void on_pbSizeWidth_clicked();
    void on_pbSettings_clicked();

    // which radio button pressed
    void historyAction(bool check);
    // change picture size
    void onChangePictureSize(int index);

    void changePicture(const QPixmap& pixmap);

private:
    // set picture to scrollArea with scale factor
    void setPicture(const QPixmap& pixmap);

    /* form */
    Ui::PluginExample *ui;
    PicturesWidget* picturesWidget_;
    PropertyDialog dialog_;

    // actions
    QAction *savePictureAction_;
};

#endif // PLUGINEXAMPLE_H
