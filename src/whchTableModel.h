/*
 * ########################################################################
 * # File: WHCH (Working Hours Counter for Humans)- whchTableModel.h      #
 * #                                                                      #
 * # Author: Patricia Santana Cruz  <patriciasc@openismus.com>            #
 * #                                                                      #
 * # Copyright (C) 2010 Openismus GmbH                                    #
 * #                                                                      #
 * # Version: 0.1                                                         #
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

#ifndef WHCHTABLEMODEL_H
#define WHCHTABLEMODEL_H

#include "whchTask.h"
#include <QAbstractTableModel>
#include <QList>
#include <QDomDocument>
#include <QStringList>

class WhchTableModel : public QAbstractTableModel
{
    Q_OBJECT

public:
    WhchTableModel(QObject *parent=0);

    /* Basic functions for providing data access. */
    int rowCount(const QModelIndex &parent  = QModelIndex()) const;
    int columnCount(const QModelIndex &parent) const;
    QVariant data(const QModelIndex &index, int role) const;
    QVariant headerData(int section, Qt::Orientation orientation,
                        int role = Qt::DisplayRole) const;

    /* Basic functions for making this model editable. */
    Qt::ItemFlags flags(const QModelIndex &index) const;
    bool setData(const QModelIndex &index, const QVariant &value,
                 int role = Qt::EditRole);

    QStringList AttributesList(const QString &attribute);
    QString clientOfTask(const QString &task);
    bool isClient(const QString &client);
    QDomDocument domFile();

public slots:
    QStringList ClientTasks(const QString &client);
    void setNewTask(WhchTask currentTask);

private:
    /* .xml file in memory. */
    QDomDocument m_domFile;
    QDomElement m_task;

    void loadXmlFile(const QString &filename);
    void writeInXmlFile(const QString &filename);
 };

#endif // WHCHTABLEMODEL_H
