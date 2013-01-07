
#include "controlsquerry.h"

namespace clientsocket
{
    namespace networkquery
    {
        ControlsQuerry::ControlsQuerry()
        {
        }

        void ControlsQuerry::execute(QTcpSocket* socket)
        {
            QByteArray bytes;
            QDataStream in(socket);
            in >> bytes;

            QList<QByteArray> ba = bytes.split(0x01);

            Q_ASSERT(ba.size() == 2);

            // work only with two elements
            if (ba.size() > 1)
            {
                QString controlText(QString::fromUtf8(ba.at(0).data()));

                int index = allControls_.size();
                allControls_.push_back(controlText);

                QString controlClass(QString::fromUtf8(ba.at(1).data()));

                emit sendData(responcedata::ControlData(index, controlText, controlClass));
            }
        }
    } // networkquery
} // clientsocket
