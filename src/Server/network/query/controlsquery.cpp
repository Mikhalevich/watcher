
#include "controlsquery.h"

namespace network
{
    namespace networkquery
    {
        ControlsQuery::ControlsQuery()
        {
        }

        void ControlsQuery::execute()
        {
            /*
            // we must get a window name
            if (!socket->canReadLine())
            {
                // wait when all bytes come
                throw std::runtime_error("not all bytes come");
            }

            QByteArray bytes;
            bytes = socket->readLine();
            QString window(QString::fromUtf8(bytes.data()));

            // remove last \n
            window.remove(window.length() - 1, 1);

            Library2::Conteiner controls;
            Library2::findAllControlsInWindow(window, controls);

            // write all controls in window to the socket
            bytes.clear();

            QDataStream out(socket);

            // each string consist of window name and window class with 0x01 character as separator
            for (Library2::Conteiner::const_iterator it = controls.begin(); it != controls.end(); ++it)
            {
                QString controlName = it->first;
                QString controlClass = it->second;
                QString nameAndClass = controlName + QChar(0x01) + controlClass;

                bytes = nameAndClass.toUtf8();

                out << (qint8)GETALLCONTROLS << (qint32)bytes.size() << bytes;
            }

            // end of transmittion
            out << (qint8)GETALLCONTROLS << (qint32)0;
            */
        }
    } // networkquery
} // network
