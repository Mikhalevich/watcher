
#include "settingsquery.h"

namespace database
{
    namespace databasequery
    {
        StoreSettingsQuery::StoreSettingsQuery(const int port, const bool trayIcon, const bool startupMode)
            : port_(port)
            , trayIcon_(trayIcon)
            , startupMode_(startupMode)
        {
        }

        databasetypes::IBaseDataPtr StoreSettingsQuery::data()
        {
            return databasetypes::IBaseDataPtr(new databasetypes::SettingsData(port_, trayIcon_, startupMode_));
        }

        //////////////////////////////////////////////////////////////////////////////////////////

        databasetypes::IBaseDataPtr GetSettingsQuery::data()
        {
            return databasetypes::IBaseDataPtr(new databasetypes::RequestData(databasetypes::GetSettingsEntity));
        }

        IDatabaseReaderImplPtr GetSettingsQuery::readerImpl(IAsyncDataReceiver *receiver)
        {
            return IDatabaseReaderImplPtr(new SyncReader());
        }

        const int GetSettingsQuery::port() const
        {
            const SyncReader& impl = static_cast<const SyncReader&>(*impl_);
            return impl.port_;
        }

        bool GetSettingsQuery::trayIcon() const
        {
            const SyncReader& impl = static_cast<const SyncReader&>(*impl_);
            return impl.trayIcon_;
        }

        bool GetSettingsQuery::startupMode() const
        {
            const SyncReader& impl = static_cast<const SyncReader&>(*impl_);
            return impl.startupMode_;
        }

        void GetSettingsQuery::SyncReader::execute(databasetypes::IBaseDataPtr base)
        {
            if (base->type() == databasetypes::SettingsEntity)
            {
                const databasetypes::SettingsData& settings = static_cast<databasetypes::SettingsData&>(*base);
                port_ = settings.port();
                trayIcon_ = settings.trayIcon();
                startupMode_ = settings.startupMode();
            }
            else
            {
                Q_ASSERT("You should't be here" && false);
            }

            // stop event loop
            emit quit();
        }
    } // databasequery
} // database
