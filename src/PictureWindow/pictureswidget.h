#ifndef PICTURESWIDGET_H
#define PICTURESWIDGET_H

#include <QWidget>
#include <QMap>

class QPixmap;
class QHBoxLayout;
class QLabel;
class QEvent;

class PicturesWidget : public QWidget
{
    Q_OBJECT

public:
    explicit PicturesWidget(QWidget *parent = 0);

    virtual QSize sizeHint() const
    {
        return QSize(170, 170);
    }

    void clearPictures();
    const QPixmap& getCurrentPicture() const;

signals:
    void changePicture(const QPixmap& pixmap);

public slots:
    void addPicture(QPixmap& pixmap, const QString& text);

protected:
    virtual bool eventFilter(QObject *obj, QEvent *event);

private:
    QHBoxLayout *layout_;
    QMap<QObject*, QPixmap> pictures_;
    QMap<QObject*, QPixmap>::iterator currentPicture_;

};

#endif // PICTURESWIDGET_H
