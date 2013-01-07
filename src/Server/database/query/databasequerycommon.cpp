
#include "databasequerycommon.h"

namespace database
{
    namespace databasequery
    {
        namespace databasereaderimpl
        {
            DatabaseReaderImplDecorator::DatabaseReaderImplDecorator(IDatabaseReaderImpl *impl)
                : impl_(impl)
            {
            }

            void DatabaseReaderImplDecorator::execute(databasetypes::IBaseDataPtr base)
            {
                impl_->execute(base);
            }

            SyncReportReaderImpl::SyncReportReaderImpl(const Map& errors)
                : errors_(errors)
            {
            }

            void SyncReportReaderImpl::execute(databasetypes::IBaseDataPtr base)
            {
                if (base->type() == databasetypes::ExecutionReportEntity)
                {
                    databasetypes::ExecutionReportData& report = static_cast<databasetypes::ExecutionReportData&>(*base);

                    if (report)
                    {
                        // clear current error
                        error_.clear();
                    }
                    else
                    {
                        databasetypes::ExecutionReportData::ExecutionStatus st = report.status();

                        Map::iterator it = errors_.find(st);
                        if (it != errors_.end())
                        {
                            // take error
                            error_ = it.value();
                        }
                        else
                        {
                            // default errors
                            switch (st)
                            {
                            case databasetypes::ExecutionReportData::GOOD:
                                // never comes here
                                error_.clear();
                                break;

                            case databasetypes::ExecutionReportData::NOITEMS:
                                error_ = tr("No items");
                                break;

                            case databasetypes::ExecutionReportData::DUPLICATEITEMS:
                                error_ = tr("Duplicate items");
                                break;

                            case databasetypes::ExecutionReportData::BAD:
                                error_ = tr("Bad operation");
                                break;

                            default:
                                error_ = tr("Unknown error");
                                break;
                            }
                        }
                    }

                    // stop event loop
                    emit quit();
                }
                else
                {
                    Q_ASSERT("You should't be here" && false);
                }
            }

            const QString& SyncReportReaderImpl::error() const
            {
                return error_;
            }
        } // databasereaderimpl

        namespace databasereader
        {
            const QString& DatabaseReaderReport::error() const
            {
                databasereaderimpl::SyncReportReaderImpl& impl = static_cast<databasereaderimpl::SyncReportReaderImpl&>(*impl_);
                return impl.error();
            }
        } // databasereader
    } // databasequery
} // database
