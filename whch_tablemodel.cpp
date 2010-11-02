#include "whch_tablemodel.h"
#include "whch_task.h"
#include <QDomDocument>
#include <QFile>
#include <iostream>
#include <QTextStream>
#include <QRegExpValidator>
#include <QRegExp>
#include <QTime>

static const QString FILENAME = "whch_log.xml";

whch_TableModel::whch_TableModel(QObject *parent)
    :QAbstractTableModel(parent)
{
    m_task = m_dom_file.createElement( "task" );

    m_task.setAttribute("start", QTime::currentTime().toString("hh:mm"));

    m_timer.start();

    /* Create .xml file if it does not exist. */
    QFile file(FILENAME);
    if(!file.exists())
    {
        m_dom_file = QDomDocument("DOMtest");
        // Create a root element for the DOM.
        QDomElement root = m_dom_file.createElement("day");
        root.setAttribute("date",QDate::currentDate().toString("yyyy/MM/dd"));
        m_dom_file.appendChild(root);

        // Write result to an .xml file.
        if(!file.open(QIODevice::WriteOnly) )
            return;

        QTextStream ts(&file);
        ts << m_dom_file.toString();

        file.close();
    }

    /* Load .xml file in memory. */
    load_xml_file(FILENAME);
}

int whch_TableModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);

    QDomElement dom_root = m_dom_file.firstChildElement("day");
    return dom_root.elementsByTagName("task").count();
}

int whch_TableModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return 6;
}

QVariant whch_TableModel::data(const QModelIndex &index,
                               int role) const
{
    if((index.isValid()) && ((role == Qt::DisplayRole) || (role == Qt::EditRole)))
    {
        // Get index element from file.
        QDomElement dom_root = m_dom_file.firstChildElement("day");
        QDomElement element = dom_root.firstChildElement("task");

        for(int i=1; i<=index.row(); i++ )
        {
            element = element.nextSiblingElement("task");
        }

        // Display index value.
        switch (index.column())
        {
            case 0:
            {
                QString time_string = element.attribute("start","");
                QTime time = QTime::fromString(time_string, "hh:mm");
                return time;
                break;
            }
            case 1:
            {
                QString time_string = element.attribute("end","");
                QTime time = QTime::fromString(time_string, "hh:mm");
                return time;
                break;
            }
            case 2:
                return element.attribute("duration","--");
            case 3:
                return element.attribute("client","--");
            case 4:
                return element.attribute("name","--");
            case 5:
                return element.firstChildElement("details").text();
            default:
                return QVariant();
        }
    }
    return QVariant();
}

QVariant whch_TableModel::headerData(int section,
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

Qt::ItemFlags whch_TableModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return Qt::ItemIsEnabled;

    if ((index.column() == 0) || (index.column() == 1) || (index.column() == 5))
        return QAbstractItemModel::flags(index) | Qt::ItemIsEditable;
    else
        return Qt::ItemIsEnabled;

}

bool whch_TableModel::setData(const QModelIndex &index,
                              const QVariant &value,
                              int role)
{
    bool changed = false;

    if (index.isValid() && role == Qt::EditRole)
    {
            // Get current index element from memory.
            QDomElement dom_root = m_dom_file.firstChildElement("day");
            QDomElement element = dom_root.firstChildElement("task");
            for(int i=1; i<=index.row(); i++ )
            {
                element = element.nextSiblingElement("task");
            }

            // Replace new value in memory.
            switch (index.column())
            {
                case 0:
                {
                    element.setAttribute("start",value.toTime().toString("hh:mm"));
                    //XXX Check duration: not working.
                    /*QVariant end_time = element.attribute("end");
                    QVariant start_time = element.attribute("start");
                    int algo = start_time.toTime().msec();
                    int algo2 = end_time.toTime().msec();
                    element.setAttribute("duration",QString("%1").arg(algo2));*/

                    emit dataChanged(index, index);
                    changed = true;
                    break;
                }
                case 1:
                    element.setAttribute("end",value.toTime().toString("hh:mm"));
                    //XXX Check duration.
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
            }

            // Update change in .xml file.
            if (changed)
            {
                write_in_xml_file(FILENAME);
            }

    }
    return changed;
}

void whch_TableModel::set_new_task(whch_task current_task)
{
    // Create a root element for the DOM.
    QDomElement dom_root = m_dom_file.firstChildElement("day");

    // Set data. (FUNCION SET TASK)
    //m_task = m_dom_file.createElement( "task" );

    //m_task.setAttribute("start", QTime::currentTime().toString("hh:mm"));
    m_task.setAttribute("end", QTime::currentTime().toString("hh:mm"));

    // I will need to make use of restart()/start()/elapse here.
    int ms = m_timer.elapsed();
    int s = ms / 1000; ms %= 1000;
    int m = s / 60; s %= 60;
    int h = m / 60; m %= 60;
    m_task.setAttribute("duration",QString("%1:%2:%3").arg(h).arg(m).arg(s));
    m_task.setAttribute("name", current_task.name);
    m_task.setAttribute("client", "Openismus");

    QDomElement details_tag = m_dom_file.createElement("details");
    QDomText details_text = m_dom_file.createTextNode(current_task.details);
    details_tag.appendChild(details_text);
    m_task.appendChild(details_tag);

    dom_root.appendChild(m_task);

    // Write result to an .xml file. (FUNCION WRITE_XML_FILE)
    QFile file(FILENAME);
    if( !file.open(QIODevice::ReadWrite) )
        std::cout << "Error writing result to file" << std::endl;

    QTextStream ts( &file );
    ts << m_dom_file.toString();

    file.close();
    reset();
    load_xml_file(FILENAME);

    // Set start time for the next task.
    m_task = m_dom_file.createElement( "task" );
    m_task.setAttribute("start", QTime::currentTime().toString("hh:mm"));
    m_timer.start();
}

// Load .xml file's content (data) in memory.
void whch_TableModel::load_xml_file(const QString &filename)
{
    QFile file(filename);
    if( !file.open(QIODevice::ReadOnly) )
    {
        std::cout << "Could not open file" << std::endl;
    }
    if( !m_dom_file.setContent( &file ) )
    {
        std::cout << "Problem setting content" << std::endl;
        file.close();
    }
    file.close();
}

// Update .xml file's content with the data in memory.
void whch_TableModel::write_in_xml_file (const QString &filename)
{
        QFile file(filename);
        if( !file.open(QIODevice::WriteOnly))
            std::cout << "Problem updating .xml file's data from memory" << std::endl;

        QTextStream ts(&file);
        ts << m_dom_file.toString();

        file.close();
 }
