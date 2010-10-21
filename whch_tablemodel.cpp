#include "whch_tablemodel.h"

whch_TableModel::whch_TableModel(QObject *parent)
    :QAbstractTableModel(parent)
{
}

whch_TableModel::whch_TableModel(const QList<QString> &list, QObject *parent)
    :QAbstractTableModel(parent)
{
    m_DisplayList = list;
}

int whch_TableModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return m_DisplayList.count();
}

int whch_TableModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return 6;
}

QVariant whch_TableModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    if (index.row() >= m_DisplayList.size() || index.row() < 0)
        return QVariant();

    if (role == Qt::DisplayRole)
    {
        QString  myString = m_DisplayList.at(index.row());

        /* XXX:test 2 columns only. */
        if (index.column() == 0)
            return myString;
        else if (index.column() == 1)
            return myString;
    }
    return QVariant();
}

QVariant whch_TableModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role != Qt::DisplayRole)
        return QVariant();

    if (orientation == Qt::Horizontal)
    {
        switch (section)
        {
            case 0:
                return tr("Start");
            case 1:
                return tr("End");
            case 2:
                return tr("Duration");
            case 3:
                return tr("Client");
            case 4:
                return tr("Task");
            case 5:
                return tr("Details");
            default:
                return QVariant();
        }
    }
    return QVariant();
}
