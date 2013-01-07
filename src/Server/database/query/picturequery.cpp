
#include <QBuffer>

#include "picturequery.h"

namespace database
{
    namespace databasequery
    {
        StorePictureQuery::StorePictureQuery(const QImage& image, const QDateTime& date)
            : image_(image)
            , date_(date)
        {
        }

        databasetypes::IBaseDataPtr StorePictureQuery::data()
        {
            return databasetypes::IBaseDataPtr(new databasetypes::PictureData(image_, date_));
        }

        ///////////////////////////////////////////////////////////////////////////////////

        GetPictureQuery::GetPictureQuery(const QDateTime& first, const QDateTime& second)
            : firstDate_(first)
            , secondDate_(second)
        {
        }

        databasetypes::IBaseDataPtr GetPictureQuery::data()
        {
            return databasetypes::IBaseDataPtr(new databasetypes::GetPicturesData(firstDate_, secondDate_));
        }

        IDatabaseReaderImplPtr GetPictureQuery::readerImpl(IAsyncDataReceiver *receiver)
        {
            return IDatabaseReaderImplPtr(new AsyncReader(receiver));
        }

        GetPictureQuery::AsyncReader::AsyncReader(IAsyncDataReceiver *receiver)
            : IDatabaseReaderImpl(receiver)
        {
        }

        void GetPictureQuery::AsyncReader::execute(databasetypes::IBaseDataPtr base)
        {
            switch (base->type())
            {
            case databasetypes::PictureEntity:
                receiver_->processData(base);
                break;

            case databasetypes::EndEntity:
                receiver_->processData(base);
                emit quit();
                break;

            default:
                Q_ASSERT("You shouldn't be here" && false);
                break;
            }
        }

        ///////////////////////////////////////////////////////////////////////////////////////////

        databasetypes::IBaseDataPtr GetNewPictureQuery::data()
        {
            return databasetypes::IBaseDataPtr(new databasetypes::RequestData(databasetypes::GetNewPictureEntity));
        }

        IDatabaseReaderImplPtr GetNewPictureQuery::readerImpl(IAsyncDataReceiver *receiver)
        {
            return IDatabaseReaderImplPtr(new SyncReader());
        }

        const GetNewPictureQuery::Conteiner& GetNewPictureQuery::pictures() const
        {
            const SyncReader& impl = static_cast<const SyncReader&>(*impl_);
            return impl.pictures_;
        }

        void GetNewPictureQuery::SyncReader::execute(databasetypes::IBaseDataPtr base)
        {
            switch (base->type())
            {
            case databasetypes::PictureEntity:
                transmitPicture(static_cast<databasetypes::PictureData&>(*base));
                break;

            case databasetypes::EndEntity:
                transmitEnd();
                break;

            default:
                Q_ASSERT("You shouldn't be here");
                break;
            }
        }

        void GetNewPictureQuery::SyncReader::transmitPicture(const databasetypes::PictureData &picture)
        {
            QImage img(picture.picture());
            QString date(picture.date().toString(QLatin1String("yyyy-MM-dd hh:mm:ss")));

            // write pixmap in bytes
            QByteArray bytes;
            QBuffer buf(&bytes);
            buf.open(QIODevice::WriteOnly);
            img.save(&buf, "png");
            buf.close();

            pictures_.push_back(QPair<QString, QByteArray>(date, bytes));
        }

        void GetNewPictureQuery::SyncReader::transmitEnd()
        {
            // todo

            emit quit();
        }
    } // databasequery
} // database
