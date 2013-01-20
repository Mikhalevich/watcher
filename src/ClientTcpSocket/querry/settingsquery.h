#ifndef SETTINGS_QUERY_H
#define SETTINGS_QUERY_H

#include "abstractquerry.h"

namespace clientsocket
{
    namespace networkquery
    {
        class GetSettingsQuery : public AbstractQuerry
        {
            Q_OBJECT

        public:
            GetSettingsQuery();

            virtual void execute(QTcpSocket* socket);

        private:
            qint32 port_;
            qint8 startupMode_;
            qint8 trayIcon_;
        };
    } // networkquery
} // clientsocket

#endif // SETTINGS_QUERY_H
