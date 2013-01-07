
#include "getcontrolinfoquery.h"

namespace network
{
    namespace networkquery
    {
        GetControlInfoQuery::GetControlInfoQuery()
        {
        }

        void GetControlInfoQuery::execute()
        {
            /*
            qint16 controlIndex;
            qint8 controlType;
            if (socket->bytesAvailable() < sizeof(controlIndex) + sizeof(controlType))
            {
                // wait when all data come
                return;
            }

            // read index and type
            QDataStream in(socket);
            in >> controlIndex;
            in >> controlType;

            sendControlInfo(controlIndex, controlType, socket);
            */
        }

        void GetControlInfoQuery::sendControlInfo(const int controlIndex, const int controlType, QTcpSocket *socket)
        {
            /*
            switch (controlType)
            {
            case EDIT:
            case BUTTON:
            {
                TCHAR editText[256];
                Library2::setControlText(controlIndex, WM_GETTEXT, 256, (LPARAM)editText);

                QByteArray bytes;
                QString et = QString::fromWCharArray(editText);
                et.replace(QChar('\n'), QString(0x02));
                et.append(QLatin1String("\n"));
                bytes += et.toUtf8();

                // add FFFF string to determinate the end
                // don't need anymore
                //bytes += ENDBYTE;

                //write(bytes);
                QDataStream out(socket);
                out << (qint8)GETCONTROLINFO << (qint32)bytes.size() << bytes;

            }
                break;

            case COMBOBOX:
            {
                // get number of items in combobox
                int count = Library2::setControlText(controlIndex, CB_GETCOUNT, 0, 0);

                QDataStream out(socket);

                for (int i = 0; i < count; ++i)
                {
                    //int strLen = Library2::setControlText(controlIndex, CB_GETLBTEXTLEN, i, 0);
                    TCHAR comboItem[256];
                    Library2::setControlText(controlIndex, CB_GETLBTEXT, (WPARAM)i, (LPARAM)comboItem);
                    QString comboString = QString::fromWCharArray(comboItem);
                    comboString = comboString.prepend("%1 - ").arg(i);
                    comboString.append(QChar(0x02));

                    //write(comboString.toUtf8());
                    QByteArray bytes = comboString.toUtf8();
                    out << (qint8)GETCONTROLINFO << bytes.size() << bytes;
                }
                // don't need anymore
                //write(QByteArray("\nFFFF\n"));
            }
                break;

            default:
                Q_ASSERT("You should't be here");
                break;
            }
            */
        }
    } // networkquery
} // network
