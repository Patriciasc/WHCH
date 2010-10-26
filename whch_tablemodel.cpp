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
    if (!index.isValid())
        return QVariant();

    if (index.row() >= m_display_list.size() || index.row() < 0)
        return QVariant();

    if (role == Qt::DisplayRole)
    {
        QString  myString = m_display_list.at(index.row());

        /* XXX:test 2 columns only. */
        if (index.column() == 0)
            return myString;
        else if (index.column() == 1)
            return myString;
    }
    return QVariant();
}

QVariant whch_TableModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role != Qt::DisplayRole)
        return QVariant();

    if (orientation == Qt::Horizontal)
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
