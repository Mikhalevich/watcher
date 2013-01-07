#ifndef PICTURESINGLETON_H
#define PICTURESINGLETON_H

#include <QMutexLocker>
#include <QPixmap>
#include <QApplication>
#include <QDesktopWidget>

/* class that want receive pictures should have 
* invoked method or slot "picture" with signature 
* Q_INVOKABLE void picture(QImage)
*/

class PictureSingleton : public QObject
{
    Q_OBJECT

public:
    static PictureSingleton* instance()
    {
        if (instance_ == 0)
        {
            instance_ = new PictureSingleton();
        }
        return instance_;
    }

public slots:
    void takePicture(QObject* sender)
    {
        QMutexLocker lock(&mutex_);
        QPixmap pix = QPixmap::grabWindow(widget_->winId());

        // return picture to the caller
        QMetaObject::invokeMethod(sender, "picture", Qt::QueuedConnection, Q_ARG(QImage, pix.toImage()));
    }

private:
    PictureSingleton()
        : widget_(QApplication::desktop())
    {
    }

    PictureSingleton(const PictureSingleton& pic);
    PictureSingleton& operator=(const PictureSingleton& pic);

    static PictureSingleton *instance_;

    QMutex mutex_;
    QDesktopWidget *widget_;
};

#endif // PICTURESINGLETON_H
