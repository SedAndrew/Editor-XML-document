#ifndef TREEMODEL_H
#define TREEMODEL_H

#include <QAbstractItemModel>
#include <QModelIndex>
#include <QVariant>

// QAbstractItemModel::reset (используем beginResetModel/endResetModel)v

class Element;
class TreeModel : public QAbstractItemModel
{
  Q_OBJECT
public:
  TreeModel(const QStringList &strings, QObject *parent = nullptr);
  TreeModel(QObject *parent = nullptr);
  ~TreeModel();

  QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const;
  QModelIndex parent(const QModelIndex &index) const;

  int rowCount(const QModelIndex &parent = QModelIndex()) const;
  int columnCount(const QModelIndex &parent = QModelIndex()) const;

  QVariant data(const QModelIndex &index, int role) const;
  bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole);

  QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;

  bool hasChildren(const QModelIndex &parent = QModelIndex()) const;
  Qt::ItemFlags flags(const QModelIndex &index) const;

  bool insertRows(int position, int rows, const QModelIndex &parent = QModelIndex());
  bool removeRows(int position, int rows, const QModelIndex &parent = QModelIndex());

  /**
   * @brief
   * получить указатель на корневой элемент
   */
  Element *getRootNote();
  void setModelData(const QStringList &lines);

private:
  void setupModelData(const QStringList &lines, Element *parent);
  Element *rootItem;

};
#endif // TREEMODEL_H
