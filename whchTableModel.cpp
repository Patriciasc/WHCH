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
#include <QRegExpValidator>
#include <QRegExp>
#include <QDateTime>
#include <QDebug>

static const QString FILENAME = "whch_log.xml";

WhchTableModel::WhchTableModel(QObject *parent)
    :QAbstractTableModel(parent)
{
    // Set start time and timer for first task.
    m_task = m_domFile.createElement("task");
    m_task.setAttribute("start", QDateTime::currentDateTime().toString("yyyy-MM-ddThh:mm:sstzd"));
    m_timer.start();

    /* Create .xml file if it does not exist. */
    QFile file(FILENAME);
    if (!file.exists())
    {
        m_domFile = QDomDocument("DOMtest");
        // Create a root element for the DOM.
        QDomElement domRoot = m_domFile.createElement("day");
        domRoot.setAttribute("date", QDate::currentDate().toString("yyyy/MM/dd"));
        m_domFile.appendChild(domRoot);

        // Write result to an .xml file.
        if (!file.open(QIODevice::WriteOnly))
            return;

        QTextStream ts(&file);
        ts << m_domFile.toString();

        file.close();
    }

    /* Load .xml file in memory. */
    loadXmlFile(FILENAME);
}

int WhchTableModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);

    QDomElement domRoot = m_domFile.firstChildElement("day");
    return domRoot.elementsByTagName("task").count();
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
        // Get index element from file.
        QDomElement domRoot = m_domFile.firstChildElement("day");
        QDomElement element = domRoot.firstChildElement("task");

        for (int i=1; i<=index.row(); i++ )
            element = element.nextSiblingElement("task");


        // Display index value.
        switch (index.column())
        {
            case 0:
            {
                QString time = element.attribute("start");
                return QDateTime::fromString(time, "yyyy-MM-ddThh:mm:sstzd");
            }
            case 1:
            {
                QString time = element.attribute("end");
                return QDateTime::fromString(time, "yyyy-MM-ddThh:mm:sstzd");
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
                return element.attribute("duration", QString("%1h%2m%3s").
                                         arg(hours).arg(minutes).arg(seconds));
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
            // Get current index element from memory.
            QDomElement domRoot = m_domFile.firstChildElement("day");
            QDomElement element = domRoot.firstChildElement("task");

            for (int i=1; i<=index.row(); i++)
                element = element.nextSiblingElement("task");

            // Replace new value in memory.
            switch (index.column())
            {
                case 0:
                    element.setAttribute("start", value.toDateTime().toString("yyyy-MM-ddThh:mm:sstzd"));
                    emit dataChanged(index, index);
                    changed = true;
                    break;
                case 1:
                    element.setAttribute("end", value.toDateTime().toString("yyyy-MM-ddThh:mm:sstzd"));
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
                writeInXmlFile(FILENAME);

    }
    return changed;
}

/* List of attributes "attribute". */
QStringList WhchTableModel::AttributesList(const QString &attribute)
{
    QDomElement domRoot = m_domFile.firstChildElement("day");
    QStringList attributes;

    if (!domRoot.isNull())
    {

        for (QDomElement element = domRoot.firstChildElement("task");
             !element.isNull(); element = element.nextSiblingElement("task"))
        { 
            QString attributeName = element.attribute(attribute);
            // Do not repeat attributes in the list.
            if (attributes.filter(attributeName).empty())
                attributes << attributeName;
        }
    }
    return attributes;
}

/* ----- */
/* SLOTS */
/* ----- */

/* List of tasks related with the given client. */
QStringList WhchTableModel::ClientTasks(const QString &client)
{
    QDomElement domRoot = m_domFile.firstChildElement("day");
    QStringList clientTasks;

    if (!domRoot.isNull())
    {
        QDomElement element = domRoot.firstChildElement("task");
        for (; !element.isNull(); element = element.nextSiblingElement("task"))
        {
            if (element.attribute("client").compare(client) == 0)
            {
                QString taskName = element.attribute("name");
                /* Not repeat tasks in the list. */
                if (clientTasks.filter(taskName).empty())
                    clientTasks << taskName;
            }
        }
    }
    return clientTasks;
}

/* FIXME: Refactorize. */
void WhchTableModel::setNewTask(WhchTask currentTask)
{
    // Create a root element for the DOM.
    QDomElement domRoot = m_domFile.firstChildElement("day");

    // Set data. (FUNCION SET TASK)
    m_task.setAttribute("end", QDateTime::currentDateTime().toString("yyyy-MM-ddThh:mm:sstzd"));
    m_task.setAttribute("name", currentTask.m_name);
    m_task.setAttribute("client", currentTask.m_client);

    QDomElement detailsTag = m_domFile.createElement("details");
    QDomText detailsText = m_domFile.createTextNode(currentTask.m_details);
    detailsTag.appendChild(detailsText);
    m_task.appendChild(detailsTag);

    domRoot.appendChild(m_task);

    // Write result to an .xml file. (FUNCION WRITE_XML_FILE)
    QFile file(FILENAME);
    if (!file.open(QIODevice::ReadWrite))
        std::cout << "Error writing result to file" << std::endl;

    QTextStream ts(&file);
    ts << m_domFile.toString();

    file.close();
    reset();
    loadXmlFile(FILENAME);

    // Set start time and restart timer for the next task.
    m_task = m_domFile.createElement("task");
    m_task.setAttribute("start", QDateTime::currentDateTime().toString("yyyy-MM-ddThh:mm:sstzd"));
    m_timer.start();
}

/* ------------------- */
/* AUXILIARY FUNCTIONS */
/* ------------------- */

// Load .xml file's content (data) in memory.
void WhchTableModel::loadXmlFile(const QString &filename)
{
    QFile file(filename);

    if (!file.open(QIODevice::ReadOnly))
        std::cout << "Could not open file" << std::endl;

    if (!m_domFile.setContent(&file))
    {
        std::cout << "Problem setting content" << std::endl;
        file.close();
    }

    file.close();
}

// Update .xml file's content with the data in memory.
void WhchTableModel::writeInXmlFile (const QString &filename)
{
    QFile file(filename);

    if (!file.open(QIODevice::WriteOnly))
        std::cout << "Problem updating .xml file's data from memory" << std::endl;

    QTextStream ts(&file);
    ts << m_domFile.toString();

    file.close();
 }

// Retrieves given task's client.
QString WhchTableModel::clientOfTask(const QString task)
{
    QDomElement domRoot = m_domFile.firstChildElement("day");
    QString client;

    if (!domRoot.isNull())
    {
        QDomElement element = domRoot.firstChildElement("task");
        for (; !element.isNull(); element = element.nextSiblingElement("task"))
        {
            if (element.attribute("name").compare(task) == 0)
                return client = element.attribute("client");
        }
    }
    return client;
}
