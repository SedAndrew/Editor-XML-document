#ifndef TREEITEMDELEGATEPAINT_H
#define TREEITEMDELEGATEPAINT_H

#include <QStyledItemDelegate>

class TreeItemDelegatePaint : public QStyledItemDelegate
{
public:
  explicit TreeItemDelegatePaint(QObject *parent = nullptr);
  TreeItemDelegatePaint(const QString &txt, QObject *parent = nullptr);

public:
  void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;
  QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const;
  QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const;
  void setEditorData(QWidget *editor, const QModelIndex &index) const;
  void updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const;
};

#endif // TREEITEMDELEGATEPAINT_H
