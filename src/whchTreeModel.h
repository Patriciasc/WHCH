#ifndef WHCHTREEMODEL_H
#define WHCHTREEMODEL_H

#include <QDomDocument>
#include <QAbstractItemModel>

class WhchTreeModel : public QAbstractItemModel
{
    Q_OBJECT

public:
    WhchTreeModel(QDomDocument document, QObject *parent = 0);
    ~WhchTreeModel();

    QVariant data(const QModelIndex &index, int role) const;
    Qt::ItemFlags flags(const QModelIndex &index) const;
    QVariant headerData(int section, Qt::Orientation orientation,
                        int role = Qt::DisplayRole) const;
    QModelIndex index(int row, int column,
                      const QModelIndex &parent = QModelIndex()) const;
    QModelIndex parent(const QModelIndex &child) const;
    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    int columnCount(const QModelIndex &parent = QModelIndex()) const;

private:
    QDomDocument m_domDocument;
    WhchTreeModel *m_root;
};

#endif // WHCHTREEMODEL_H
