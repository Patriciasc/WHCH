/*
 * ########################################################################
 * # File: WHCH (Working Hours Counter for Humans)- whchDomModel.h        #
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

#ifndef WHCHDOMMODEL_H
#define WHCHDOMMODEL_H

#include "whchDomNode.h"
#include "whchTask.h"
#include <QAbstractItemModel>
#include <QDomDocument>
#include <QModelIndex>
#include <QVariant>

class WhchDomModel : public QAbstractItemModel
{
    Q_OBJECT

public:
    WhchDomModel(QObject *parent = 0);
    ~WhchDomModel();

    QVariant data(const QModelIndex &index, int role) const;
    bool setData(const QModelIndex &index,
                               const QVariant &value, int role);
    Qt::ItemFlags flags(const QModelIndex &index) const;
    QVariant headerData(int section, Qt::Orientation orientation,
                        int role = Qt::DisplayRole) const;
    QModelIndex index(int row, int column,
                      const QModelIndex &parent = QModelIndex()) const;
    QModelIndex parent(const QModelIndex &child) const;
    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    int columnCount(const QModelIndex &parent = QModelIndex()) const;

    void addNewTaskElement(WhchTask currentTask);
    QModelIndex currentDayIndex();
    QStringList AttributesList(const QString &attribute);
    QStringList xmlClientTasks(const QString &client);
    bool isXmlClient(const QString &client);
    QString xmlClientOfTask(const QString &task);

    // Debugging functions.
     void printModelIndexTree();

private:
    QDomDocument m_domDocument;
    WhchDomNode *m_rootNode;
    QDomElement m_taskNode;

    // Auxiliary functions.
    void writeInXmlFile(const QString &fileName);
    void loadXmlFile(const QString &fileName);
};

#endif // WHCHDOMMODEL_H
