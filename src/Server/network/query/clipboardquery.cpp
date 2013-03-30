
#include <QClipboard>
#include <QApplication>
#include <QMimeData>

#include "clipboardquery.h"
#include "clipboardsingleton.h"

namespace network
{
    namespace networkquery
    {

        ClipboardQuery::ClipboardQuery()
        {
        }

        void ClipboardQuery::execute()
        {
            QByteArray dataToWrite;

            // get clipboard data
            QVariant data;
            ClipboardSingleton::ClipboardType type;
            ClipboardSingleton::instance()->clipboardData(data, type);

            if (data.isValid())
            {
                global::functions::writeDataToByteArray(dataToWrite, GETCLIPBOARD, data);

                // emit signal to write data to socket
                emit writeData(dataToWrite);
            }
        }

        ////////////////////////////////////////////////////////////////////////////

        SetClipdoardQuery::SetClipdoardQuery()
        {
        }

        void SetClipdoardQuery::readData(TcpSocket *socket)
        {
            QDataStream in(socket);
            in >> clipboardData_;
        }

        void SetClipdoardQuery::execute()
        {
            if (clipboardData_.isValid())
            {
                ClipboardSingleton::ClipboardType type = ClipboardSingleton::NONE;

                switch (clipboardData_.type())
                {
                case QVariant::String:
                    type = ClipboardSingleton::TEXT;
                    break;

                case QVariant::Image:
                    type = ClipboardSingleton::IMAGE;
                    break;

                default:
                    type = ClipboardSingleton::NONE;
                    break;
                }

                ClipboardSingleton::instance()->setClipboadData(clipboardData_, type);
            }
        }

    } // networkquery
} // network
