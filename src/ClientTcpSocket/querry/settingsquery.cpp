
#include "settingsquery.h"

namespace clientsocket
{
    namespace networkquery
    {
        GetSettingsQuery::GetSettingsQuery()
        {
        }

        void GetSettingsQuery::execute(QTcpSocket* socket)
        {
            QDataStream in(socket);

            in >> port_ >> startupMode_ >> trayIcon_;

            emit sendData(responcedata::SettingsData(port_, startupMode_, trayIcon_));
        }
    } // networkquery
} // clientsocket
