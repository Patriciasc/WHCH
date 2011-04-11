/*
 * #########################################################################
 * # File: WHCH (Working Hours Counter for Humans)- whchTreeProxyModel.cpp #
 * #                                                                       #
 * # Author: Patricia Santana Cruz  <patriciasc@openismus.com>             #
 * #                                                                       #
 * # Copyright (C) 2010 Openismus GmbH                                     #
 * #                                                                       #
 * # Version: 0.3                                                          #
 * #                                                                       #
 * # Description: Application for tracking the time spent on individual    #
 * # projects.                                                             #
 * #                                                                       #
 * # WHCH is free software; you can redistribute it and/or modify          #
 * # it under the terms of the GNU General Public License as published by  #
 * # the Free Software Foundation; either version 3 of the License,        #
 * # or (at your option) any later version.                                #
 * #                                                                       #
 * # WHCH is distributed in the hope that it will be useful,               #
 * # but WITHOUT ANY WARRANTY; without even the implied warranty of        #
 * # MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU     #
 * # General Public License for more details.                              #
 * #                                                                       #
 * # You should have received a copy of the GNU General Public License     #
 * # along with WHCH. If not, see <http://www.gnu.org/licenses/>.          #
 * #########################################################################
*/

#include "whchTreeProxyModel.h"
#include "whchDomNode.h"
#include "whchDomModel.h"
#include <QStringList>
#include <QDebug>

WhchTreeProxyModel::WhchTreeProxyModel(QObject *parent) :
    QSortFilterProxyModel(parent)
{
}

bool WhchTreeProxyModel::filterAcceptsRow(int sourceRow,
                                          const QModelIndex &sourceParent) const
{
    QModelIndex index = sourceModel()->index(sourceRow, 6, sourceParent);
    // Check if it is a task row.
    if (sourceModel()->data(index).toString().compare("") != 0)
        return false;

    return true;
}

bool WhchTreeProxyModel::filterAcceptsColumn (int sourceColumn,
                                              const QModelIndex & sourceParent ) const
{
    Q_UNUSED(sourceParent);

    if (sourceColumn != 0)
        return false;
    return true;
}
