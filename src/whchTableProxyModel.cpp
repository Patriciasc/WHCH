#include "whchTableProxyModel.h"

WhchTableProxyModel::WhchTableProxyModel(QObject *parent) :
        QSortFilterProxyModel(parent)
{
}

bool WhchTableProxyModel::filterAcceptsColumn (int source_column,
                          const QModelIndex & source_parent ) const
{
    if (source_column == 0)
        return false;
    return true;
}
