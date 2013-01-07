
#include <QProcess>

#include "processquery.h"

namespace network
{
    namespace networkquery
    {
        ProcessQuery::ProcessQuery()
        {
        }

        void ProcessQuery::execute()
        {
            /*
            if (!socket->canReadLine())
            {
                // wait when all data come
                return;
            }

            QByteArray bytes = socket->readLine();
            QString processLine = QString::fromUtf8(bytes.data());

            QProcess::startDetached(processLine);
            */
        }
    } // networkquery
} // network
