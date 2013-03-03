
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

            in >> clipboardData_;

            emit sendData(responcedata::ClipboardData(clipboardData_));
        }
    } // networkquery
} // clientsocket