#ifndef MAILPROPERTIESQUERRY_H
#define MAILPROPERTIESQUERRY_H

#include "networkquery.h"

namespace network
{
    namespace networkquery
    {
        class MailPropertiesQuery : public INetworkQuery
        {
        public:
            MailPropertiesQuery();

            virtual void readData(TcpSocket *socket);
            virtual void execute();

        private:
            QString server_;
            qint32 serverPort_;
            QString user_;
            QString password_;
            QString sendFrom_;
            QStringList sendTo_;
            qint16 mailInterval_;
        };
    } // networkquery
} // network

#endif // MAILPROPERTIESQUERRY_H
