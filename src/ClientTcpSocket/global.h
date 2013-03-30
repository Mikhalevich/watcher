#ifndef GLOBAL_H
#define GLOBAL_H

#include <QPixmap>
#include <QDateTime>

namespace global
{
    namespace types
    {
        /* operation type
         if equal zero then previous operation compleated and wait a new command */
        typedef qint8 operation_t;

        /* operation data size which read/write from/to socket */
        typedef qint32 operation_size_t;
    }

    //////////////////////////////////////////////////////////////////////////////////////////

    namespace params
    {
        enum Operation {
            NOOPERATION,

            // autorization
            AUTORIZATION,

            // pictures operations
            GETPICTURE,
            GETALLPICTURES,
            GETCURRENTDATEPICTURES,
            GETPERIODPICTURES,
            PICTURETIMER,
            GETPICTURETIMER,
            MAILPROPERTIES,
            GETMAILPROPERTIES,

            // execution process
            EXECUTIONREPORT,

            // settings
            GETSETTINGS,
            SETSETTINGS,

            // clipboard
            GETCLIPBOARD,
            SETCLIPBOARD
        };

        /* the end in transmittion */
        /* in size field */
        const qint32 END = 0;
    }

    //////////////////////////////////////////////////////////////////////////////////////////////

    namespace functions
    {
        /* calculate size of data(without operation field) and set after operation field */

        // with 1 parameter
        template <typename T>
        void writeDataToByteArray(QByteArray& outputArray, params::Operation operation, const T& t)
        {
            QDataStream out(&outputArray, QIODevice::WriteOnly);
            // write data with zero size
            out << (types::operation_t)operation << (types::operation_size_t)0 << t;
            // counting size
            types::operation_size_t size = outputArray.size() - sizeof(types::operation_t) - sizeof(types::operation_size_t);
            // write size
            out.device()->seek(sizeof(types::operation_t));
            out << size;
        }

        // with 2 parameters
        template <typename T1, typename T2>
        void writeDataToByteArray(QByteArray& outputArray, params::Operation operation, const T1& t1, const T2& t2)
        {
            QDataStream out(&outputArray, QIODevice::WriteOnly);
            // write data with zero size
            out << (types::operation_t)operation << (types::operation_size_t)0 << t1 << t2;
            // counting size
            types::operation_size_t size = outputArray.size() - sizeof(types::operation_t) - sizeof(types::operation_size_t);
            // write size
            out.device()->seek(sizeof(types::operation_t));
            out << size;
        }

        // with 3 parameters
        template <typename T1, typename T2, typename T3>
        void writeDataToByteArray(QByteArray& outputArray, params::Operation operation, const T1& t1, const T2& t2, const T3& t3)
        {
            QDataStream out(&outputArray, QIODevice::WriteOnly);
            // write data with zero size
            out << (types::operation_t)operation << (types::operation_size_t)0 << t1 << t2 << t3;
            // counting size
            types::operation_size_t size = outputArray.size() - sizeof(types::operation_t) - sizeof(types::operation_size_t);
            // write size
            out.device()->seek(sizeof(types::operation_t));
            out << size;
        }

        // with 7 parameters
        template <typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7>
        void writeDataToByteArray(QByteArray& outputArray, params::Operation operation, const T1& t1, const T2& t2, const T3& t3,
            const T4& t4, const T5& t5, const T6& t6, const T7& t7)
        {
            QDataStream out(&outputArray, QIODevice::WriteOnly);
            // write data with zero size
            out << (types::operation_t)operation << (types::operation_size_t)0 << t1 << t2 << t3 << t4 << t5 << t6 << t7;
            // counting size
            types::operation_size_t size = outputArray.size() - sizeof(types::operation_t) - sizeof(types::operation_size_t);
            // write size
            out.device()->seek(sizeof(types::operation_t));
            out << size;
        }
    }
} // global

#endif // GLOBAL_H
