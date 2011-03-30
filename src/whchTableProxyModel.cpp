#include "whchTableProxyModel.h"
#include <QDebug>

WhchTableProxyModel::WhchTableProxyModel(QObject *parent) :
        QSortFilterProxyModel(parent)
{
}

void WhchTableProxyModel::onItemClicked(const QModelIndex &index)
{
    emit retrieve_children(mapFromSource(index));
}
