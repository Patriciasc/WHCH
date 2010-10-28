#include "whch_tablemodel.h"
#include <QDomDocument>
#include <QFile>
#include <iostream>
#include <QTextStream>


whch_TableModel::whch_TableModel(QObject *parent)
    :QAbstractTableModel(parent)
{
    load_xml_file();
    QDomElement dom_root = m_dom_file.documentElement();
}

whch_TableModel::whch_TableModel(const QString &filename, QObject *parent)
    :QAbstractTableModel(parent)
{
    load_xml_file(filename);
    QDomElement dom_root = m_dom_file.documentElement();
}

int whch_TableModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);

    QDomElement dom_root = m_dom_file.documentElement();
    return dom_root.elementsByTagName("task").count();
}

int whch_TableModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return 6;
}

QVariant whch_TableModel::data(const QModelIndex &index, int role) const
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
                return element.attribute("start","--");
                break;
            case 1:
                return element.attribute("end","--");
                break;
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

QVariant whch_TableModel::headerData(int section, Qt::Orientation orientation, int role) const
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
        if (index.column() == 0)
        {
            element.setAttribute("start",value.toString());
            emit dataChanged(index, index);
            changed = true;
        }

        if (index.column() == 1)
        {
            element.setAttribute("end",value.toString());
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
