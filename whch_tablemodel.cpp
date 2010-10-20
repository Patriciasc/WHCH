#include "whch_tablemodel.h"

whch_TableModel::whch_TableModel(QObject *parent)
    :QAbstractTableModel(parent)
{
}

whch_TableModel::whch_TableModel(QList<QString> mylist, QObject *parent)
    :QAbstractTableModel(parent)
{
}

int whch_TableModel::rowCount(const QModelIndex &parent) const
{
}

int whch_TableModel::columnCount(const QModelIndex &parent) const
{
}

QVariant whch_TableModel::data(const QModelIndex &index, int role) const
{
}

QVariant whch_TableModel::headerData(int section, Qt::Orientation orientation, int role) const
{
}
