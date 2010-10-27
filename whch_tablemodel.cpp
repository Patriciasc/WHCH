#include "whch_tablemodel.h"
#include <QDomDocument>
#include <QFile>
#include <iostream>


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
    if((index.isValid()) && (role == Qt::DisplayRole))
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
    // XXX: I do not like the way this looks. Need to find something
    // that looks more natural.
    if ((index.column() == 0) || (index.column() == 1))
        return QAbstractItemModel::flags(index) | Qt::ItemIsEditable;

}
bool whch_TableModel::setData(const QModelIndex &index,
                              const QVariant &value,
                              int role)
{

}

// Load .xml file's content (data).
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
