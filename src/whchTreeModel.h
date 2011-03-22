#ifndef WHCHTREEMODEL_H
#define WHCHTREEMODEL_H

#include "whchTreeNode.h"
#include "whchTask.h"
#include <QDomDocument>
#include <QAbstractItemModel>
#include <QFile>

class WhchTreeModel : public QAbstractItemModel
{
    Q_OBJECT

public:
    WhchTreeModel(QObject *parent = 0);
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
    bool setData(const QModelIndex &index, const QVariant &value, int role);

public slots:
    void addNewTaskElement(WhchTask currentTask);

private:
    QDomDocument m_domFile;
    WhchTreeNode *m_root;
    QDomElement m_taskNode;

    void loadXmlFile(const QString &filename);
    void writeInXmlFile(const QString &filename);
};

#endif // WHCHTREEMODEL_H
