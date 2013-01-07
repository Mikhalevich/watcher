#ifndef ALLPICTURESQUERRY_H
#define ALLPICTURESQUERRY_H

#include "networkquery.h"
#include "database/query/abstractdatabasequery.h"

namespace network
{
    namespace networkquery
    {
        class AllPicturesQuery : public INetworkQuery, public database::databasequery::IAsyncDataReceiver
        {
            Q_OBJECT

        public:
            AllPicturesQuery();

            virtual void execute();

            virtual void processData(database::databasetypes::IBaseDataPtr base);

        private:
            void transmitPicture(const database::databasetypes::PictureData &picture);
            void transmitEnd();
        };
    } // networkquery
} // network

#endif // ALLPICTURESQUERRY_H
