
#include "settingsquery.h"
#include "commonfunctions.h"
#include "database/query/settingsquery.h"

namespace network
{
    namespace networkquery
    {
        GetSettingsQuery::GetSettingsQuery()
        {
        }

        void GetSettingsQuery::execute()
        {
            database::databasequery::GetSettingsQuery query;
            query.execute();

            QByteArray dataToWrite;

            qint32 port = query.port();
            // 1 - true, 0 - false
            qint8 startupMode = query.startupMode() ? 1 : 0;
            qint8 trayIcon = query.trayIcon() ? 1 : 0;

            global::functions::writeDataToByteArray(dataToWrite, GETSETTINGS, port, startupMode, trayIcon);
            

            // emit signal to write data to socket
            emit writeData(dataToWrite);
        }

        ///////////////////////////////////////////////////////////////////

        SetSettingsQuery::SetSettingsQuery()
            : port_(0)
            , startupMode_(0)
            , trayIcon_(0)
        {
        }

        void SetSettingsQuery::readData(TcpSocket *socket)
        {
            QDataStream in(socket);

            in >> port_ >> startupMode_ >> trayIcon_;
        }

        void SetSettingsQuery::execute()
        {
            database::databasequery::StoreSettingsQuery query(port_, startupMode_ == 1, trayIcon_ == 1);
            query.execute();

            // change startupMode
            checkAutorunSettings(startupMode_ == 1);
        }
    } // networkquery
} // network
