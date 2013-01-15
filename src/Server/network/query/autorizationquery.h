#ifndef AUTORIZATIONQUERRY_H
#define AUTORIZATIONQUERRY_H

#include "networkquery.h"

namespace network
{
    namespace networkquery
    {
        class AutorizationQuery : public INetworkQuery
        {
            Q_OBJECT

        public:
            AutorizationQuery();

            virtual void execute();
            virtual void readData(TcpSocket *socket);

        signals:
            // notify tcpsocket about autorization
            void notifyAutorization(bool state);

        private:
            QString user_;
            QByteArray password_;
        };
    } // networkquery
} // network

#endif // AUTORIZATIONQUERRY_H
