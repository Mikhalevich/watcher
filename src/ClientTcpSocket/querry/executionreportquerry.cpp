
#include "executionreportquerry.h"

namespace clientsocket
{
    namespace networkquery
    {
        ExecutionReportQuerry::ExecutionReportQuerry()
        {
        }

        void ExecutionReportQuerry::execute(QTcpSocket *socket)
        {
            QByteArray bytes;

            QDataStream in(socket);
            in >> bytes;

            QString error(QString::fromUtf8(bytes.data()));

            emit sendData(responcedata::ExecutionReportData(error));
        }
    } // networkquery
} // clientsocket
