
#include "autorizationquery.h"
#include "database/query/autorizationquery.h"

namespace network
{
    namespace networkquery
    {
        AutorizationQuery::AutorizationQuery()
        {
        }

        void AutorizationQuery::readData(TcpSocket *socket)
        {
            QDataStream in(socket);

            QByteArray baUser;
            in >> baUser >> password_;

            user_ = QString::fromUtf8(baUser.data());
        }

        void AutorizationQuery::execute()
        {
            database::databasequery::AutorizationQuery query(user_, password_);
            query.execute();

            // autorization
            qint8 autorization = 0;
            QByteArray dataToWrite;

            if (query.error().isEmpty())
            {
                global::functions::writeDataToByteArray(dataToWrite, AUTORIZATION, autorization);
            }
            else
            {
                autorization = 1;
                QByteArray error = query.error().toUtf8();

                global::functions::writeDataToByteArray(dataToWrite, AUTORIZATION, autorization, error);
            }

            // notify tcpsocket about autorization
            emit notifyAutorization(autorization == 0);

            // emit signal to write data to socket
            emit writeData(dataToWrite);
        }
    } // networkquery
} // network
