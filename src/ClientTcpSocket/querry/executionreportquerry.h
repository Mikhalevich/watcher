#ifndef EXECUTIONREPORTQUERRY_H
#define EXECUTIONREPORTQUERRY_H

#include "abstractquerry.h"

namespace clientsocket
{
    namespace networkquery
    {
        class ExecutionReportQuerry : public AbstractQuerry
        {
        public:
            ExecutionReportQuerry();

            virtual void execute(QTcpSocket *socket);
        };
    } // networkquery
} // clientsocket

#endif // EXECUTIONREPORTQUERRY_H
