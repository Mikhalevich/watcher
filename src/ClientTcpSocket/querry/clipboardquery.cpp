
#include "clipboardquery.h"

namespace clientsocket
{
    namespace networkquery
    {
        ClipboardQuery::ClipboardQuery()
        {
        }

        void ClipboardQuery::execute(QTcpSocket* socket)
        {
            QDataStream in(socket);

            in >> text_;

            emit sendData(responcedata::ClipboardData(text_));
        }
    } // networkquery
} // clientsocket