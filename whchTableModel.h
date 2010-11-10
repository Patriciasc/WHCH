#ifndef WHCHTABLEMODEL_H
#define WHCHTABLEMODEL_H

#include <QAbstractTableModel>
#include <QList>
#include <QDomDocument>
#include <QStringList>
#include "whchTask.h"

class WhchTableModel : public QAbstractTableModel
{
    Q_OBJECT

public:
    WhchTableModel(QObject *parent=0);

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

    QStringList AttributesList(const QString &attribute);

public slots:
    void setNewTask(WhchTask currentTask);
    QStringList ClientTasks(const QString &client);

private:
    /* .xml file in memory. */
    QDomDocument m_domFile;
    QDomElement m_task;
    QTime m_timer;

    void loadXmlFile(const QString &filename);
    void writeInXmlFile (const QString &filename);
};

#endif // WHCHTABLEMODEL_H
