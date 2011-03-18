#include "whchTableProxyModel.h"
#include <QDebug>

WhchTableProxyModel::WhchTableProxyModel(QObject *parent) :
        QSortFilterProxyModel(parent)
{
}

/*
bool WhchTableProxyModel::filterAcceptsColumn (int sourceColumn,
                          const QModelIndex & sourceParent ) const
{
    Q_UNUSED(sourceParent);

    if (sourceColumn == 0)
        return false;
    return true;
}
*/

//test
void WhchTableProxyModel::onItemClicked(const QModelIndex &index)
{
    QModelIndex sourceIndex = mapFromSource(index);
    emit retrieve_children(sourceIndex);
}
