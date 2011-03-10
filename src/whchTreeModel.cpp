#include "whchTreeModel.h"
#include <QStringList>

WhchTreeModel::WhchTreeModel(QDomDocument document,
                             QObject *parent)
                                 : QAbstractItemModel(parent), m_domDocument(document)
{
    m_root = new WhchTreeNode(m_domDocument, 0);
}

WhchTreeModel::~WhchTreeModel()
{
    delete m_root;
}

QVariant WhchTreeModel::data(const QModelIndex &index,
                             int role) const
{
    if (!index.isValid())
        return QVariant();

    if (role != Qt::DisplayRole)
        return QVariant();

    WhchTreeNode *indexNode = static_cast<WhchTreeNode*>(index.internalPointer());

    QDomNode node = indexNode->node();
    QStringList attributes;
    QDomNamedNodeMap attributeMap = node.attributes();

    if (node.nodeName().compare("task") != 0)
    {
        if (index.column() == 0)
        {
            for (int i = 0; i < attributeMap.count(); ++i)
            {
                QDomNode attribute = attributeMap.item(i);
                attributes << attribute.nodeName() + " " + attribute.nodeValue();
            }
        }
        return attributes;
    }
}

Qt::ItemFlags WhchTreeModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return 0;

    return Qt::ItemIsEnabled | Qt::ItemIsSelectable;
}

QVariant WhchTreeModel::headerData(int section,
                                   Qt::Orientation orientation,
                                   int role) const
{
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole)
    {
        switch (section)
        {
        case 0:
            return tr("History");
        default:
            return QVariant();
        }
    }
    return QVariant();
}

QModelIndex WhchTreeModel::index(int row, int column,
                                 const QModelIndex &parent) const
{
    if (!hasIndex(row, column, parent))
        return QModelIndex();

    WhchTreeNode *parentNode;

    if (!parent.isValid())
        parentNode = m_root;
    else
        parentNode = static_cast<WhchTreeNode*>(parent.internalPointer());

    WhchTreeNode *childNode = parentNode->child(row);
    if (childNode)
        return createIndex(row, column, childNode);
    else
        return QModelIndex();
}

QModelIndex WhchTreeModel::parent(const QModelIndex &child) const
{
    if (!child.isValid())
        return QModelIndex();

    WhchTreeNode *childNode = static_cast<WhchTreeNode*>(child.internalPointer());
    WhchTreeNode *parentNode = childNode->parent();

    if (!parentNode || parentNode == m_root)
        return QModelIndex();

    return createIndex(parentNode->row(), 0, parentNode);
}

int WhchTreeModel::rowCount(const QModelIndex &parent) const
{
    if (parent.column() > 0)
        return 0;

    WhchTreeNode *parentNode;

    if (!parent.isValid())
        parentNode = m_root;
    else
        parentNode = static_cast<WhchTreeNode*>(parent.internalPointer());

    return parentNode->node().childNodes().count();
}

int WhchTreeModel::columnCount(const QModelIndex &parent) const
{
    return 1;
}
