
#include <QBuffer>
#include <QPixmap>
#include <QByteArray>
#include <QSharedPointer>

#include "picturequery.h"
#include "picturesingleton.h"
#include "database/query/picturequery.h"

namespace network
{
    namespace networkquery
    {
        PictureQuery::PictureQuery()
        {
        }

        void PictureQuery::picture(QImage image)
        {
            // save image
            image_ = image;

            // close event loop
            emit quitPicturesLoop();
        }

        void PictureQuery::execute()
        {
            // event to take a picture
            QMetaObject::invokeMethod(PictureSingleton::instance(), "takePicture", Qt::QueuedConnection, Q_ARG(QObject*, this));

            // wait while picture come
            QEventLoop loop;
            connect(this, SIGNAL(quitPicturesLoop()), &loop, SLOT(quit()));
            loop.exec();

            QDateTime date(QDateTime::currentDateTime());

            // save picture to database
            database::databasequery::StorePictureQuery query(image_, date);
            query.execute();

            // write pixmap in bytes
            QByteArray bytes;
            QBuffer buf(&bytes);
            buf.open(QIODevice::WriteOnly);
            image_.save(&buf, "png");
            buf.close();

            QByteArray dataToWrite;
            global::functions::writeDataToByteArray(dataToWrite, GETPICTURE, bytes, date);

            emit writeData(dataToWrite);

            /*// send end of transmitting
            out << (qint8)GETPICTURE << END;
            */
        }
    } // networkquery
} // network
