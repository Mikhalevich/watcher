#ifndef ABSTRACTQUERRY_H
#define ABSTRACTQUERRY_H

#include <QTcpSocket>
#include <QDataStream>
#include <QByteArray>

#include "global.h"
#include "responcedata.h"

using namespace global::params;

namespace clientsocket
{
    namespace networkquery
    {
        class AbstractQuerry : public QObject
        {   
            Q_OBJECT

        public:
            AbstractQuerry()
            {
            }

            virtual ~AbstractQuerry()
            {
            }

            virtual void execute(QTcpSocket* socket) = 0;

        signals:
            void sendData(const responcedata::AbstractData& data);

        protected:
            QByteArray bytes_;
        };
    } // networkquery
} // clientsocket

#endif // ABSTRACTQUERRY_H
