
#include <QApplication>
#include <QClipboard>
#include <QMimeData>
#include <QVariant>
#include <QImage>

#include "clipboardsingleton.h"

ClipboardSingleton* ClipboardSingleton::instance_ = NULL;

ClipboardSingleton::ClipboardSingleton(QObject *parent /* = NULL */)
    : QObject(parent)
{
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
