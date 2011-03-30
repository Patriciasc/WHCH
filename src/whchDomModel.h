#ifndef WHCHDOMMODEL_H
#define WHCHDOMMODEL_H

#include "whchDomNode.h"
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
    Qt::ItemFlags flags(const QModelIndex &index) const;
    QVariant headerData(int section, Qt::Orientation orientation,
                        int role = Qt::DisplayRole) const;
    QModelIndex index(int row, int column,
                      const QModelIndex &parent = QModelIndex()) const;
    QModelIndex parent(const QModelIndex &child) const;
    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    int columnCount(const QModelIndex &parent = QModelIndex()) const;

    //Debugging functions.
     void printModelIndexTree();
     void loadXmlFile(const QString &fileName);

private:
    QDomDocument m_domDocument;
    WhchDomNode *m_rootNode;

    void writeInXmlFile(const QString &fileName);
};

#endif // WHCHDOMMODEL_H
