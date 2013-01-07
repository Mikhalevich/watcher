
#include "getmailpropertiesquery.h"
#include "database/query/mailquery.h"

namespace network
{
    namespace networkquery
    {
        void GetMailPropertiesQuery::execute()
        {
            database::databasequery::GetMailPropertiesQuery query;
            query.execute();

            if (query.error().isEmpty())
            {
                transmitMailProperties(query.mailParams());
            }
            else
            {
                transmitError(query.error());
            }
        }

        void GetMailPropertiesQuery::transmitMailProperties(const database::databasetypes::MailParams &mp)
        {
            QByteArray recepients = mp.sendTo.join(QLatin1String(",")).toUtf8();

            QByteArray dataToWrite;
            global::functions::writeDataToByteArray(dataToWrite, GETMAILPROPERTIES, mp.server.toUtf8(), (qint32)mp.serverPort,
                mp.user.toUtf8(), mp.password.toUtf8(),
                mp.sendFrom.toUtf8(), recepients,
                (qint16)mp.interval);

            emit writeData(dataToWrite);
        }

        void GetMailPropertiesQuery::transmitError(const QString& error)
        {
            //QByteArray error = tr("Error while getting mail properties").toUtf8();

            QByteArray dataToWrite;
            global::functions::writeDataToByteArray(dataToWrite, EXECUTIONREPORT, error.toUtf8());

            emit writeData(dataToWrite);
        }
    } // networkquery
} // network
