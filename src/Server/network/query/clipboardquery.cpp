
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
    } // networkquery
} // network
