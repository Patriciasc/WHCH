#ifndef WHCHTREENODE_H
#define WHCHTREENODE_H

#include <QDomNode>
#include <QHash>

class WhchTreeNode
{
public:
    WhchTreeNode(QDomNode &node, int row, WhchTreeNode *parent = 0);
    ~WhchTreeNode();
    WhchTreeNode *child(int i);
    WhchTreeNode *parent();
    QDomNode node() const;
    int row();

private:
    QDomNode m_node;
    QHash<int,WhchTreeNode*> m_children;
    WhchTreeNode *m_parent;
    int m_row;
};

#endif // WHCHTREENODE_H
