#ifndef SETTINGS_QUERY_H
#define SETTINGS_QUERY_H

#include "networkquery.h"

namespace network
{
    namespace networkquery
    {
        class GetSettingsQuery : public INetworkQuery
        {
        public:
            GetSettingsQuery();

            virtual void execute();
        };

        ///////////////////////////////////////////////////////////////////////////////

        class SetSettingsQuery : public INetworkQuery
        {
        public:
            SetSettingsQuery();

            virtual void readData(TcpSocket *socket);
            virtual void execute();

        private:
            qint32 port_;
            qint8 startupMode_;
            qint8 trayIcon_;
        };
    } // networkquery
} // network

#endif // SETTINGS_QUERY_H
