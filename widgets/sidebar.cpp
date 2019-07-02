#include "sidebar.h"
#include "ui_sidebar.h"
//#include "workspace.h"
//#include "mainwindow.h"
#include "session.h"
#include "document.h"

#include <QListWidget>
#include <QShortcut>
#include <QKeySequence>
#include <QDebug>
#include <QMouseEvent>

Sidebar::Sidebar(QWidget *parent) :
  QWidget(parent),
  ui(new Ui::Sidebar)
{
  ui->setupUi(this);
//  setContextMenu("");

  m_session = new Session();
  ui->m_listView->setHidden(true);
//  connect(ui->m_listWidget, SIGNAL(doubleClicked(QModelIndex)), SLOT(dropRow(QModelIndex)));
  QShortcut* shortcut = new QShortcut( QKeySequence( Qt::Key_Delete ), ui->m_listWidget );
  connect(shortcut, SIGNAL( activated() ), this, SLOT( onDeleteItem() ));

//  connect(ui->m_listWidget, SIGNAL(clicked(QModelIndex)), SLOT(onSelectItem(QModelIndex)));
}

Sidebar::~Sidebar()
{
  delete ui;
}

void Sidebar::setContextMenu(QList<QAction *> &actions)
{
  ui->m_listWidget->addActions(actions);
  ui->m_listWidget->setContextMenuPolicy(Qt::ActionsContextMenu);
}

void Sidebar::setContextMenu(const QString nameFile)
{
  QList<QAction *> actions = ui->m_listWidget->actions();
  actions[3]->setText(tr("Сохранить %1").arg(nameFile));
  actions[4]->setText(tr("Сохранить %1 как...").arg(nameFile));
  actions[6]->setText(tr("Перезагрузить %1").arg(nameFile));
  actions[8]->setText(tr("Закрыть %1").arg(nameFile));
  actions[10]->setText(tr("Закрыть все кроме %1").arg(nameFile));
}

QListWidgetItem *Sidebar::currentItem()
{
  return ui->m_listWidget->currentItem();
}



void Sidebar::onAddItem(Document *doc)
{
  // Загрузка документов не учитыавет случай одинаковых имен. Для этого нужно сравнить пути с одинаковыми
  // именами документов и перезаписать имена документов так, чтобы имя содержало путь от директории до файла.

    m_session->add(doc);
  qDebug() << "Add : " << doc->name;
  ui->m_listWidget->addItem(doc->name);

  // ищем все элементы с именем искомого документа
  QList<QListWidgetItem *> items =
        ui->m_listWidget->findItems(QString("%1").arg(doc->name),//path.split("/").last()
                                    Qt::MatchEndsWith | Qt::MatchWrap | Qt::MatchWildcard);
    items.back()->setToolTip(doc->path);

  // выбор документа
  for (QListWidgetItem *item : items) {
    if (item->toolTip() == doc->path) {
      ui->m_listWidget->setItemSelected(item, true);
      on_m_listWidget_itemPressed(item);
      break;
    }
  }
  if (items.size() > 1) { // если искомый документ имеет имя уже добавленного элемента
    QVector<Document *> data = m_session->documents();
    for (int i = 0; i < data.size() - 1; ++i) {
      Document *doc1 = data[i],
               *doc2 = data[i+1];
      QString name1 = doc->name
            , name2 = doc->name;   // новые имена документов
      QStringList split1 = doc1->path.split("/"),    // разбитый по каталогам путь к документу
                  split2 = doc2->path.split("/");
      for (int j = 1; j < split1.size(); ++j) {      // цикл по размеру вложенности первого документа
        QString dirDoc1 = split1.at(split1.size() - j),   // получаем следующую директорию по вложенности
                dirDoc2 = split2.at(split2.size() - j);
        name1 = dirDoc1 + "/" + name1;
        name2 = dirDoc2 + "/" + name2;
        if (dirDoc1 != dirDoc2) {
          for (QListWidgetItem *item : items) {
            if (item->toolTip() == doc1->path) {
              if (doc1->name != name1) { // если имя документа изменилось
                doc1->name = name1;
                item->setText(name1);
              }
//              break;
            }
            if (item->toolTip() == doc2->path) { //  && j == data.size() - 1
              if (doc2->name != name2) {
                doc2->name = name2;
                item->setText(name2);
              }
              break;
            }
          }
        }
      }
    }
  }

  ui->m_listWidget->sortItems();

//  ui->m_listWidget->setCurrentItem(item);
//  onSelectItem(wd);

//  QPoint point(0, ui->m_listWidget->currentRow());
//  QModelIndex index = ui->m_listWidget->indexAt(point);//ui->m_listWidget->currentIndex();

//  onSelectItem(index);

//  onSelectItem(wd);
//  emit appendFile(str);
}


void Sidebar::onDeleteItem()
{
  int row;
//  if (ui->m_listWidget) {
    QListWidgetItem *item = ui->m_listWidget->selectedItems().back();
    row = ui->m_listWidget->row(item);
    if (ui->m_listWidget->takeItem(row)) {
      delete item;
      ui->m_listWidget->sortItems();
      emit deleteLineList(row);
    }
//  }
}

/**void Sidebar::dropRow(const QModelIndex &index)
//{
//  if( !index.isValid() ) {
//      return;
//  }

//  if( QListWidget* listWgt = dynamic_cast< QListWidget* >( sender() ) ) {
//      if( QListWidgetItem* item = listWgt->takeItem( index.row() ) ) {
//          delete item;
//      }
//  }
//  emit deleteRowList(index.row());
//}
**/

/*
//void Sidebar::onSelectItem(const QModelIndex &index)
//{
//  if (!index.isValid()) {
//    return;
//  }
//  QString name = index.data().toString();
//  foreach (Document *d, m_session->documents()) {
//    if (d->name == name) {
//      emit selectItem(d);
//      break;
//    }
//  }
//}
*/
/*
void Sidebar::onSelectItem(WorkSpace *wd)
{
  QString nameDoc = wd->getCurrentFile();
  QModelIndex index;
  QList<QListWidgetItem *> items =
        ui->m_listWidget->findItems(nameDoc, Qt::MatchFixedString);//Qt::MatchWrap | Qt::MatchWildcard);
  if (items.isEmpty()) {
    qDebug() << "Not can found document in list widget.";
    return;
  }
  foreach(QListWidgetItem *item, items)
  {
    if (item->text() == nameDoc)
      ui->m_listWidget->setItemSelected(item, true);
  }
  index = ui->m_listWidget->currentIndex();
//  emit selectItem(index.row());
//  emit selectItem(nameDoc);

//  QListWidgetItem *item = ui->m_listWidget->findItems(nameDoc, Qt::MatchFixedString).back();

//  ui->m_listWidget->setItemSelected(item, true);


//  QListWidgetItem *docs = ui->m_listWidget->currentItem();
//  int num = 0;
//  QString doc;
//  foreach (doc : docs) {
//    if (nameDoc.indexOf(doc) != -1) { // поиск нужного документа
//      ui->m_listWidget->setCurrentRow(num);
//      break;
//    }
//    num++;
//  }
}
*/
void Sidebar::onSelectItem(const QString &nameDoc)
{
  QList<QListWidgetItem *> items =
        ui->m_listWidget->findItems(QString("*"), Qt::MatchWrap | Qt::MatchWildcard);
  if (items.isEmpty()) {
    qDebug() << "Not can found document in list widget.";
    return;
  }

  for (QListWidgetItem *item : items)
    if (item->text() == nameDoc) {
      ui->m_listWidget->setItemSelected(item, true);
      foreach (Document *d, m_session->documents()) {
        if (d->name == nameDoc) {
          emit selectItem(d);
          break;
        }
      }
    }
}

void Sidebar::on_m_listWidget_itemPressed(QListWidgetItem *item)
{
  QString nameFile = "\"" + item->text() + "\"";
  setContextMenu(nameFile);
  foreach (Document *d, m_session->documents())
    if (d->name == item->text()) {
      emit selectItem(d);
      break;
    }
}
