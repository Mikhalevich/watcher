#ifndef SETTINGSQUERY_H
#define SETTINGSQUERY_H

#include "abstractdatabasequery.h"

namespace database
{
    namespace databasequery
    {
        class StoreSettingsQuery : public IDatabaseWriter
        {
        public:
            StoreSettingsQuery(const int port, const bool trayIcon, const bool startupMode);

        protected:
            virtual databasetypes::IBaseDataPtr data();

        private:
            int port_;
            bool trayIcon_;
            bool startupMode_;
        };

        ////////////////////////////////////////////////////////////

        // class to get settings from database
        class GetSettingsQuery : public IDatabaseReader
        {
            Q_OBJECT

        public:
            const int port() const;
            bool trayIcon() const;
            bool startupMode() const;

        protected:

            virtual databasetypes::IBaseDataPtr data();

            virtual IDatabaseReaderImplPtr readerImpl(IAsyncDataReceiver *receiver);

        private:

            class SyncReader : public IDatabaseReaderImpl
            {
                friend class GetSettingsQuery;

            public:
                virtual void execute(databasetypes::IBaseDataPtr base);

            private:
                int port_;
                bool trayIcon_;
                bool startupMode_;
            };
        };
    } // databasequery
} // database

#endif // SETTINGSQUERY_H