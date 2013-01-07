
#include <QBuffer>
#include <QByteArray>

#include "allpicturesquery.h"
#include "database/query/picturequery.h"

namespace network
{
    namespace networkquery
    {
        AllPicturesQuery::AllPicturesQuery()
        {
        }

        void AllPicturesQuery::execute()
        {
            database::databasequery::GetPictureQuery query;
            query.execute(this);
        }

        void AllPicturesQuery::processData(database::databasetypes::IBaseDataPtr base)
        {
            switch (base->type())
            {
            case database::databasetypes::PictureEntity:
                transmitPicture(static_cast<database::databasetypes::PictureData&>(*base));
                break;

            case database::databasetypes::EndEntity:
                transmitEnd();
                break;

            default:
                Q_ASSERT("You shouldn't be here" && false);
                break;
            }
        }

        void AllPicturesQuery::transmitPicture(const database::databasetypes::PictureData &picture)
        {
            QImage img = picture.picture();
            QDateTime date = picture.date();

            // write pixmap in bytes
            QByteArray bytes;
            QBuffer buf(&bytes);
            buf.open(QIODevice::WriteOnly);
            img.save(&buf, "png");
            buf.close();

            QByteArray dataToWrite;
            global::functions::writeDataToByteArray(dataToWrite, GETPICTURE, bytes, date);

            emit writeData(dataToWrite);
        }

        void AllPicturesQuery::transmitEnd()
        {
            QByteArray dataToWrite;
            global::functions::writeDataToByteArray(dataToWrite, GETPICTURE, END);

            emit writeData(dataToWrite);
        }
    } // networkquery
}// network
