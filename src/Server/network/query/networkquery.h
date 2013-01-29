#ifndef ABSTRACTQUERRY_H
#define ABSTRACTQUERRY_H

#include <stdexcept>

#include <QObject>

#include "../tcpsocket.h"
#include "global.h"

using namespace global::params;

namespace network
{
    namespace networkquery
    {
        class INetworkQuery : public QObject
        {
            Q_OBJECT

        public:
            INetworkQuery()
            {
            }

            virtual ~INetworkQuery()
            {
            }

            // read data from socket (if needed)
            virtual void readData(TcpSocket *socket)
            {
            }

            // all work here
            virtual void execute() = 0;

        signals:
            // signal write data to socket
            void writeData(const QByteArray& data);
        };
    } // networkquery
} // network

#endif // ABSTRACTQUERRY_H
