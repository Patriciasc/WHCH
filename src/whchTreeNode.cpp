#include "whchTreeNode.h"

WhchTreeNode::WhchTreeNode(QDomNode &node, int row,
                           WhchTreeNode *parent)
{
    m_node = node;
    m_row = row;
    m_parent = parent;
}

WhchTreeNode::~WhchTreeNode()
{
    QHash<int,WhchTreeNode*>::iterator i;
    for (i = m_children.begin(); i != m_children.end(); ++i)
        delete i.value();
}

QDomNode WhchTreeNode::node() const
{
    return m_node;
}

WhchTreeNode *WhchTreeNode::parent()
{
    return m_parent;
}

WhchTreeNode *WhchTreeNode::child(int i)
{
    if (m_children.contains(i))
        return m_children[i];

    if (i >= 0 && i < m_node.childNodes().count())
    {
        QDomNode newChildNode = m_node.childNodes().item(i);
        WhchTreeNode *childNode = new WhchTreeNode(newChildNode, i, this);
        m_children[i] = childNode;
        return childNode;
    }
    return 0;
}

int WhchTreeNode::row()
{
    return m_row;
}
