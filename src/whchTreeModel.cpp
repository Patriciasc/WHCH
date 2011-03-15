#include "whchTreeModel.h"
#include <QStringList>
#include <QDateTime>

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

    switch (index.column())
    {
        case 0:
            for (int i = 0; i < attributeMap.count(); ++i)
            {
                QDomNode attribute = attributeMap.item(i);
                attributes << attribute.nodeName() + " " + attribute.nodeValue();
            }
            return attributes;
        case 1:
            {
                QString time = attributeMap.namedItem("start").nodeValue();
                return QDateTime::fromString(time, Qt::ISODate);
            }
        case 2:
            {
                QString time = attributeMap.namedItem("end").nodeValue();
                return QDateTime::fromString(time, Qt::ISODate);
            }
        case 3:
            {
                QVariant start = attributeMap.namedItem("start").nodeValue();
                QVariant end = attributeMap.namedItem("end").nodeValue();
                int seconds = start.toDateTime().secsTo(end.toDateTime());
                int minutes = seconds / 60;
                seconds %= 60;
                int hours = minutes / 60;
                minutes %= 60;
                return QString("%1h%2m%3s").arg(hours).arg(minutes).arg(seconds);
            }
        case 4:
            return attributeMap.namedItem("client").nodeValue();
        case 5:
            return attributeMap.namedItem("name").nodeValue();;
        case 6:
            return node.firstChildElement("details").text();
        default:
            return QVariant();
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
        case 1:
            return tr("Start");
        case 2:
            return tr("End");
        case 3:
            return tr("Duration");
        case 4:
            return tr("Client");
        case 5:
            return tr("Task");
        case 6:
            return tr("Details");
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
    return 7;
}
