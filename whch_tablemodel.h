#ifndef WHCH_TABLEMODEL_H
#define WHCH_TABLEMODEL_H

#include <QAbstractTableModel>
#include <QList>

class whch_TableModel : public QAbstractTableModel
{
    Q_OBJECT

public:
    whch_TableModel(QObject *parent=0);
    whch_TableModel(const QList<QString> &list, QObject *parent=0);

    /* Basic functions for providing data access. */
    int rowCount(const QModelIndex &parent  = QModelIndex()) const;
    int columnCount(const QModelIndex &parent) const;
    QVariant data(const QModelIndex &index, int role) const;
    QVariant headerData(int section, Qt::Orientation orientation,
                        int role = Qt::DisplayRole) const;

private:
    /* Test data structure. */
    QList<QString> m_DisplayList;
};

#endif // WHCH_TABLEMODEL_H
