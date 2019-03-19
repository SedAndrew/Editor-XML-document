#include "models/table/tablemodel.h"

TableModel::TableModel(const QStringList &strings, QObject *parent)
  : QAbstractTableModel(parent)
{

}
TableModel::~TableModel()
{

}

QModelIndex TableModel::index(int row, int column, const QModelIndex &parent) const
{
}

int TableModel::rowCount(const QModelIndex &parent) const
{
}

int TableModel::columnCount(const QModelIndex &parent) const
{
}

QVariant TableModel::data(const QModelIndex &index, int role) const
{
}

bool TableModel::setItemData(const QModelIndex &index, const QMap<int, QVariant> &roles)
{
}

bool TableModel::insertRows(int row, int count, const QModelIndex &parent)
{
}

bool TableModel::removeRows(int row, int count, const QModelIndex &parent)
{
}

Qt::ItemFlags TableModel::flags(const QModelIndex &index) const
{
}
