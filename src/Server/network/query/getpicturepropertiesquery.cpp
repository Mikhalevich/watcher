
#include "getpicturepropertiesquery.h"
#include "database/query/picturepropertiesquery.h"

namespace network
{
    namespace networkquery
    {
        void GetPicturePropertiesQuery::execute()
        {
            database::databasequery::GetPicturePropertiesQuery query;
            query.execute();

            if (query.error().isEmpty())
            {
                transmitPictureProperties(query.interval());
            }
            else
            {
                transmitError(query.error());
            }
        }

        void GetPicturePropertiesQuery::transmitPictureProperties(const qint16 interval)
        {
            QByteArray dataToWrite;
            global::functions::writeDataToByteArray(dataToWrite, GETPICTURETIMER, interval);

            emit writeData(dataToWrite);
        }

        void GetPicturePropertiesQuery::transmitError(const QString& error)
        {
            //QByteArray error = tr("Error while getting picture properties").toUtf8();

            QByteArray dataToWrite;
            global::functions::writeDataToByteArray(dataToWrite, EXECUTIONREPORT, error.toUtf8());

            emit writeData(dataToWrite);
        }
    } // networkquery
} // network
