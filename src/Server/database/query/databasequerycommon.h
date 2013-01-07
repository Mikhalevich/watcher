#ifndef DATABASECOMMON_H
#define DATABASECOMMON_H

#include <QMap>

#include "abstractdatabasequery.h"

namespace database
{
    namespace databasequery
    {
        namespace databasereaderimpl
        {
            class DatabaseReaderImplDecorator : public IDatabaseReaderImpl
            {
                Q_OBJECT

            public:
                DatabaseReaderImplDecorator(IDatabaseReaderImpl *impl);

                virtual void execute(databasetypes::IBaseDataPtr base);

            private:
                typedef QScopedPointer<IDatabaseReaderImpl> IDatabaseReaderImplPtr;

                IDatabaseReaderImplPtr impl_;
            };

            class SyncReportReaderImpl : public IDatabaseReaderImpl
            {
                Q_OBJECT

            public:
                typedef QMap<databasetypes::ExecutionReportData::ExecutionStatus, QString> Map;

                SyncReportReaderImpl(const Map& errors = Map());

                virtual void execute(databasetypes::IBaseDataPtr base);

                const QString& error() const;

            private:
                QString error_;
                Map errors_;
            };
        } // databasereaderimpl

        namespace databasereader
        {
            class DatabaseReaderReport : public IDatabaseReader
            {
            public:
                const QString& error() const;
            };
        } // databasereader
    } // databasequery
} // database

#endif // DATABASECOMMON_H