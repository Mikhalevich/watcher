#ifndef CLIENTSOCKET_H
#define CLIENTSOCKET_H

#include <QDateTime>
#include <QStringList>
#include <QByteArray>

namespace clientsocket
{
    class IClientSocket
    {
    public:
        virtual ~IClientSocket()
        {
        }

        // common functions
        virtual bool isConnected() = 0;

        // public functions
        virtual void getAllWindows() = 0;
        virtual void getAllControls(const QString& windowName) = 0;
        virtual void setControlText(qint16 controlIndex, qint8 controlType, const QString& newControlText) = 0;
        virtual void getControlInfo(qint16 controlIndex, qint8 controlType) = 0;
        virtual void runProcess(const QString& processLine) = 0;

        // take screenshot
        virtual void getPicture() = 0;
        // get all pictures in database
        virtual void getPictures(const QDateTime begin = QDateTime(), const QDateTime end = QDateTime()) = 0;

        // timed pictures
        virtual void pictureTimer(int value) = 0;
        virtual void getPictureTimer() = 0;

        // email
        virtual void mailProperties(const QString& server, const int port,
                            const QString& user, const QString& password,
                            const QString& sendFrom, const QStringList& sendTo,
                            int interval) = 0;
        virtual void getMailProperties() = 0;

        // autorization
        virtual void login(const QString& user, const QByteArray& password) = 0;
    };
}

#endif // CLIENTSOCKET_H
