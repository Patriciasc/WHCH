/*
 * ########################################################################
 * # File: WHCH (Working Hours Counter for Humans)- whchDomModel.cpp      #
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

#include "whchDomModel.h"
#include <QStringList>
#include <QDateTime>
#include <QDir>
#include <QDebug>

static const QString XML_FILENAME = "whch_log.xml";

WhchDomModel::WhchDomModel(QObject *parent) :
              QAbstractItemModel(parent)
{
    // Set start time and timer for first task.
    m_taskNode = m_domDocument.createElement("task");
    m_taskNode.setAttribute("start", QDateTime::currentDateTime().toString(Qt::ISODate));

    m_domDocument = QDomDocument("WHCH");
    QString fileName(QDir::homePath() + "/" + "." + XML_FILENAME);
    QFile file(fileName);

    // Create a new .xml file.
    if (!file.exists())
    {
        // Initialize root element in memory.
        QDomElement root = m_domDocument.createElement("History");
        m_domDocument.appendChild(root);
        m_rootNode = new WhchDomNode(m_domDocument, 0);

        // Write memory data in the .xml file.
        writeInXmlFile(XML_FILENAME);
    }
    else
        m_rootNode = new WhchDomNode(m_domDocument, 0);

    // Load .xml file's content in memory.
    loadXmlFile(XML_FILENAME);

}


WhchDomModel::~WhchDomModel()
{
    delete m_rootNode;
}

QVariant WhchDomModel::data(const QModelIndex &index,
                            int role) const
{
    if ((index.isValid()) && ((role == Qt::DisplayRole) || (role == Qt::EditRole)))
    {
        WhchDomNode *indexNode = static_cast<WhchDomNode*>(index.internalPointer());

        QDomNode node = indexNode->node();
        QStringList attributes;
        QDomNamedNodeMap attributeMap = node.attributes();

        switch (index.column())
        {
        case 0:
            for (int i = 0; i < attributeMap.count(); ++i)
            {
                QDomNode attribute = attributeMap.item(i);
                attributes << attribute.nodeName() + " " + attribute.nodeValue();
            }
            return attributes;
             case 1:
            {
                QString time = attributeMap.namedItem("start").nodeValue();
                return QDateTime::fromString(time, Qt::ISODate);
            }
             case 2:
            {
                QString time = attributeMap.namedItem("end").nodeValue();
                return QDateTime::fromString(time, Qt::ISODate);
            }
             case 3:
            {
                QVariant start = attributeMap.namedItem("start").nodeValue();
                QVariant end = attributeMap.namedItem("end").nodeValue();
                int seconds = start.toDateTime().secsTo(end.toDateTime());
                int minutes = seconds / 60;
                seconds %= 60;
                int hours = minutes / 60;
                minutes %= 60;
                return QString("%1h%2m").arg(hours).arg(minutes);
            }
             case 4:
            return attributeMap.namedItem("client").nodeValue();
             case 5:
            return attributeMap.namedItem("name").nodeValue();;
             case 6:
            return node.firstChildElement("details").text();
             default:
            return QVariant();
        }
    }
    return QVariant();
}

bool WhchDomModel::setData(const QModelIndex &index,
                            const QVariant &value,
                            int role)
{
    bool changed = false;

    if (index.isValid() && role == Qt::EditRole)
    {

        WhchDomNode *indexNode = static_cast<WhchDomNode*>(index.internalPointer());
        QDomNode node = indexNode->node();
        QDomNamedNodeMap attributeMap = node.attributes();

        switch (index.column())
        {
        case 1:
            attributeMap.namedItem("start").setNodeValue(value.toDateTime().toString(Qt::ISODate));
            emit dataChanged(index, index);
            changed = true;
            break;
        case 2:
            attributeMap.namedItem("end").setNodeValue(value.toDateTime().toString(Qt::ISODate));
            emit dataChanged(index, index);
            changed = true;
            break;
        case 6:
            node.firstChildElement("details").firstChild().toText().setNodeValue(value.toString());
            emit dataChanged(index, index);
            changed = true;
            break;
        default:
            changed = false;
            break;
        }

        if (changed)
            writeInXmlFile(XML_FILENAME);
    }
    return changed;
}


Qt::ItemFlags WhchDomModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return 0;

    int columnNumber = index.column();
    if ((columnNumber == 1) || (columnNumber == 2) || (columnNumber == 6))
        return Qt::ItemIsEnabled | Qt::ItemIsEditable | Qt::ItemIsSelectable;
    else
        return Qt::ItemIsEnabled;
}

QVariant WhchDomModel::headerData(int section,
                                  Qt::Orientation orientation,
                                  int role) const
{
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole)
    {
        switch (section)
        {
        case 0:
            return tr("History");
        case 1:
            return tr("Start");
        case 2:
            return tr("End");
        case 3:
            return tr("Duration");
        case 4:
            return tr("Client");
        case 5:
            return tr("Task");
        case 6:
            return tr("Details");
        default:
            return QVariant();
        }
    }
    return QVariant();
}

QModelIndex WhchDomModel::index(int row,
                                int column,
                                const QModelIndex &parent) const
{
    if (!hasIndex(row, column, parent))
        return QModelIndex();

    WhchDomNode *parentNode;

    if (!parent.isValid())
        parentNode = m_rootNode;
    else
        parentNode = static_cast<WhchDomNode*>(parent.internalPointer());

    WhchDomNode *childNode = parentNode->child(row);
    if (childNode)
        return createIndex(row, column, childNode);
    else
        return QModelIndex();
}

QModelIndex WhchDomModel::parent(const QModelIndex &child) const
{
    if (!child.isValid())
        return QModelIndex();

    WhchDomNode *childNode = static_cast<WhchDomNode*>(child.internalPointer());
    WhchDomNode *parentNode = childNode->parent();

    if (!parentNode || parentNode == m_rootNode)
        return QModelIndex();

    return createIndex(parentNode->row(), 0, parentNode);
}

int WhchDomModel::rowCount(const QModelIndex &parent) const
{
    if (parent.column() > 0)
        return 0;

    WhchDomNode *parentNode;

    if (!parent.isValid())
        parentNode = m_rootNode;
    else
        parentNode = static_cast<WhchDomNode*>(parent.internalPointer());

    return parentNode->node().childNodes().count();
}

int WhchDomModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);

    return 7;
}

// Adds a new task node element.
void WhchDomModel::addNewTaskElement(WhchTask currentTask)
{
    QString currentDate(QDate::currentDate().toString("yyyy/MM/dd"));
    QDomElement rootElement =  m_domDocument.firstChildElement();

    QDomElement yearNode = rootElement.lastChildElement();
    QString yearDate(currentDate.section("/", 0, 0));
    if (yearNode.attribute("year").compare(yearDate) != 0)
    {
        yearNode = m_domDocument.createElement("year");
        yearNode.setAttribute("year", yearDate);
        rootElement.appendChild(yearNode);
    }

    QDomElement monthNode = yearNode.lastChildElement();
    int monthNumber = QDate::currentDate().month();
    QString month = QDate::longMonthName(monthNumber);
    if(monthNode.attribute("month").compare(month) != 0)
    {
        // Create month node and assign it as month node.
        monthNode = m_domDocument.createElement("month");
        monthNode.setAttribute("month",  month);
        yearNode.appendChild(monthNode);
    }

    QDomElement weekNode = monthNode.lastChildElement();
    int weekNumber = QDate::currentDate().weekNumber();
    if(weekNode.attribute("week").toInt() != weekNumber)
    {
        // Create week node and assign it as week node.
        weekNode = m_domDocument.createElement("week");
        weekNode.setAttribute("week",  weekNumber);
        monthNode.appendChild(weekNode);
    }

    QDomElement dayNode = weekNode.lastChildElement();
    if(dayNode.attribute("date").compare(currentDate) != 0)
    {
        // Create day node and assign it as day node.
        dayNode = m_domDocument.createElement("day");
        dayNode.setAttribute("date", currentDate);
        weekNode.appendChild(dayNode);
    }

    // Create a new task node.
    m_taskNode.setAttribute("end", QDateTime::currentDateTime().toString(Qt::ISODate));
    m_taskNode.setAttribute("name", currentTask.m_name.section(" (", 0,0));
    m_taskNode.setAttribute("client", currentTask.m_client);

    QDomElement detailsNode= m_domDocument.createElement("details");
    QDomText detailsText = m_domDocument.createTextNode(currentTask.m_details);
    detailsNode.appendChild(detailsText);
    m_taskNode.appendChild(detailsNode);

    dayNode.appendChild(m_taskNode);

    // Write data in .xml file.
    writeInXmlFile (XML_FILENAME);

    // Reset view.
    reset();

    // Set start time and timer for first task.
    m_taskNode = m_domDocument.createElement("task");
    m_taskNode.setAttribute("start", QDateTime::currentDateTime().toString(Qt::ISODate));
}

// Returns the index of the item representing the current day
// or an invalid index if the item does not exist.
QModelIndex WhchDomModel::currentDayIndex()
{
    // Get root index.
    QDomElement rootElement =  m_domDocument.firstChildElement();
    QModelIndex rootIndex = index(0, 0, QModelIndex());

    // Get current year index.
    QDomElement currentYearElement = rootElement.lastChildElement();
    int numYearElements = rootElement.childNodes().count();
    QModelIndex yearIndex = index(numYearElements-1, 0, rootIndex);

    if (!yearIndex.isValid())
        return QModelIndex();

    // Get current month index.
    QDomElement currentMonthElement = currentYearElement.lastChildElement();
    int numMonthElements = currentYearElement.childNodes().count();
    QModelIndex monthIndex = index(numMonthElements-1, 0, yearIndex);

    if (!monthIndex.isValid())
        return QModelIndex();

    // Get current week index.
    QDomElement currentWeekElement = currentMonthElement.lastChildElement();
    int numWeekElements = currentMonthElement.childNodes().count();
    QModelIndex weekIndex = index(numWeekElements-1, 0, monthIndex);

    if (!weekIndex.isValid())
        return QModelIndex();

    // Get current day index.
    QString currentDate(QDate::currentDate().toString("yyyy/MM/dd"));
    QDomElement currentDayElement = currentWeekElement.lastChildElement();

    if (currentDayElement.attribute("date").compare(currentDate) == 0)
    {
        int numDayElements = currentWeekElement.childNodes().count();
        QModelIndex dayIndex = index(numDayElements-1, 0, weekIndex);

        if (!dayIndex.isValid())
            return QModelIndex();

        return dayIndex;
    }
    else
        return QModelIndex();
}

// Returns a list of "attribute" attributes.
QStringList WhchDomModel::AttributesList(const QString &attribute)
{
    QStringList attributes;
    QDomElement rootElement =  m_domDocument.firstChildElement();

    for (QDomElement yearElement = rootElement.firstChildElement();
    !yearElement.isNull(); yearElement = yearElement.nextSiblingElement())
    {
        for (QDomElement monthElement = yearElement.firstChildElement();
        !monthElement.isNull(); monthElement = monthElement.nextSiblingElement())
        {
            for (QDomElement weekElement = monthElement.firstChildElement();
            !weekElement.isNull(); weekElement = weekElement.nextSiblingElement())
            {
                for (QDomElement dayElement = weekElement.firstChildElement();
                !dayElement.isNull(); dayElement = dayElement.nextSiblingElement())
                {
                    for (QDomElement element = dayElement.firstChildElement();
                    !element.isNull(); element = element.nextSiblingElement())
                    {
                        const QString attributeName = element.attribute(attribute);
                        // Do not repeat attributes in the list.
                        if (attributes.filter(attributeName).empty())
                            attributes << attributeName;
                    }
                }
            }
        }
    }
    return attributes;
}

// Return a list of tasks related to the given client.
QStringList WhchDomModel::xmlClientTasks(const QString &client)
{
    QStringList clientTasks;
    QDomElement rootElement =  m_domDocument.firstChildElement();

    for (QDomElement yearElement = rootElement.firstChildElement();
    !yearElement.isNull(); yearElement = yearElement.nextSiblingElement())
    {
        for (QDomElement monthElement = yearElement.firstChildElement();
        !monthElement.isNull(); monthElement = monthElement.nextSiblingElement())
        {
            for (QDomElement weekElement = monthElement.firstChildElement();
            !weekElement.isNull(); weekElement = weekElement.nextSiblingElement())
            {
                for (QDomElement dayElement = weekElement.firstChildElement();
                !dayElement.isNull(); dayElement = dayElement.nextSiblingElement())
                {
                    for (QDomElement element = dayElement.firstChildElement();
                    !element.isNull(); element = element.nextSiblingElement())
                    {
                        if (element.attribute("client").compare(client) == 0)
                        {
                            const QString taskName = element.attribute("name");
                            /* Not repeat tasks in the list. */
                            if (clientTasks.filter(taskName).empty())
                                clientTasks << taskName;
                        }
                    }
                }
            }
        }
    }
    return clientTasks;
}

// Checks if the given client is in the .xml file.
bool WhchDomModel::isXmlClient(const QString &client)
{
    QDomElement rootElement =  m_domDocument.firstChildElement();

    for (QDomElement yearElement = rootElement.firstChildElement();
    !yearElement.isNull(); yearElement = yearElement.nextSiblingElement())
    {
        for (QDomElement monthElement = yearElement.firstChildElement();
        !monthElement.isNull(); monthElement = monthElement.nextSiblingElement())
        {
            for (QDomElement weekElement = monthElement.firstChildElement();
            !weekElement.isNull(); weekElement = weekElement.nextSiblingElement())
            {
                for (QDomElement dayElement = weekElement.firstChildElement();
                !dayElement.isNull(); dayElement = dayElement.nextSiblingElement())
                {
                    for (QDomElement element = dayElement.firstChildElement();
                    !element.isNull(); element = element.nextSiblingElement())
                    {
                        if (element.attribute("client").compare(client) == 0)
                            return true;
                    }
                }
            }
        }
    }
    return false;
}

// Retrieves the client of a given task.
QString WhchDomModel::xmlClientOfTask(const QString &task)
{
    QString client;
    QDomElement rootElement =  m_domDocument.firstChildElement();


    for (QDomElement yearElement = rootElement.firstChildElement();
    !yearElement.isNull(); yearElement = yearElement.nextSiblingElement())
    {
        for (QDomElement monthElement = yearElement.firstChildElement();
        !monthElement.isNull(); monthElement = monthElement.nextSiblingElement())
        {
            for (QDomElement weekElement = monthElement.firstChildElement();
            !weekElement.isNull(); weekElement = weekElement.nextSiblingElement())
            {
                for (QDomElement dayElement = weekElement.firstChildElement();
                !dayElement.isNull(); dayElement = dayElement.nextSiblingElement())
                {
                    for (QDomElement element = dayElement.firstChildElement();
                    !element.isNull(); element = element.nextSiblingElement())
                    {
                        if (element.attribute("name").compare(task) == 0)
                            return client = element.attribute("client");
                    }
                }
            }
        }

    }
    return client;
}

// ## Debugging functions ##

// Prints the DomModel.
void WhchDomModel::printModelIndexTree()
{
    qDebug() << "----------------------------------- Tree Model Index -----------------------------------";
    qDebug() <<"";

    QDomElement rootElement =  m_domDocument.firstChildElement();
    QModelIndex rootIndex = index(0, 0, QModelIndex());
    qDebug() << "History" << rootIndex.data() << rootIndex;

    int numYearElements = (m_rootNode->node().childNodes().count()) - 1;
    for (QDomElement YearElement = rootElement.firstChildElement();
    !YearElement.isNull(); YearElement = YearElement.nextSiblingElement())
    {
        QModelIndex yearIndex= index(numYearElements--, 0, rootIndex);
        qDebug() << "|";
        qDebug() << "|_" << YearElement.nodeName() << yearIndex.data() << yearIndex;

        int numMonthElements = YearElement.childNodes().count() - 1;
        for (QDomElement monthElement = YearElement.firstChildElement("week");
        !monthElement.isNull(); monthElement = monthElement.nextSiblingElement("week"))
        {
            QModelIndex monthIndex= index(numMonthElements--, 0, yearIndex);
            qDebug() << "|";
            qDebug() << "|_" << monthElement.nodeName() << monthIndex.data() << monthIndex;


            int numWeekElements = monthElement.childNodes().count() - 1;
            for (QDomElement weekElement = monthElement.firstChildElement("week");
            !weekElement.isNull(); weekElement = weekElement.nextSiblingElement("week"))
            {
                QModelIndex weekIndex = index(numWeekElements--, 0, yearIndex);
                qDebug() << "   |";
                qDebug() << "   |_" << weekElement.nodeName() << weekIndex.data() << weekIndex;
                qDebug() << "   |  |";

                int numDayElements = weekElement.childNodes().count() - 1;
                for (QDomElement dayElement = weekElement.firstChildElement("day");
                !dayElement.isNull(); dayElement = dayElement.nextSiblingElement("day"))
                {
                    QModelIndex dayIndex = index(numDayElements--, 0, weekIndex);
                    qDebug() << "   |  |_" << dayElement.nodeName() << dayIndex.data() << dayIndex;
                }
            }
        }
    }

    qDebug() <<"";
    qDebug() << "----------------------------------------------------------------------------------------";
}

// ## Auxiliary functions ##
// Load .xml file's content in memory.
void WhchDomModel::loadXmlFile(const QString &fileName)
{
    QFile file(QDir::homePath() + "/" + "." + fileName);

    if (!file.open(QIODevice::ReadOnly))
    {
        qDebug() << "Error. Could not open file: " << fileName
                << "in" << QDir::current().absolutePath();
        return;
    }

    QString setContentError;
    if (!m_domDocument.setContent(&file, &setContentError))
    {
        qDebug() << "Error. Could not set content for file: " << fileName
                << "in" << QDir::current().absolutePath();
        qDebug() << "Error message: " << setContentError;

        file.close();
        return;
    }

    file.close();
}

// Write memory data in the .xml file.
void WhchDomModel::writeInXmlFile (const QString &fileName)
{
    QFile file(QDir::homePath() + "/" + "." + fileName);

    if (!file.open(QIODevice::WriteOnly))
    {
        qDebug() << "Error. Could not write in file: " << fileName;
        return;
    }

    QTextStream ts(&file);
    ts << m_domDocument.toString();

    file.close();
}
