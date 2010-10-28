#ifndef WHCH_TABLEMODEL_H
#define WHCH_TABLEMODEL_H

#include <QAbstractTableModel>
#include <QList>
#include <QDomDocument>

class whch_TableModel : public QAbstractTableModel
{
    Q_OBJECT

public:
    whch_TableModel(QObject *parent=0);
    whch_TableModel(const QString &filename, QObject *parent=0);

    /* Basic functions for providing data access. */
    int rowCount(const QModelIndex &parent  = QModelIndex()) const;
    int columnCount(const QModelIndex &parent) const;
    QVariant data(const QModelIndex &index, int role) const;
    QVariant headerData(int section, Qt::Orientation orientation,
                        int role = Qt::DisplayRole) const;

    /* Basic functions for making this model editable. */
    Qt::ItemFlags flags(const QModelIndex &index) const;
    bool setData(const QModelIndex &index, const QVariant &value,
                 int role = Qt::EditRole);

private:
    /* Test data structure. */
    QList<QString> m_display_list;
    QDomDocument m_dom_file;

    void load_xml_file(const QString &filename = "test.xml");
    void write_in_xml_file (const QString &filename = "test.xml");
};

#endif // WHCH_TABLEMODEL_H
