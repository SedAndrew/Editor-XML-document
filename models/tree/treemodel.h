#ifndef TREEMODEL_H
#define TREEMODEL_H

#include <QAbstractItemModel>
#include <QModelIndex>
#include <QVariant>

class Element;
class TreeModel : public QAbstractItemModel
{
public:
  TreeModel(const QStringList &strings, QObject *parent = nullptr);
  ~TreeModel();

  QVariant data(const QModelIndex &index, int role) const;
  Qt::ItemFlags flags(const QModelIndex &index) const;
  QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;
  QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const;
  QModelIndex parent(const QModelIndex &index) const;
  int rowCount(const QModelIndex &parent = QModelIndex()) const;
  int columnCount(const QModelIndex &parent = QModelIndex()) const;
  bool insertRows(int position, int rows, const QModelIndex &parent = QModelIndex());
  bool removeRows(int position, int rows, const QModelIndex &parent = QModelIndex());
  bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole);

private:
  void setupModelData(const QStringList &lines, Element *parent);
  Element *rootItem;

};

#endif // TREEMODEL_H
