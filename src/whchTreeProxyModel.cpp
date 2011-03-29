#include "whchTreeProxyModel.h"
#include "whchDomNode.h"
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

bool WhchTreeProxyModel::filterAcceptsColumn (int sourceColumn,
                                              const QModelIndex & sourceParent ) const
{
    Q_UNUSED(sourceParent);

    if (sourceColumn != 0)
        return false;
    return true;
}

void WhchTreeProxyModel::onItemClicked(const QModelIndex &index)
{
    emit clicked(mapToSource(index));
}
