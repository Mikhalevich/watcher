
#include "userquery.h"

namespace database
{
    namespace databasequery
    {
        StoreUserQuery::StoreUserQuery(const QString& userName, const QByteArray& password)
            : userName_(userName)
            , password_(password)
        {
        }

        databasetypes::IBaseDataPtr StoreUserQuery::data()
        {
            return databasetypes::IBaseDataPtr(new databasetypes::UserData(userName_, password_));
        }

        IDatabaseReaderImplPtr StoreUserQuery::readerImpl(IAsyncDataReceiver *receiver)
        {
            databasereaderimpl::SyncReportReaderImpl::Map map;
            map.insert(databasetypes::ExecutionReportData::DUPLICATEITEMS, tr("User with this name exists already"));

            return IDatabaseReaderImplPtr(new databasereaderimpl::SyncReportReaderImpl(map));
        }

        ////////////////////////////////////////////////////////////////////////////////////////////////

        databasetypes::IBaseDataPtr GetUserQuery::data()
        {
            return databasetypes::IBaseDataPtr(new databasetypes::RequestData(databasetypes::GetUserEntity));
        }

        IDatabaseReaderImplPtr GetUserQuery::readerImpl(IAsyncDataReceiver *receiver)
        {
            return IDatabaseReaderImplPtr(new SyncReader());
        }

        const GetUserQuery::Conteiner& GetUserQuery::users() const
        {
            SyncReader& impl = static_cast<SyncReader&>(*impl_);
            return impl.users();
        }

        const GetUserQuery::Conteiner& GetUserQuery::SyncReader::users() const
        {
            return users_;
        }

        void GetUserQuery::SyncReader::execute(databasetypes::IBaseDataPtr base)
        {
            switch (base->type())
            {
            case databasetypes::UserEntity:
                transmitUser(static_cast<databasetypes::UserData&>(*base));
                break;

            case databasetypes::EndEntity:
                transmitEnd();
                break;

            default:
                Q_ASSERT("You shouldn't be here");
                break;
            }
        }

        void GetUserQuery::SyncReader::transmitUser(databasetypes::UserData& userData)
        {
            // store user data
            QString user = userData.user();
            QByteArray password = userData.password();

            users_.push_back(QPair<QString, QByteArray>(user, password));
        }

        void GetUserQuery::SyncReader::transmitEnd()
        {
            // close event loop
            emit quit();
        }
        /////////////////////////////////////////////////////////////////////////////////////////

        DeleteUserQuery::DeleteUserQuery(const QString& userName)
            : userName_(userName)
        {
        }

        databasetypes::IBaseDataPtr DeleteUserQuery::data()
        {
            return databasetypes::IBaseDataPtr(new databasetypes::DeleteUserData(userName_));
        }

        IDatabaseReaderImplPtr DeleteUserQuery::readerImpl(IAsyncDataReceiver *receiver)
        {
            databasereaderimpl::SyncReportReaderImpl::Map map;
            map.insert(databasetypes::ExecutionReportData::BAD, tr("Error while deleting user"));

            return IDatabaseReaderImplPtr(new databasereaderimpl::SyncReportReaderImpl(map));
        }
    } // databasequery
} // database
