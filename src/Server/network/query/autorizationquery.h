#ifndef AUTORIZATIONQUERRY_H
#define AUTORIZATIONQUERRY_H

#include "networkquery.h"

namespace network
{
    namespace networkquery
    {
        class AutorizationQuery : public INetworkQuery
        {
        public:
            AutorizationQuery();

            virtual void execute();
            virtual void readData(TcpSocket *socket);

        private:
            QString user_;
            QByteArray password_;
        };
    } // networkquery
} // network

#endif // AUTORIZATIONQUERRY_H
