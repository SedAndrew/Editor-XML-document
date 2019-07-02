#include "treeitemdelegatepaint.h"

TreeItemDelegatePaint::TreeItemDelegatePaint(QObject *parent)
{

}

TreeItemDelegatePaint::TreeItemDelegatePaint(const QString &txt, QObject *parent)
{

}


void TreeItemDelegatePaint::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
}

QSize TreeItemDelegatePaint::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
}

QWidget *TreeItemDelegatePaint::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
}

void TreeItemDelegatePaint::setEditorData(QWidget *editor, const QModelIndex &index) const
{
}

void TreeItemDelegatePaint::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
}
