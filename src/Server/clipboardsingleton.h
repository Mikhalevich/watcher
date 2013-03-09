#ifndef CLIPBOARDSINGLETON_H
#define CLIPBOARDSINGLETON_H

#include <QObject>

class QVariant;

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

    void clipboardData(QVariant& data /* out */, ClipboardType& type /* out */);
    void setClipboadData(const QVariant& data, ClipboardType type);

private slots:
    void clipboardChanged();

private:
    ClipboardSingleton(QObject *parent = NULL);
    ClipboardSingleton(const ClipboardSingleton&);
    ClipboardSingleton& operator=(const ClipboardSingleton&);

    static ClipboardSingleton *instance_;
};

#endif // CLIPBOARDSINGLETON_H
