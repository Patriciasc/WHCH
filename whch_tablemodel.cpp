#include "whch_tablemodel.h"
#include <QDomDocument>
#include <QFile>
#include <iostream>
#include <QTextStream>
#include <QRegExpValidator>
#include <QRegExp>
#include <QTime>

whch_TableModel::whch_TableModel(QObject *parent)
    :QAbstractTableModel(parent)
{
    m_dom_file = QDomDocument("DOMtest");
    // Create a root element for the DOM.
    QDomElement root = m_dom_file.createElement("day");
    root.setAttribute("date",QDate::currentDate().toString("yyyy/MM/dd"));
    m_dom_file.appendChild(root);

    // Write result to an .xml file.
    QString filename("test.xml");
    QFile file(filename);
    if(!file.open(QIODevice::WriteOnly) )
        return;

    QTextStream ts(&file);
    ts << m_dom_file.toString();

    file.close();
    load_xml_file(filename);
}

whch_TableModel::whch_TableModel(const QString &filename,
                                 QObject *parent)
    :QAbstractTableModel(parent)
{
    load_xml_file(filename);
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
                return element.attribute("duration","jop");
                break;
            case 3:
                return element.attribute("client","--");
                break;
            case 4:
                return element.attribute("name","car");
                break;
            case 5:
                return element.firstChildElement("details").text();
                break;
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

    if ((index.column() == 0) || (index.column() == 1))
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
                    element.setAttribute("start",value.toTime().toString("hh:mm"));
                    emit dataChanged(index, index);
                    changed = true;
                case 1:
                    element.setAttribute("end",value.toTime().toString("hh:mm"));
                    emit dataChanged(index, index);
                    changed = true;
            }

            // Update change in .xml file.
            if (changed)
            {
                write_in_xml_file();
            }

    }
    return changed;
}

void whch_TableModel::set_new_task()
{
    // Create a root element for the DOM.
    QDomElement dom_root = m_dom_file.firstChildElement("day");

    // Set data. (FUNCION SET TASK)
    QDomElement task = m_dom_file.createElement( "task" );
    task.setAttribute("start", QTime::currentTime().toString("hh:mm"));
    task.setAttribute("end", QTime::currentTime().toString("hh:mm"));
    // I will need to make use of restart()/start()/elapse here.
    task.setAttribute("duration", "duration");
    task.setAttribute("name", "name");
    task.setAttribute("client", "Openismus");

    QDomElement details_tag = m_dom_file.createElement("details");
    //QString details_text = ui->lineEdit->text();
    QDomText details_text = m_dom_file.createTextNode("prueba");
    details_tag.appendChild(details_text);
    task.appendChild(details_tag);

    dom_root.appendChild(task);

    // Write result to an .xml file. (FUNCION WRITE_XML_FILE)
    QFile file( "test.xml" );
    if( !file.open(QIODevice::ReadWrite) )
        std::cout << "Error writing result to file" << std::endl;

    QTextStream ts( &file );
    ts << m_dom_file.toString();

    file.close();
    reset();
    load_xml_file("test.xml");
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
