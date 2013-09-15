#ifndef CLIPBOARDMODEL_H
#define CLIPBOARDMODEL_H

#include <QAbstractListModel>

namespace clipboardmodel
{
    class ClipboardModel : public QAbstractListModel
    {
        Q_OBJECT

    public:
        ClipboardModel(QObject* parent = 0);

        typedef QVariant ClipboardElement;
        typedef QVector<ClipboardElement> ClipboardData;

        void addClipboardData(const ClipboardElement& element);
        void clearClipboardData();

        virtual int rowCount(const QModelIndex& parentIndex = QModelIndex()) const;
        virtual QVariant data(const QModelIndex& index, int role) const;

    private:
        ClipboardData m_clipboardData;
    };
} // clipboardmodel

#endif // CLIPBOARDMODEL_H
