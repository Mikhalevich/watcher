#ifndef CLIENTTCPSOCKET_H
#define CLIENTTCPSOCKET_H

#include <QTcpSocket>
#include <QDateTime>

#include "global.h"
#include "objectfactory.h"
#include "clientsocket.h"

using namespace global::params;

namespace clientsocket
{
    namespace responcedata
    {
        class AbstractData;
    }

    namespace networkquery
    {
        class AbstractQuerry;
    }

    class ClientTcpSocket : public QTcpSocket, public IClientSocket
    {
        Q_OBJECT

    public:
        ClientTcpSocket(QObject *parent = 0);
        ClientTcpSocket(int number, QObject *parent = 0);

        void setNumber(int number)
        {
            socketNumber_ = number;
        }

        int number() const
        {
            return socketNumber_;
        }

        // common
        virtual bool isConnected();

        // public functions
        void getAllWindows();
        void getAllControls(const QString& windowName);
        void setControlText(qint16 controlIndex, qint8 controlType, const QString& newControlText);
        void getControlInfo(qint16 controlIndex, qint8 controlType);
        void runProcess(const QString& processLine);

        // take screenshot
        void getPicture();
        // get all pictures in database
        void getPictures(const QDateTime begin = QDateTime(), const QDateTime end = QDateTime());

        // timed pictures
        void pictureTimer(int value);
        void getPictureTimer();

        // email
        void mailProperties(const QString& server, const int port,
                            const QString& user, const QString& password,
                            const QString& sendFrom, const QStringList& sendTo,
                            int interval);
        void getMailProperties();

        // autorization
        void login(const QString& user, const QByteArray& password);

    signals:
        void sendData(const responcedata::AbstractData& data);
        void sendAutorization(bool success, const QString& error = QString());

    private slots:
        void readData();
        void checkAutorization(QTcpSocket *socket);

    private:
        void init();
        void fillFactory();

        ObjectFactory<Operation, networkquery::AbstractQuerry> querryFactory_;

        int socketNumber_;

        // outgoing
        qint8 operation_;
        qint32 size_;

        // incoming
        qint8 serverOperation_;
        qint32 serverSize_;
    };
} // clientsocket

#endif // CLIENTTCPSOCKET_H
