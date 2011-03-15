#ifndef WHCHTABLEPROXYMODEL_H
#define WHCHTABLEPROXYMODEL_H

#include <QSortFilterProxyModel>

class WhchTableProxyModel :  public QSortFilterProxyModel
{
public:
    WhchTableProxyModel(QObject *parent = 0);

protected:
    bool filterAcceptsColumn (int source_column,
                              const QModelIndex & source_parent ) const;
};

#endif // WHCHTABLEPROXYMODEL_H
