#ifndef TABLEMODEL_H
#define TABLEMODEL_H

#include <QAbstractTableModel>
#include <QModelIndex>
#include <QVariant>

class Element;
class TableModel : public QAbstractTableModel
{
public:
  TableModel(const QStringList &strings, QObject *parent = nullptr);
  ~TableModel();

  QModelIndex index(int row, int column, const QModelIndex &parent) const;
  int rowCount(const QModelIndex &parent) const;
  int columnCount(const QModelIndex &parent) const;
  QVariant data(const QModelIndex &index, int role) const;
  bool setItemData(const QModelIndex &index, const QMap<int, QVariant> &roles);
  bool insertRows(int row, int count, const QModelIndex &parent);
  bool removeRows(int row, int count, const QModelIndex &parent);
  Qt::ItemFlags flags(const QModelIndex &index) const;

private:
  void setupModelData(const QStringList &lines, Element *parent);
  Element *Item;

};

#endif // TABLEMODEL_H
