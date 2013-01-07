
#include "getpicturepropertiesquerry.h"

namespace clientsocket
{
    namespace networkquery
    {
        GetPicturePropertiesQuerry::GetPicturePropertiesQuerry()
        {
        }

        void GetPicturePropertiesQuerry::execute(QTcpSocket *socket)
        {
            QDataStream in(socket);
            qint16 interval;

            // interval
            in >> interval;

            emit sendData(responcedata::PicturePropertiesData(interval));
        }
    } // networkquery
} // clientsocket
