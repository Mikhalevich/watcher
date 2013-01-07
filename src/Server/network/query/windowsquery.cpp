
#include "windowsquery.h"

namespace network
{
    namespace networkquery
    {
        WindowsQuery::WindowsQuery()
        {
        }

        void WindowsQuery::execute()
        {
            /*
            Library2::Conteiner windows;
            Library2::findAllWindows(windows);

            // write all windows to the socket
            //QByteArray bytes;

            QDataStream out(socket);

            // each string consist of window name and window class with 0x01 character as separator
            for (Library2::Conteiner::const_iterator it = windows.begin(); it != windows.end(); ++it)
            {
                QString windowName = it->first;
                QString windowClass = it->second;
                QString nameAndClass = windowName + QChar(0x01) + windowClass;

                QByteArray bytes(nameAndClass.toUtf8());

                // operation, size, data
                out << (qint8)GETALLWINDOWS << (qint32)bytes.size() << bytes;
            }

            // end of transmittion
            out << (qint8)GETALLWINDOWS << (qint32)0;
            */
        }
    } // networkquery
} // network
