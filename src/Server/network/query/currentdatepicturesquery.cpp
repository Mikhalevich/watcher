
#include "currentdatepicturesquery.h"
#include "database/query/picturequery.h"

namespace network
{
    namespace networkquery
    {
        CurrentDatePicturesQuery::CurrentDatePicturesQuery()
        {
        }

        void CurrentDatePicturesQuery::readData(TcpSocket *socket)
        {
            QDataStream in(socket);
            in >> date_;
        }

        void CurrentDatePicturesQuery::execute()
        {
            database::databasequery::GetPictureQuery query(date_);
            query.execute(this);
        }
    } // networkquery
} // network
