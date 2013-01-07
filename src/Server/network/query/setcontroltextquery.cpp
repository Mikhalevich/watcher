
#include "setcontroltextquery.h"

namespace network
{
    namespace networkquery
    {
        SetControlTextQuery::SetControlTextQuery()
        {
        }

        void SetControlTextQuery::execute()
        {
            /*
            static qint16 controlIndex = -1;
            static qint8 controlType = -1;

            if (controlIndex == -1)// && controlType == -1)
            {
                if (socket->bytesAvailable() < sizeof(controlIndex) + sizeof(controlType))
                {
                    // wait when all data come
                    throw std::runtime_error("not all bytes come");
                }

                // read index and type
                QDataStream in(socket);
                in >> controlIndex;
                in >> controlType;
            }

            while (!socket->canReadLine())
            {
                // wait when all data come
                throw std::runtime_error("not all bytes come");
            }

            QByteArray bytes;
            bytes = socket->readLine();
            QString controlText = QString::fromUtf8(bytes.data());

            // remove last \n
            controlText.remove(controlText.length() - 1, 1);

            // replace all number 2 character to new line
            controlText.replace(QChar(0x02), QLatin1String("\n"));


            if (controlType == EDIT)
            {
                Library2::setControlText(controlIndex, controlText);
            }
            else if (controlType == BUTTON)
            {
                Library2::setControlText(controlIndex, BM_CLICK, 0, 0);
            }
            else if (controlType == COMBOBOX)
            {
                Library2::setControlText(controlIndex, CB_SETCURSEL , (LPARAM)controlText.toInt(), 0);
            }

            controlIndex = -1;
            controlType = -1;
            */
        }
    } // networkquery
} // network
