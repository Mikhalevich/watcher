
#include "tcpserver.h"
#include "tcpsocket.h"
#include "socketthread.h"

namespace network
{
    TcpServer::TcpServer()
    {
    }

    void TcpServer::incomingConnection(qintptr socketDescriptor)
    {
        SocketThread *thread = new SocketThread(socketDescriptor, this);
        connect(thread, SIGNAL(finished()), thread, SLOT(deleteLater()));
        thread->start();
    }
} // network
