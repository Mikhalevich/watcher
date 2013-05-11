#ifndef TCPSERVER_H
#define TCPSERVER_H

#include <QTcpServer>
#include <QThread>
#include <QMutex>

#include "singletonthread.h"
//#include "databasethread.h"
#include "databasemanager.h"
#include "picturetimer.h"

namespace network
{
    class TcpServer : public QTcpServer
    {
        Q_OBJECT
    public:
        TcpServer();

    protected:
        virtual void incomingConnection (qintptr socketDescriptor);
    };
} // network

#endif // TCPSERVER_H
