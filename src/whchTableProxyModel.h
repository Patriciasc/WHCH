/*
 * ##########################################################################
 * # File: WHCH (Working Hours Counter for Humans)- whchTableProxyModel.h   #
 * #                                                                        #
 * # Author: Patricia Santana Cruz  <patriciasc@openismus.com>              #
 * #                                                                        #
 * # Copyright (C) 2010 Openismus GmbH                                      #
 * #                                                                        #
 * # Version: 0.3                                                           #
 * #                                                                        #
 * # Description: Application for tracking the time spent on individual     #
 * # projects.                                                              #
 * #                                                                        #
 * # WHCH is free software; you can redistribute it and/or modify           #
 * # it under the terms of the GNU General Public License as published by   #
 * # the Free Software Foundation; either version 3 of the License,         #
 * # or (at your option) any later version.                                 #
 * #                                                                        #
 * # WHCH is distributed in the hope that it will be useful,                #
 * # but WITHOUT ANY WARRANTY; without even the implied warranty of         #
 * # MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU      #
 * # General Public License for more details.                               #
 * #                                                                        #
 * # You should have received a copy of the GNU General Public License      #
 * # along with WHCH. If not, see <http://www.gnu.org/licenses/>.           #
 * ##########################################################################
*/

#ifndef WHCHTABLEPROXYMODEL_H
#define WHCHTABLEPROXYMODEL_H

#include <QSortFilterProxyModel>

class WhchTableProxyModel :  public QSortFilterProxyModel
{
    Q_OBJECT
public:
    WhchTableProxyModel(QObject *parent = 0);
};

#endif // WHCHTABLEPROXYMODEL_H
