
#include <stdexcept>

#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPixmap>
#include <QLabel>
#include <QEvent>

#include "pictureswidget.h"

PicturesWidget::PicturesWidget(QWidget *parent) :
    QWidget(parent),
    currentPicture_(pictures_.end())
{
    layout_ = new QHBoxLayout(this);
    layout_->setSpacing(15);
    layout_->addStretch();
    this->setLayout(layout_);
}

void PicturesWidget::addPicture(QPixmap &pixmap, const QString& text)
{
    // pixmap to iconic label
    QPixmap iconicPixmap = pixmap.scaled(QSize(120, 120), Qt::KeepAspectRatio);

    // create widget with pixmap and text
    QWidget *widget = new QWidget(this);

    QLabel *pictureLabel = new QLabel(widget);
    pictureLabel->setStyleSheet(QLatin1String("QLabel { background-color: gray; }"));
    //label->setScaledContents(true);
    pictureLabel->setFrameShape(QFrame::Box);
    pictureLabel->setPixmap(iconicPixmap);

    QLabel *textLabel = new QLabel(text, widget);

    QVBoxLayout *widgetLayout = new QVBoxLayout(widget);
    widgetLayout->addWidget(pictureLabel, 1);
    widgetLayout->addWidget(textLabel);

    widget->setLayout(widgetLayout);
    widget->setAttribute(Qt::WA_Hover);
    widget->installEventFilter(this);

    currentPicture_ = pictures_.insert(widget, pixmap);

    layout_->insertWidget(0, widget);
}

void PicturesWidget::clearPictures()
{
    QMap<QObject*, QPixmap>::iterator it = pictures_.begin();
    while (it != pictures_.end())
    {
        QMap<QObject*, QPixmap>::iterator current = it++;

        // delete label
        QObject *obj = current.key();
        delete obj;

        // remove iterator from map
        pictures_.erase(current);
    }

    currentPicture_ = pictures_.end();
}

const QPixmap& PicturesWidget::getCurrentPicture() const
{
    if (currentPicture_ == pictures_.end())
    {
        throw std::invalid_argument("Empty picture");
    }

    return currentPicture_.value();
}

bool PicturesWidget::eventFilter(QObject *obj, QEvent *event)
{
    if (pictures_.contains(obj))
    {
        switch (event->type())
        {
        case QEvent::MouseButtonPress:
        {
            QMap<QObject*, QPixmap>::iterator it = pictures_.find(obj);
            if (it != pictures_.end())
            {
                /*if (currentPicture_)
                {
                    QLabel * label = qobject_cast<QLabel*>(currentPicture_.key());
                    if (label)
                    {
                        //label->setFocus();
                    }
                }*/

                currentPicture_ = it;
                emit changePicture(it.value());
            }
            return true;
        }

        case QEvent::HoverEnter:
        {
            QWidget *widget = qobject_cast<QWidget*>(obj);
            QSize size = widget->size();
            size.setHeight(size.height() + 10);
            size.setWidth(size.width() + 10);
            widget->resize(size);
            return true;
        }

        case QEvent::HoverLeave:
        {
            QWidget *widget = qobject_cast<QWidget*>(obj);
            QSize size = widget->size();
            size.setHeight(size.height() - 10);
            size.setWidth(size.width() - 10);
            widget->resize(size);
            return true;
        }

        default:
            return false;
        }
    }
    else
    {
        return QWidget::eventFilter(obj, event);
    }
}
