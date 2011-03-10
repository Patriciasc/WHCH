#include "whchTreeNode.h"

WhchTreeNode::WhchTreeNode(QDomNode &node, int row,
                           WhchTreeNode *parent)
{
    m_domNode = node;
    m_row = row;
    m_parent = parent;
}

WhchTreeNode::~WhchTreeNode()
{
    QHash<int,WhchTreeNode*>::iterator i;
    for (i = m_child.begin(); i != m_child.end(); ++i)
        delete i.value();
}

QDomNode WhchTreeNode::node() const
{
    return m_domNode;
}

WhchTreeNode *WhchTreeNode::parent()
{
    return m_parent;
}

WhchTreeNode *WhchTreeNode::child(int rowNumber)
{
    if (m_child.contains(rowNumber))
        return m_child[rowNumber];

    if (rowNumber >= 0 && rowNumber < m_domNode.childNodes().count())
    {
        QDomNode newChildNode = m_domNode.childNodes().item(rowNumber);
        WhchTreeNode *childNode = new WhchTreeNode(newChildNode, rowNumber, this);
        m_child[rowNumber] = childNode;
        return childNode;
    }
    return 0;
}

int WhchTreeNode::row()
{
    return m_row;
}
