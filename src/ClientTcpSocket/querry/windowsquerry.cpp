
#include "windowsquerry.h"

namespace clientsocket
{
    namespace networkquery
    {
        WindowsQuerry::WindowsQuerry()
        {
        }

        void WindowsQuerry::execute(QTcpSocket *socket)
        {
            QByteArray bytes;
            QDataStream in(socket);
            in >> bytes;

            QList<QByteArray> ba = bytes.split(0x01);

            Q_ASSERT(ba.size() == 2);

            // work only with two elements
            if (ba.size() > 1)
            {
                QString windowName(QString::fromUtf8(ba.at(0).data()));
                //allWindows_.push_back(windowName);
                QString windowClass(QString::fromUtf8(ba.at(1).data()));

                emit sendData(responcedata::WindowData(windowName, windowClass));
            }
        }
    } // networkquery
} // clientsocket
