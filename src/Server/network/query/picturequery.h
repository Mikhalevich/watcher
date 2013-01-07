#ifndef PICTUREQUERRY_H
#define PICTUREQUERRY_H

#include "networkquery.h"

namespace network
{
    namespace networkquery
    {
        class PictureQuery : public INetworkQuery
        {
            Q_OBJECT

        public:
            PictureQuery();

            virtual void execute();

            /* to receive picture from PictureSingleton */
            Q_INVOKABLE void picture(QImage image);

        signals:
            void quitPicturesLoop();

        private:
            QImage image_;
        };
    } // networkquery
} // network

#endif // PICTUREQUERRY_H
