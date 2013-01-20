#ifndef RESPONCEDATA_H
#define RESPONCEDATA_H

#include "global.h"

namespace clientsocket
{
    namespace responcedata
    {
        using namespace global::params;

        /* base data class */
        class AbstractData
        {
        public:
            virtual ~AbstractData()
            {
            }

            virtual Operation type() const = 0;
        };
        //////////////////////////////////////////////////////////////////////////////////////////////

        class AbstractIndexData : public AbstractData
        {
        public:
            AbstractIndexData(int index)
                : index_(index)
            {
            }

            int index() const
            {
                return index_;
            }

        private:
            int index_;
        };
        //////////////////////////////////////////////////////////////////////////////////////////////

        class ControlInfoData : public AbstractData
        {
        public:
            ControlInfoData(const QString& controlInfo)
                : controlInfo_(controlInfo)
            {
            }

            const QString& controlInfo() const
            {
                return controlInfo_;
            }

            virtual Operation type() const
            {
                return GETCONTROLINFO;
            }

        private:
            QString controlInfo_;
        };
        //////////////////////////////////////////////////////////////////////////////////////////////

        class WindowData : public AbstractData
        {
        public:
            WindowData(QString windowName, QString windowClass)
                : windowName_(windowName)
                , windowClass_(windowClass)
            {
            }

            virtual Operation type() const
            {
                return GETALLWINDOWS;
            }

            const QString& windowName() const
            {
                return windowName_;
            }

            const QString& windowClass() const
            {
                return windowClass_;
            }

        private:
            QString windowName_;
            QString windowClass_;

        };
        //////////////////////////////////////////////////////////////////////////////////////////////

        class ControlData : public AbstractIndexData
        {
        public:
            ControlData(int controlIndex, QString controlText, QString controlClass)
                : AbstractIndexData(controlIndex)
                , controlText_(controlText)
                , controlClass_(controlClass)
            {
            }

            virtual Operation type() const
            {
                return GETALLCONTROLS;
            }

            const QString& controlText() const
            {
                return controlText_;
            }

            const QString& controlClass() const
            {
                return controlClass_;
            }

        private:
            QString controlText_;
            QString controlClass_;
        };
        //////////////////////////////////////////////////////////////////////////////////////////////

        class PictureData : public AbstractData
        {
        public:
            PictureData(const QPixmap& pix, const QDateTime& date)
                : pixmap_(pix)
                , date_(date)
            {
            }

            virtual Operation type() const
            {
                return GETPICTURE;
            }

            const QPixmap& pixmap() const
            {
                return pixmap_;
            }

            const QDateTime& date() const
            {
                return date_;
            }

        private:
            QPixmap pixmap_;
            QDateTime date_;
        };
        //////////////////////////////////////////////////////////////////////////////////////////////

        class MailPropertiesData : public AbstractData
        {
        public:
            MailPropertiesData(const QString& server, qint32 serverPort,
                               const QString& user, const QString& password,
                               const QString& sendFrom, const QStringList& sendTo,
                               qint16 interval)
                : server_(server)
                , serverPort_(serverPort)
                , user_(user)
                , password_(password)
                , sendFrom_(sendFrom)
                , sendTo_(sendTo)
                , interval_(interval)
            {
            }

            virtual Operation type() const
            {
                return GETMAILPROPERTIES;
            }

            QString server_;
            qint32 serverPort_;
            QString user_;
            QString password_;
            QString sendFrom_;
            QStringList sendTo_;
            qint16 interval_;
        };
        //////////////////////////////////////////////////////////////////////////////////////////////

        class PicturePropertiesData : public AbstractData
        {
        public:
            PicturePropertiesData(const qint16 interval)
                : interval_(interval)
            {
            }

            virtual Operation type() const
            {
                return GETPICTURETIMER;
            }

            const int interval() const
            {
                return (int)interval_;
            }

        private:
            qint16 interval_;
        };
        //////////////////////////////////////////////////////////////////////////////////////////////

        class ExecutionReportData : public AbstractData
        {
        public:
            ExecutionReportData(const QString error)
                : error_(error)
            {
            }

            virtual Operation type() const
            {
                return EXECUTIONREPORT;
            }

            const QString& error() const
            {
                return error_;
            }

        private:
            QString error_;
        };

        ///////////////////////////////////////////////////////////////////////////////////////////////

        class SettingsData : public AbstractData
        {
        public:
            SettingsData(const qint32 port, const qint8 startupMode, qint8 trayIcon)
                : port_(port)
                , startupMode_(startupMode)
                , trayIcon_(trayIcon)
            {
            }

            virtual Operation type() const
            {
                return GETSETTINGS;
            }

            const qint32 port() const
            {
                return port_;
            }

            const qint8 startupMode() const
            {
                return startupMode_;
            }

            const qint8 trayIcon() const
            {
                return trayIcon_;
            }

        private:
            qint32 port_;
            qint8 startupMode_;
            qint8 trayIcon_;
        };
    } // responcedata
} // clientsocket

#endif // RESPONCEDATA_H
