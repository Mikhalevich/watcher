
#include <QPixmap>
#include <QByteArray>

#include "picturequerry.h"

namespace clientsocket
{
    namespace networkquery
    {
        PictureQuerry::PictureQuerry()
        {
        }

        void PictureQuerry::execute(QTcpSocket* socket)
        {
            QByteArray bytes;
            QDateTime date;
            QDataStream in(socket);
            in >> bytes >> date;

            QPixmap pix;
            bool check = pix.loadFromData(bytes, "png");
            if (check)
            {
                emit sendData(responcedata::PictureData(pix, date));
            }
            else
            {
                // ???
            }
        }
    } // networkquery
} // clientsocket
