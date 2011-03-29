#include "whchDomNode.h"

WhchDomNode::WhchDomNode(QDomNode &node, int row, WhchDomNode *parent)
{
    m_domNode = node;
    m_rowNumber = row;
    m_parentNode = parent;
}

WhchDomNode::~WhchDomNode()
{
    QHash<int,WhchDomNode*>::iterator it;
    for (it = m_childNodes.begin(); it != m_childNodes.end(); ++it)
        delete it.value();
}

QDomNode WhchDomNode::node() const
{
    return m_domNode;
}

WhchDomNode *WhchDomNode::parent()
{
    return m_parentNode;
}

WhchDomNode *WhchDomNode::child(int rowNumber)
{
    if (m_childNodes.contains(rowNumber))
        return m_childNodes[rowNumber];

    if (rowNumber >= 0 && rowNumber < m_domNode.childNodes().count()) {
        QDomNode childNode = m_domNode.childNodes().item(rowNumber);
        WhchDomNode *childItem = new WhchDomNode(childNode, rowNumber, this);
        m_childNodes[rowNumber] = childItem;
        return childItem;
    }
    return 0;
}

int WhchDomNode::row()
{
    return m_rowNumber;
}
