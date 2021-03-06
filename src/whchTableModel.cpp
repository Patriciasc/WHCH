/*
 * ########################################################################
 * # File: WHCH (Working Hours Counter for Humans)- whchTableModel.cpp    #
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

#include "whchTableModel.h"
#include "whchTask.h"
#include <QDomDocument>
#include <QFile>
#include <iostream>
#include <QTextStream>
#include <QDateTime>
#include <QDebug>
#include <QDir>

static const QString XML_FILENAME = "whch_log.xml";

WhchTableModel::WhchTableModel(QObject *parent)
    :QAbstractTableModel(parent)
{
    // Set start time and timer for first task.
    m_task = m_domFile.createElement("task");
    m_task.setAttribute("start", QDateTime::currentDateTime().toString(Qt::ISODate));

    /* Create .xml file if it does not exist. */

    QFile file(QDir::homePath() + "/" + "." + XML_FILENAME);
    if (!file.exists())
    {
        m_domFile = QDomDocument("WHCH");

        // Create root element for the DOM
        QString currentDate(QDate::currentDate().toString("yyyy/MM/dd"));

        QDomElement domRoot = m_domFile.createElement("year");
        domRoot.setAttribute("date", currentDate.section("/", 0, 0));
        m_domFile.appendChild(domRoot);

        // Write result to an .xml file.
        if (!file.open(QIODevice::WriteOnly))
            return;

        QTextStream ts(&file);
        ts << m_domFile.toString();

        file.close();
    }

    /* Load .xml file in memory. */
    loadXmlFile(XML_FILENAME);
}

int WhchTableModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);

    QString currentDate(QDate::currentDate().toString("yyyy/MM/dd"));
    int rowCount = 0;

    for (QDomElement domRoot = m_domFile.firstChildElement("year");
    !domRoot.isNull(); domRoot = domRoot.nextSiblingElement("year"))
    {
        if (domRoot.attribute("date").compare(currentDate.section("/", 0, 0)) == 0)
        {
            for (QDomElement weekElement = domRoot.firstChildElement("week");
            !weekElement.isNull(); weekElement = weekElement.nextSiblingElement("week"))
            {
                if (weekElement.attribute("number").toInt() == QDate::currentDate().weekNumber())
                {
                    for (QDomElement dayElement = weekElement.firstChildElement("day");
                    !dayElement.isNull(); dayElement = dayElement.nextSiblingElement("day"))
                    {
                        if (dayElement.attribute("date").compare(currentDate) == 0)
                            rowCount = +dayElement.elementsByTagName("task").count();
                    }
                }
            }
        }
    }
    return rowCount;
}

int WhchTableModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return 6;
}

QVariant WhchTableModel::data(const QModelIndex &index,
                               int role) const
{
    if ((index.isValid()) && ((role == Qt::DisplayRole) || (role == Qt::EditRole)))
    {
        QString currentDate (QDate::currentDate().toString("yyyy/MM/dd"));
        QDomElement element;

        // Search for the element in memory.
        for (QDomElement domRoot = m_domFile.firstChildElement("year");
        !domRoot.isNull(); domRoot = domRoot.nextSiblingElement("year"))
        {
            for (QDomElement weekElement = domRoot.firstChildElement("week");
            !weekElement.isNull(); weekElement = weekElement.nextSiblingElement("week"))
            {
                for (QDomElement dayElement = weekElement.firstChildElement("day");
                !dayElement.isNull(); dayElement = dayElement.nextSiblingElement("day"))
                {
                    if (dayElement.attribute("date").compare(currentDate) == 0)
                    {
                        element = dayElement.firstChildElement("task");
                        break;
                    }
                }

            }
        }

        for (int i=1; i<=index.row(); i++ )
            element = element.nextSiblingElement("task");

        // Display index value.
        switch (index.column())
        {
            case 0:
            {
                QString time = element.attribute("start");
                return QDateTime::fromString(time, Qt::ISODate);
            }
            case 1:
            {
                QString time = element.attribute("end");
                return QDateTime::fromString(time, Qt::ISODate);
            }
            case 2:
            {
                QVariant start = element.attribute("start");
                QVariant end = element.attribute("end");
                int seconds = start.toDateTime().secsTo(end.toDateTime());
                int minutes = seconds / 60;
                seconds %= 60;
                int hours = minutes / 60;
                minutes %= 60;
                return element.attribute("duration", QString("%1h%2m").
                                         arg(hours).arg(minutes));
            }
            case 3:
                return element.attribute("client");
            case 4:
                return element.attribute("name");
            case 5:
                return element.firstChildElement("details").text();
            default:
                return QVariant();
        }
    }
    return QVariant();
}

QVariant WhchTableModel::headerData(int section,
                                     Qt::Orientation orientation,
                                     int role) const
{
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole)
    {
        switch (section)
        {
            case 0:
                return tr("Start");
            case 1:
                return tr("End");
            case 2:
                return tr("Duration");
            case 3:
                return tr("Client");
            case 4:
                return tr("Task");
            case 5:
                return tr("Details");
            default:
                return QVariant();
        }
    }
    return QVariant();
}

Qt::ItemFlags WhchTableModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return Qt::ItemIsEnabled;

    if ((index.column() == 0) || (index.column() == 1) || (index.column() == 5))
    {
        return QAbstractItemModel::flags(index) | Qt::ItemIsEditable;
    }
    else
    {
        return Qt::ItemIsEnabled;
    }
}

bool WhchTableModel::setData(const QModelIndex &index,
                              const QVariant &value,
                              int role)
{
    bool changed = false;

    if (index.isValid() && role == Qt::EditRole)
    {
        QString currentDate (QDate::currentDate().toString("yyyy/MM/dd"));
        QDomElement element;

        // Serach for the element in memory
        for (QDomElement domRoot = m_domFile.firstChildElement("year");
        !domRoot.isNull(); domRoot = domRoot.nextSiblingElement("year"))
        {
            for (QDomElement weekElement = domRoot.firstChildElement("week");
            !weekElement.isNull(); weekElement = weekElement.nextSiblingElement("week"))
            {
                for (QDomElement dayElement = weekElement.firstChildElement("day");
                !dayElement.isNull(); dayElement = dayElement.nextSiblingElement("day"))
                {
                    if (dayElement.attribute("date").compare(currentDate) == 0)
                    {
                        element = dayElement.firstChildElement("task");
                        break;
                    }
                }
            }
        }

        for (int i=1; i<=index.row(); i++)
            element = element.nextSiblingElement("task");

        // Replace new value in memory.
        switch (index.column())
        {
            case 0:
                element.setAttribute("start", value.toDateTime().toString(Qt::ISODate));
                emit dataChanged(index, index);
                changed = true;
                break;
            case 1:
                element.setAttribute("end", value.toDateTime().toString(Qt::ISODate));
                emit dataChanged(index, index);
                changed = true;
                break;
            case 5:
                element.firstChildElement("details").firstChild().toText().setData(value.toString());
                emit dataChanged(index, index);
                changed = true;
                break;
            default:
                changed = false;
                break;
        }

        // Update change in .xml file.
        if (changed)
            writeInXmlFile(XML_FILENAME);
    }
    return changed;
}

/* List of attributes "attribute". */
QStringList WhchTableModel::AttributesList(const QString &attribute)
{
    QStringList attributes;

    for (QDomElement domRoot = m_domFile.firstChildElement("year");
    !domRoot.isNull(); domRoot = domRoot.nextSiblingElement("year"))
    {
        for (QDomElement weekElement = domRoot.firstChildElement("week");
        !weekElement.isNull(); weekElement = weekElement.nextSiblingElement("week"))
        {
            for (QDomElement dayElement = weekElement.firstChildElement("day");
            !dayElement.isNull(); dayElement = dayElement.nextSiblingElement("day"))
            {
                for (QDomElement element = dayElement.firstChildElement("task");
                !element.isNull(); element = element.nextSiblingElement("task"))
                {
                    const QString attributeName = element.attribute(attribute);
                    // Do not repeat attributes in the list.
                    if (attributes.filter(attributeName).empty())
                        attributes << attributeName;
                }
            }
        }
    }
    return attributes;
}

// Retrieves given task's client.
QString WhchTableModel::clientOfTask(const QString &task)
{
    QString client;

    for (QDomElement domRoot = m_domFile.firstChildElement("year");
    !domRoot.isNull(); domRoot = domRoot.nextSiblingElement("year"))
    {
        for (QDomElement weekElement = domRoot.firstChildElement("week");
        !weekElement.isNull(); weekElement = weekElement.nextSiblingElement("week"))
        {
            for (QDomElement dayElement = weekElement.firstChildElement("day");
            !dayElement.isNull(); dayElement = dayElement.nextSiblingElement())
            {
                for (QDomElement element = dayElement.firstChildElement("task");
                !element.isNull(); element = element.nextSiblingElement("task"))
                {
                    if (element.attribute("name").compare(task) == 0)
                        return client = element.attribute("client");
                }
            }
        }

    }
    return client;
}

bool WhchTableModel::isClient(const QString &client)
{
    for (QDomElement domRoot = m_domFile.firstChildElement("year");
    !domRoot.isNull(); domRoot = domRoot.nextSiblingElement("year"))
    {
        for (QDomElement weekElement = domRoot.firstChildElement("week");
        !weekElement.isNull(); weekElement = weekElement.nextSiblingElement("week"))
        {
            for (QDomElement dayElement = weekElement.firstChildElement("day");
            !dayElement.isNull(); dayElement = dayElement.nextSiblingElement("day"))
            {
                for (QDomElement element = dayElement.firstChildElement("task");
                !element.isNull(); element = element.nextSiblingElement("task"))
                {
                    if (element.attribute("client").compare(client) == 0)
                        return true;
                }
            }
        }
    }
    return false;
}

/* Return DOM File in memory. */
QDomDocument WhchTableModel::domFile()
{
    return m_domFile;
}

/* ----- */
/* SLOTS */
/* ----- */

/* List of tasks related with the given client. */
QStringList WhchTableModel::ClientTasks(const QString &client)
{
    QStringList clientTasks;

    for (QDomElement domRoot = m_domFile.firstChildElement("year");
    !domRoot.isNull(); domRoot = domRoot.nextSiblingElement("year"))
    {
        for (QDomElement weekElement = domRoot.firstChildElement("week");
        !weekElement.isNull(); weekElement = weekElement.nextSiblingElement("week"))
        {
            for (QDomElement dayElement = weekElement.firstChildElement("day");
            !dayElement.isNull(); dayElement = dayElement.nextSiblingElement("day"))
            {
                for (QDomElement element = dayElement.firstChildElement("task");
                !element.isNull(); element = element.nextSiblingElement("task"))
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
    return clientTasks;
}

/* TODO: Refactorize. */
void WhchTableModel::setNewTask(WhchTask currentTask)
{
    QString currentDate (QDate::currentDate().toString("yyyy/MM/dd"));

    // Set the new task in the current date.
    for (QDomElement domRoot = m_domFile.firstChildElement("year");
    !domRoot.isNull(); domRoot = domRoot.nextSiblingElement("year"))
    {
        if (domRoot.attribute("date").compare(currentDate.section("/", 0, 0)) == 0)
        {
            // Look if the current week already exists in memory.
            QDomElement weekElement = domRoot.firstChildElement("week");
            for (; !weekElement.isNull(); weekElement = weekElement.nextSiblingElement())
            {
                if (weekElement.attribute("number").toInt() == QDate::currentDate().weekNumber())
                    break;
            }

            // Create current week in memory.
            if (weekElement.isNull())
            {
                weekElement = m_domFile.createElement("week");
                weekElement.setAttribute("number", QDate::currentDate().weekNumber());
                domRoot.appendChild(weekElement);
            }

            // Look if the current day already exists in memory
            QDomElement dayElement = weekElement.firstChildElement("day");
            for (; !dayElement.isNull(); dayElement = dayElement.nextSiblingElement())
            {
                if (dayElement.attribute("date").compare(currentDate) == 0)
                    break;
            }

            // Create current day in memory.
            if (dayElement.isNull())
            {
                dayElement = m_domFile.createElement("day");
                dayElement.setAttribute("date", QDate::currentDate().toString("yyyy/MM/dd"));
                weekElement.appendChild(dayElement);
            }

            // Set data for the new task. (FUNCION SET TASK)
            m_task.setAttribute("end", QDateTime::currentDateTime().toString(Qt::ISODate));
            m_task.setAttribute("name", currentTask.m_name.section(" (", 0,0));
            m_task.setAttribute("client", currentTask.m_client);

            QDomElement detailsTag = m_domFile.createElement("details");
            QDomText detailsText = m_domFile.createTextNode(currentTask.m_details);
            detailsTag.appendChild(detailsText);
            m_task.appendChild(detailsTag);

            // Add elements in memory.
            dayElement.appendChild(m_task);

            // Write result to an .xml file.
            writeInXmlFile (XML_FILENAME);

            // Refresh view.
            reset();

            loadXmlFile(XML_FILENAME);

            // Set start time and restart timer for the next task.
            m_task = m_domFile.createElement("task");
            m_task.setAttribute("start", QDateTime::currentDateTime().toString(Qt::ISODate));
        }
    }
}

/* ------------------- */
/* AUXILIARY FUNCTIONS */
/* ------------------- */

// Load .xml file's content (data) in memory.
void WhchTableModel::loadXmlFile(const QString &fileName)
{
    QFile file(QDir::homePath() + "/" + "." + fileName);

    if (!file.open(QIODevice::ReadOnly))
    {
        qDebug() << "Error. Could not open file: " << fileName
                << "in" << QDir::current().absolutePath();
        return;
    }

    QString setContentError;
    if (!m_domFile.setContent(&file, &setContentError))
    {
        qDebug() << "Error. Could not set content for file: " << fileName
                << "in" << QDir::current().absolutePath();
        qDebug() << "Error message: " << setContentError;

        file.close();
        return;
    }

    file.close();
}

// Update .xml file's content with the data in memory.
void WhchTableModel::writeInXmlFile (const QString &fileName)
{
    QFile file(QDir::homePath() + "/" + "." + fileName);

    if (!file.open(QIODevice::WriteOnly))
    {
        qDebug() << " Error updating memory data from file: " << fileName;
        return;
    }
    QTextStream ts(&file);
    ts << m_domFile.toString();

    file.close();
 }
