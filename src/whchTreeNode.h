#ifndef WHCHTREENODE_H
#define WHCHTREENODE_H

#include <QDomNode>
#include <QHash>

class WhchTreeNode
{
public:
    WhchTreeNode(QDomNode &node, int row, WhchTreeNode *parent = 0);
    ~WhchTreeNode();
    QDomNode node() const;
    WhchTreeNode *child(int rowNumber);
    WhchTreeNode *parent();
    int row();

private:
    QDomNode m_domNode;
    QHash<int,WhchTreeNode*> m_child;
    WhchTreeNode *m_parent;
    int m_row;
};

#endif // WHCHTREENODE_H
