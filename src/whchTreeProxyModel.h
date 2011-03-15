#ifndef WHCHTREEPROXYMODEL_H
#define WHCHTREEPROXYMODEL_H

#include <QSortFilterProxyModel>

class WhchTreeProxyModel : public QSortFilterProxyModel
{
    Q_OBJECT
public:
    explicit WhchTreeProxyModel(QObject *parent = 0);

protected:
    bool filterAcceptsRow(int sourceRow,
                          const QModelIndex &sourceParent);

};

#endif // WHCHTREEPROXYMODEL_H
