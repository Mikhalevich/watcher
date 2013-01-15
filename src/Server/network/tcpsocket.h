#ifndef TCPSOCKET_H
#define TCPSOCKET_H

#include <QTcpSocket>
#include <QSharedPointer>
#include <QTimer>

#include <global.h>
#include "library2.h"
#include "querythread.h"
#include "helper/objectfactory.h"

class QPixmap;

using namespace global::params;

namespace network
{
    class TcpSocket : public QTcpSocket
    {
        Q_OBJECT

    public:
        TcpSocket(QObject *parent = 0);

    private slots:
        void readData();
        void writeData(const QByteArray& data);
        void setAutorization(bool state);

    private:
        void fillFactory();

        // executes querry in queue
        QueryThread queryThread_;

        // operation type
        // if equal zero then previous operatin compleated and wait a new command
        global::types::operation_t operation_;
        // operation data size which read from socket
        global::types::operation_size_t size_;
        // autorization flag
        bool autorization_;

        ObjectFactory<Operation, networkquery::INetworkQuery> queryFactory_;
    };
} // network

#endif // TCPSOCKET_H
