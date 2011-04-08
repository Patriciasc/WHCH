/*
 * ########################################################################
 * # File: WHCH (Working Hours Counter for Humans)- whchDomNode.cpp       #
 * #                                                                      #
 * # Author: Patricia Santana Cruz  <patriciasc@openismus.com>            #
 * #                                                                      #
 * # Copyright (C) 2010 Openismus GmbH (Important: see NOTE)              #
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
 *
 * [[ NOTE ]]:
 * The DOM Model concept and some functions in WHCH were taken from the
 * Nokia examples in the reference manual and modified. For those, the
 * following license applies:
 * ########################################################################
 **
 ** Copyright (C) 2011 Nokia Corporation and/or its subsidiary(-ies).
 ** All rights reserved.
 ** Contact: Nokia Corporation (qt-info@nokia.com)
 **
 ** This file is part of the examples of the Qt Toolkit.
 **
 ** $QT_BEGIN_LICENSE:BSD$
 ** You may use this file under the terms of the BSD license as follows:
 **
 ** "Redistribution and use in source and binary forms, with or without
 ** modification, are permitted provided that the following conditions are
 ** met:
 **   * Redistributions of source code must retain the above copyright
 **     notice, this list of conditions and the following disclaimer.
 **   * Redistributions in binary form must reproduce the above copyright
 **     notice, this list of conditions and the following disclaimer in
 **     the documentation and/or other materials provided with the
 **     distribution.
 **   * Neither the name of Nokia Corporation and its Subsidiary(-ies) nor
 **     the names of its contributors may be used to endorse or promote
 **     products derived from this software without specific prior written
 **     permission.
 **
 ** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 ** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 ** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 ** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 ** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 ** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 ** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 ** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 ** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 ** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 ** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
 ** $QT_END_LICENSE$
 **
 * ########################################################################
*/

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
