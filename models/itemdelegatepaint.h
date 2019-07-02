#ifndef ITEMDELEGATEPAINT_H
#define ITEMDELEGATEPAINT_H

#include <QStyledItemDelegate>

class TableItemDelegatePaint : public QStyledItemDelegate
{
  Q_OBJECT
public:
  explicit TableItemDelegatePaint(QObject *parent = nullptr);
  TableItemDelegatePaint(const QString &txt, QObject *parent = nullptr);

protected:
  QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const;
  void paint( QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index ) const;
  QSize sizeHint( const QStyleOptionViewItem &option, const QModelIndex &index ) const;
  void setEditorData(QWidget * editor, const QModelIndex & index) const;
  void setModelData(QWidget * editor, QAbstractItemModel * model, const QModelIndex & index) const;
  void updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const;

signals:

  public slots:

private:


};

#endif // ITEMDELEGATEPAINT_H
