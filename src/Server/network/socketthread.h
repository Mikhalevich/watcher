#ifndef SOCKETTHREAD_H
#define SOCKETTHREAD_H

#include <QThread>
#include <QPixmap>

#include "tcpsocket.h"
#include "databasetypes.h"

namespace network
{
    class SocketThread : public QThread
    {
        Q_OBJECT

    public:
        SocketThread(int socketDescriptor, QObject *parent = 0)
            : QThread(parent)
            , socketDescriptor_(socketDescriptor)
        {
        }

    protected:
        virtual void run()
        {
            //TcpSocket *socket = new TcpSocket(this);
            //socket->setSocketDescriptor(socketDescriptor_);
            TcpSocket socket;
            socket.setSocketDescriptor(socketDescriptor_);
            connect(&socket, SIGNAL(disconnected()), this, SLOT(quit()));

            // run event loop
            exec();
        }

        int socketDescriptor_;
    };
} // network

#endif // SOCKETTHREAD_H
