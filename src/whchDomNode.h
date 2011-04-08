/*
 * ########################################################################
 * # File: WHCH (Working Hours Counter for Humans)- whchDomNode.h         #
 * #                                                                      #
 * # Author: Patricia Santana Cruz  <patriciasc@openismus.com>            #
 * #                                                                      #
 * # Copyright (C) 2010 Openismus GmbH                                    #
 * #                                                                      #
 * # Version: 0.3                                                         #
 * #                                                                      #
 * # Description: Application for tracking the time spent on individual   #
 * # projects.                                                            #
 * #                                                                      #
 * # WHCH is free software; you can redistribute it and/or modify         #
 * # it under the terms of the GNU General Public License as published by #
 * # the Free Software Foundation; either version 3 of the License,       #
 * # or (at your option) any later version.                               #
 * #                                                                      #
 * # WHCH is distributed in the hope that it will be useful,              #
 * # but WITHOUT ANY WARRANTY; without even the implied warranty of       #
 * # MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU    #
 * # General Public License for more details.                             #
 * #                                                                      #
 * # You should have received a copy of the GNU General Public License    #
 * # along with WHCH. If not, see <http://www.gnu.org/licenses/>.         #
 * ########################################################################
*/

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
