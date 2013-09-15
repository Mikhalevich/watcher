
#include "clipboardmodel.h"

namespace clipboardmodel
{
    ClipboardModel::ClipboardModel(QObject* parent /* = 0 */)
        : QAbstractListModel(parent)
    {
    }

    int ClipboardModel::rowCount(const QModelIndex& parentIndex /* = QModelIndex() */) const
    {
        Q_UNUSED(parentIndex);

        return m_clipboardData.count();
    }

    QVariant ClipboardModel::data(const QModelIndex& index, int role) const
    {
        if (!index.isValid())
        {
            return QVariant();
        }

        ClipboardElement element = m_clipboardData.at(index.row());

        switch (role)
        {
        case Qt::DisplayRole:
            switch (element.type())
            {
            case QVariant::Image:
                element = QLatin1String("#Image#");
                break;

            default:
                // nothing
                break;
            }

            return element;

        default:
            // nothing
            break;
        }

        return QVariant();
    }

    void ClipboardModel::addClipboardData(const ClipboardElement& element)
    {
        beginInsertRows(QModelIndex(), m_clipboardData.count(), m_clipboardData.count());
        m_clipboardData.push_back(element);
        endInsertRows();
    }

    void ClipboardModel::clearClipboardData()
    {
        beginResetModel();
        m_clipboardData.clear();
        endResetModel();
    }

} // clipboardmodel
