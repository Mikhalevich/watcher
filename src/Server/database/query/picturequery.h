#ifndef PICTUREQUERY_H
#define PICTUREQUERY_H

#include <QImage>
#include <QDateTime>

#include "abstractdatabasequery.h"

namespace database
{
    namespace databasequery
    {
        class StorePictureQuery : public IDatabaseWriter
        {
        public:
            StorePictureQuery(const QImage& image, const QDateTime& date);

        protected:
            virtual databasetypes::IBaseDataPtr data();

        private:
            QImage image_;
            QDateTime date_;
        };

        ////////////////////////////////////////////////////////////////

        class GetPictureQuery : public IDatabaseReader
        {
        public:
            GetPictureQuery(const QDateTime& first = QDateTime(), const QDateTime& second = QDateTime());

        protected:
            virtual databasetypes::IBaseDataPtr data();
            virtual IDatabaseReaderImplPtr readerImpl(IAsyncDataReceiver *receiver);

        private:
            QDateTime firstDate_;
            QDateTime secondDate_;

            class AsyncReader : public IDatabaseReaderImpl
            {
            public:
                AsyncReader(IAsyncDataReceiver *receiver);

                virtual void execute(databasetypes::IBaseDataPtr base);
            };

        };

        /////////////////////////////////////////////////////////////////////

        class GetNewPictureQuery : public IDatabaseReader
        {
        public:
            typedef QVector<QPair<QString, QByteArray> > Conteiner;

            const Conteiner& pictures() const;

        protected:
            virtual databasetypes::IBaseDataPtr data();
            virtual IDatabaseReaderImplPtr readerImpl(IAsyncDataReceiver *receiver);

        private:
        
            class SyncReader : public IDatabaseReaderImpl
            {
                friend class GetNewPictureQuery;

            public:
                virtual void execute(databasetypes::IBaseDataPtr base);

            private:
                void transmitPicture(const databasetypes::PictureData &picture);
                void transmitEnd();

                Conteiner pictures_;
            };

        };
    } // databasequery
} // database

#endif // PICTUREQUERY_H