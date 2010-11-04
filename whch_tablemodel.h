#ifndef WHCH_TABLEMODEL_H
#define WHCH_TABLEMODEL_H

#include <QAbstractTableModel>
#include <QList>
#include <QDomDocument>
#include <QStringList>
#include "whch_task.h"

class whch_TableModel : public QAbstractTableModel
{
    Q_OBJECT

public:
    whch_TableModel(QObject *parent=0);

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

    QStringList get_clients_list();

public slots:
    void set_new_task(whch_task current_task);
    QStringList get_client_tasks(const QString &client);

private:
    /* .xml file in memory. */
    QDomDocument m_dom_file; 
    QDomElement m_task;
    QTime m_timer;

    void load_xml_file(const QString &filename);
    void write_in_xml_file (const QString &filename);
};

#endif // WHCH_TABLEMODEL_H
