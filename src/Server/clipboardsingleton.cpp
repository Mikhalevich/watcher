
#include <QApplication>
#include <QClipboard>
#include <QMimeData>
#include <QVariant>
#include <QImage>

#include "clipboardsingleton.h"
#include "database/query/clipboardquery.h"

ClipboardSingleton* ClipboardSingleton::instance_ = NULL;

ClipboardSingleton::ClipboardSingleton(QObject *parent /* = NULL */)
    : QObject(parent)
{
    connect(QApplication::clipboard(), SIGNAL(dataChanged()), this, SLOT(clipboardChanged()));

    qRegisterMetaType<ClipboardType>("ClipboardType");
}

void ClipboardSingleton::clipboardData(QVariant& data /* out */, ClipboardType& type /* out */)
{
    QClipboard *clipboard = QApplication::clipboard();
    const QMimeData *mime = clipboard->mimeData();

    type = NONE;

    if (mime->hasText())
    {
        data = mime->text();
        type = TEXT;
    }
    else if (mime->hasHtml())
    {
        data = mime->html();
        type = RICHTEXT;
    }
    else if (mime->hasImage())
    {
        data = mime->imageData();
        type = IMAGE;
    }
}

void ClipboardSingleton::setClipboadData(const QVariant& data, ClipboardType type)
{
    // move to main thread
    QMetaObject::invokeMethod(this, "queuedSetClipboardData", Qt::QueuedConnection, 
        Q_ARG(QVariant, data), 
        Q_ARG(ClipboardType, type));
}

void ClipboardSingleton::queuedSetClipboardData(const QVariant& data, ClipboardType type)
{
    // work with clipboard in main thread
    QClipboard *clipboard = QApplication::clipboard();

    switch (type)
    {
    case TEXT:
        {
            QString plainText = data.toString();
            Q_ASSERT(plainText.isEmpty());

            clipboard->setText(plainText);
        }
        break;

    case RICHTEXT:
        {
            QString richText = data.toString();
            Q_ASSERT(richText.isEmpty());

            QMimeData mime;
            mime.setHtml(richText);
            clipboard->setMimeData(&mime);
        }
        break;

    case IMAGE:
        {
            QImage image = data.value<QImage>();
            Q_ASSERT(image.isNull());

            clipboard->setImage(image);
        }
        break;

    default:
        Q_ASSERT("You sould't be here" && false);
        break;
    }
}

void ClipboardSingleton::clipboardChanged()
{
    QVariant data;
    ClipboardType type;

    clipboardData(data, type);

    // do not store clipboard data to database
    /*if (type != NONE)
    {
        database::databasequery::StoreClipboardQuery query(data, type);
        query.execute();
    }*/
}
