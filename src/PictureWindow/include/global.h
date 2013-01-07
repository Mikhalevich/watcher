#ifndef GLOBAL_H
#define GLOBAL_H

#include <QPixmap>

namespace params
{
    enum Operation {
        NOOPERATION,

        // autorization
        AUTORIZATION,

        // windows operations
        GETALLWINDOWS,
        GETALLCONTROLS,
        SETCONTROLTEXT,
        GETCONTROLINFO,
        RUNPROCESS,

        // pictures operations
        GETPICTURE,
        GETALLPICTURES,
        GETCURRENTDATEPICTURES,
        GETPERIODPICTURES,
        STARTPICTURETIMER,
        STOPPICTURETIMER
    };

    enum ControlType {
        EDIT,
        BUTTON,
        COMBOBOX
    };

    /* the end in transmittion */
    /* in size field */
    const qint32 END = 0;
}

//------------------------------------------------------------------------------------

namespace datatypes
{
    class AbstractData
    {
    public:
        virtual ~AbstractData()
        {
        }

        virtual params::Operation type() const = 0;
    };

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

        virtual params::Operation type() const
        {
            return params::GETCONTROLINFO;
        }

    private:
        QString controlInfo_;
    };

    class WindowData : public AbstractData
    {
    public:
        WindowData(QString windowName, QString windowClass)
            : windowName_(windowName)
            , windowClass_(windowClass)
        {
        }

        virtual params::Operation type() const
        {
            return params::GETALLWINDOWS;
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

    class ControlData : public AbstractIndexData
    {
    public:
        ControlData(int controlIndex, QString controlText, QString controlClass)
            : AbstractIndexData(controlIndex)
            , controlText_(controlText)
            , controlClass_(controlClass)
        {
        }

        virtual params::Operation type() const
        {
            return params::GETALLCONTROLS;
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

    class PictureData : public AbstractData
    {
    public:
        PictureData(QPixmap pix)
            : pixmap_(pix)
        {
        }

        virtual params::Operation type() const
        {
            return params::GETPICTURE;
        }

        const QPixmap& pixmap() const
        {
            return pixmap_;
        }

    private:
        QPixmap pixmap_;
    };
}

#endif // GLOBAL_H
