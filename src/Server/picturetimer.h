#ifndef PICTURETIMER_H
#define PICTURETIMER_H

#include <QObject>
#include <QTimer>
#include <QPixmap>
#include <QVector>
#include <QBuffer>
#include <QByteArray>
#include <QPair>

#include "databasetypes.h"

class PictureTimer : public QObject
{
    Q_OBJECT

public:
    static PictureTimer* instance()
    {
        static PictureTimer* pictureTimer;
        if (pictureTimer == 0)
        {
            pictureTimer = new PictureTimer();
        }

        return pictureTimer;
    }

    /* to receive picture from PictureSingleton */
    Q_INVOKABLE void picture(QImage image);

public slots:
    void pictureTimer(int secInterval);
    void mailProperties(const QString& server, const int port,
                        const QString& user, const QString& password,
                        const QString& sendFrom, const QStringList& sendTo,
                        int interval);

private slots:
    void savePicture();
    void sendMail();
    void sendTestMail();

private:
    PictureTimer();
    PictureTimer(const PictureTimer&);
    PictureTimer& operator=(const PictureTimer&);

    QTimer pictureTimer_;
    QTimer mailTimer_;

    // mail parameters
    database::databasetypes::MailParams mailProperties_;
};

#endif // PICTURETIMER_H
