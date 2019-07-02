#ifndef SIDEBAR_H
#define SIDEBAR_H

#include <QWidget>

namespace Ui {
  class Sidebar;
}

struct Document;
struct Session;
//class WorkSpace;
class QListWidgetItem;
class Sidebar : public QWidget
{
  Q_OBJECT

public:
  explicit Sidebar(QWidget *parent = nullptr);
  ~Sidebar();

  void setContextMenu(QList<QAction *> &actions);
  void setContextMenu(const QString nameFile);

  QListWidgetItem *currentItem();

signals:
  void deleteLineList(const int&);  // сигнал предупреждающий об удалении документа
  void appendFile(const QString&);  // сигнал предупреждающий о добавлении нового документа
  void changedFile(const QString&); // ?!
//  void selectItem(const int&);      // сигнал предупреждающий о выборе документа
  void selectItem(Document*);      // сигнал предупреждающий о выборе документа

private slots:
  void on_m_listWidget_itemPressed(QListWidgetItem *item); // выбор документа

public slots:
//  void onAddItem(const QString &str);           // добавить документ
  void onAddItem(Document *doc);           // добавить документ
  void onDeleteItem();                          // удалить документ
//  void dropRow(const QModelIndex &index);
  /**
   * @brief onSelectItem
   * @param index
   *
   * Select line элемента по указанному индексу.
   *
   * Связи:
   *  Данный слот связан:
   *    с сигналом мышы (принимает индекс выделенного элемента);
   *    с сигналом changedFocus от mainwindow (принимает активный WorkSpace)
   *
   * Связывается с виджетами WorkSpace сигналом selectItem для предупреждения изменения.
   * --------------------------------------------------------------------------------------------
   *
   *
   */
  // Генерация сигнала выбора элемента списка
//  void onSelectItem(const QModelIndex &index);  // выбор документа по индексу -- !
//  void onSelectItem(WorkSpace *wd);             // выбор документа по наименованию -- !
  void onSelectItem(const QString& nameDoc);       // выбор документа по наименованию <--

public:
  Session *m_session;

private:
  Ui::Sidebar *ui;

};

#endif // SIDEBAR_H
