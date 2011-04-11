/*
 * #########################################################################
 * # File: WHCH (Working Hours Counter for Humans)- whchTreeProxyModel.h   #
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

#ifndef WHCHTREEPROXYMODEL_H
#define WHCHTREEPROXYMODEL_H

#include <QSortFilterProxyModel>

class WhchTreeProxyModel : public QSortFilterProxyModel
{
    Q_OBJECT
public:
    explicit WhchTreeProxyModel(QObject *parent = 0);

protected:
    bool filterAcceptsRow(int sourceRow,
                          const QModelIndex &sourceParent) const;
    bool filterAcceptsColumn (int sourceColumn,
                              const QModelIndex &sourceParent ) const;
};

#endif // WHCHTREEPROXYMODEL_H
