#ifndef CLIPBOARDSINGLETON_H
#define CLIPBOARDSINGLETON_H

#include <QObject>
#include <QQueue>
#include <QPair>
#include <QVariant>

class ClipboardSingleton : public QObject
{
    Q_OBJECT

public:
    static ClipboardSingleton* instance()
    {
        if (instance_ == NULL)
        {
            instance_ = new ClipboardSingleton();
        }
        return instance_;
    }

    enum ClipboardType
    {
        NONE,
        TEXT,
        RICHTEXT,
        IMAGE
    };

    typedef QPair<QVariant, ClipboardType> ClipboardPair;
    typedef QQueue<ClipboardPair > ClipboardQueue;

    void clipboardData(QVariant& data /* out */, ClipboardType& type /* out */);
    void setClipboadData(const QVariant& data, ClipboardType type);
    const ClipboardQueue& clipboardQueue() const;

private slots:
    void clipboardChanged();

private:
    Q_INVOKABLE void queuedSetClipboardData(const QVariant& data, ClipboardType type);

    ClipboardSingleton(QObject *parent = NULL);
    ClipboardSingleton(const ClipboardSingleton&);
    ClipboardSingleton& operator=(const ClipboardSingleton&);

    static ClipboardSingleton *instance_;

    ClipboardQueue clipboardQueue_;
};

#endif // CLIPBOARDSINGLETON_H
