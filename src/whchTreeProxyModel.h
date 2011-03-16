#ifndef WHCHTREEPROXYMODEL_H
#define WHCHTREEPROXYMODEL_H

#include <QSortFilterProxyModel>

class WhchTreeProxyModel : public QSortFilterProxyModel
{
    Q_OBJECT
public:
    explicit WhchTreeProxyModel(QObject *parent = 0);

public slots:
    void onItemClicked(const QModelIndex &index);

protected:
    bool filterAcceptsRow(int sourceRow,
                          const QModelIndex &sourceParent) const;
    bool filterAcceptsColumn (int sourceColumn,
                              const QModelIndex &sourceParent ) const;

signals:
     void clicked(const QModelIndex &index);
};

#endif // WHCHTREEPROXYMODEL_H
