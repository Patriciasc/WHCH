#include "whchTreeProxyModel.h"
#include "whchTreeNode.h"
#include <QStringList>
#include <QDebug>

WhchTreeProxyModel::WhchTreeProxyModel(QObject *parent) :
    QSortFilterProxyModel(parent)
{
}

bool WhchTreeProxyModel::filterAcceptsRow(int sourceRow,
                                          const QModelIndex &sourceParent) const
{
    QModelIndex index = sourceModel()->index(sourceRow, 4, sourceParent);
    // Check if it is a task row.
    if (sourceModel()->data(index).toString().compare("") != 0)
        return false;

    return true;
}

bool WhchTreeProxyModel::filterAcceptsColumn (int source_column,
                                              const QModelIndex & source_parent ) const
{
    if (source_column != 0)
        return false;
    return true;
}
