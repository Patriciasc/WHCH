#include "whchTreeProxyModel.h"

WhchTreeProxyModel::WhchTreeProxyModel(QObject *parent) :
    QSortFilterProxyModel(parent)
{
}

bool WhchTreeProxyModel::filterAcceptsRow(int sourceRow,
                                          const QModelIndex &sourceParent)
{
}
