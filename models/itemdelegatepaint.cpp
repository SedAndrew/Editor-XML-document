#include "itemdelegatepaint.h"

#include <QComboBox>
#include <QPainter>
#include <QDebug>
#include <QSpinBox>
#include <QLineEdit>

TableItemDelegatePaint::TableItemDelegatePaint(QObject *parent)
  : QStyledItemDelegate (parent)
{

}

TableItemDelegatePaint::TableItemDelegatePaint(const QString &txt, QObject *parent)
  : QStyledItemDelegate (parent)
{

}

void TableItemDelegatePaint::paint(QPainter *painter, const QStyleOptionViewItem &option,
                              const QModelIndex &index) const
{
  painter->save();
  if(index.row() % 2 == 0)
    painter->fillRect(option.rect, QColor(195, 196, 141));
  else
    painter->fillRect(option.rect, QColor(173, 196, 141));

  painter->restore();

  QStyledItemDelegate::paint(painter, option, index);
}
//  QString txt = index.model()->data( index, Qt::DisplayRole ).toString();
//switch (option.state) {
//  case QStyle::State_HasFocus: {
//      painter->fillRect(option.rect, QColor(238, 240, 127));
//      painter->setFont(QFont("Time New Roman", 12, QFont::Bold));
//      break;
//    }
//}
//standard processing
// option.palette.highlight()); //QColor(238, 240, 127)
//  if( option.state & QStyle::State_Selected )//we need this to show selection // State_Selected
//  {
//    painter->fillRect(option.rect, option.palette.highlight()); //QColor(238, 240, 127)
//  }

QSize TableItemDelegatePaint::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{

}

QWidget *TableItemDelegatePaint::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
//  qDebug() << "HIE!";
//  if(index.column()==0)
//      return nullptr;
  if (index.column() == 2 && index.row() < index.model()->rowCount() - 2 && index.row() > 0) {
    QComboBox *editor = new QComboBox(parent);
    //  QSpinBox *editor = new QSpinBox(parent);
    //  editor->setMinimum(0);
    //  editor->setMaximum(100);

    editor->setEditable(true);
    editor->addItem("item1");
    editor->addItem("item2");
    editor->addItem("item3");
    return editor;
  }
  return new QLineEdit(parent);
}

void TableItemDelegatePaint::setEditorData(QWidget *editor, const QModelIndex &index) const
{
  QString value = index.model()->data(index, Qt::DisplayRole).toString();
          if (index.column() == 2 && index.row() < index.model()->rowCount() - 2 && index.row() > 0){
            QComboBox *combo = static_cast<QComboBox*>(editor);
            combo->setEditText(value);
          }
          else {
            QLineEdit *line = static_cast<QLineEdit*>(editor);
            line->setText(value);
            qDebug() << "line: " << line->text();
          }


//  int value = index.model()->data(index, Qt::EditRole).toInt();

//       QSpinBox *spinBox = static_cast<QSpinBox*>(editor);
//       spinBox->setValue(value);
}

void TableItemDelegatePaint::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
  if (index.column() == 2 && index.row() < index.model()->rowCount() && index.row() > 0){
        QComboBox *combo = static_cast<QComboBox*>(editor);
        model->setData(index, combo->currentText(), Qt::EditRole);
  }
  else {
    QLineEdit *line = static_cast<QLineEdit*>(editor);
        model->setData(index, line->text(), Qt::EditRole);
  }

//  QSpinBox *spinBox = static_cast<QSpinBox*>(editor);
//       spinBox->interpretText();

//       model->setData(index, spinBox->value(), Qt::EditRole);
}

void TableItemDelegatePaint::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
  editor->setGeometry(option.rect);
}
