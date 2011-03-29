#ifndef WHCHTABLEPROXYMODEL_H
#define WHCHTABLEPROXYMODEL_H

#include <QSortFilterProxyModel>

class WhchTableProxyModel :  public QSortFilterProxyModel
{
    Q_OBJECT
public:
    WhchTableProxyModel(QObject *parent = 0);

public slots:
    void onItemClicked(const QModelIndex &index);

protected:
    bool filterAcceptsColumn (int sourceColumn,
                              const QModelIndex & sourceParent ) const;

signals:
     void retrieve_children(const QModelIndex &index);
};

#endif // WHCHTABLEPROXYMODEL_H
