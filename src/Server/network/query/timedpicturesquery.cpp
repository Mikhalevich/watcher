
#include "timedpicturesquery.h"
#include "picturetimer.h"

namespace network
{
    namespace networkquery
    {
        TimedPicturesQuery::TimedPicturesQuery()
        {
        }

        void TimedPicturesQuery::readData(TcpSocket *socket)
        {
            QDataStream in(socket);
            in >> interval_;
        }

        void TimedPicturesQuery::execute()
        {
            QMetaObject::invokeMethod(PictureTimer::instance(), "pictureTimer", Qt::QueuedConnection, Q_ARG(int, (int)interval_));
        }
    } // networkquery
} // network
