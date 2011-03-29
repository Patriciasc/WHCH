#ifndef WHCHDOMNODE_H
#define WHCHDOMNODE_H

#include <QDomNode>
#include <QHash>

class WhchDomNode
{
public:
    WhchDomNode(QDomNode &node, int row, WhchDomNode *parent = 0);
    ~WhchDomNode();
    QDomNode node() const;
    WhchDomNode *parent();
    WhchDomNode *child(int rowNumber);
    int row();

private:
    QDomNode m_domNode;
    QHash<int,WhchDomNode*> m_childNodes;
    WhchDomNode *m_parentNode;
    int m_rowNumber;
};

#endif // WHCHDOMNODE_H
