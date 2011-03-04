#include "whchTreeModel.h"
#include "whchTreeNode.h"

WhchTreeModel::WhchTreeModel(QDomDocument document,
                             QObject *parent)
    : QAbstractItemModel(parent), m_domDocument(document)
{
}

WhchTreeModel::~WhchTreeModel()
{
}

QVariant WhchTreeModel::data(const QModelIndex &index,
                             int role) const
{
}

Qt::ItemFlags WhchTreeModel::flags(const QModelIndex &index) const
{
}

QVariant WhchTreeModel::headerData(int section,
                                   Qt::Orientation orientation,
                                   int role) const
{
}

QModelIndex WhchTreeModel::index(int row, int column,
                                 const QModelIndex &parent) const
{
}

QModelIndex WhchTreeModel::parent(const QModelIndex &child) const
{
}

int WhchTreeModel::rowCount(const QModelIndex &parent) const
{
}

int WhchTreeModel::columnCount(const QModelIndex &parent) const
{
}
