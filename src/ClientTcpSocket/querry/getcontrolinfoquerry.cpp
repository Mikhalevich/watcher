
#include "getcontrolinfoquerry.h"

namespace clientsocket
{
    namespace networkquery
    {
        GetControlInfoQuerry::GetControlInfoQuerry()
        {
        }

        void GetControlInfoQuerry::execute(QTcpSocket* socket)
        {
            QByteArray bytes;
            QDataStream in(socket);
            in >> bytes;

            QString controlInfo(QString::fromUtf8(bytes.data()));
            controlInfo.replace(QChar(0x02), QLatin1String("\n"));
            emit sendData(responcedata::ControlInfoData(controlInfo));   
        }
    } // networkquery
} // clientsocket
