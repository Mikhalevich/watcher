
#include "periodpicturesquery.h"
#include "database/query/picturequery.h"

namespace network
{
    namespace networkquery
    {
        PeriodPicturesQuery::PeriodPicturesQuery()
        {
        }

        void PeriodPicturesQuery::readData(TcpSocket *socket)
        {
            QDataStream in(socket);
            in >> firstDate_ >> secondDate_;
        }

        void PeriodPicturesQuery::execute()
        {
            database::databasequery::GetPictureQuery query(firstDate_, secondDate_);
            query.execute(this);
        }
    } // networkquery
} // network
