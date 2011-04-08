#ifndef WHCHDOMMODEL_H
#define WHCHDOMMODEL_H

#include "whchDomNode.h"
#include "whchTask.h"
#include <QAbstractItemModel>
#include <QDomDocument>
#include <QModelIndex>
#include <QVariant>

class WhchDomModel : public QAbstractItemModel
{
    Q_OBJECT

public:
    WhchDomModel(QObject *parent = 0);
    ~WhchDomModel();

    QVariant data(const QModelIndex &index, int role) const;
    bool setData(const QModelIndex &index,
                               const QVariant &value, int role);
    Qt::ItemFlags flags(const QModelIndex &index) const;
    QVariant headerData(int section, Qt::Orientation orientation,
                        int role = Qt::DisplayRole) const;
    QModelIndex index(int row, int column,
                      const QModelIndex &parent = QModelIndex()) const;
    QModelIndex parent(const QModelIndex &child) const;
    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    int columnCount(const QModelIndex &parent = QModelIndex()) const;

    void addNewTaskElement(WhchTask currentTask);
    QModelIndex currentDayIndex();
    QStringList AttributesList(const QString &attribute);
    QStringList xmlClientTasks(const QString &client);
    bool isXmlClient(const QString &client);
    QString xmlClientOfTask(const QString &task);

    // Debugging functions.
     void printModelIndexTree();

private:
    QDomDocument m_domDocument;
    WhchDomNode *m_rootNode;
    QDomElement m_taskNode;

    // Auxiliary functions.
    void writeInXmlFile(const QString &fileName);
    void loadXmlFile(const QString &fileName);
};

#endif // WHCHDOMMODEL_H
