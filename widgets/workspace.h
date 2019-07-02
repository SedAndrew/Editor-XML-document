#ifndef WORKSPACE_H
#define WORKSPACE_H

#include <QWidget>
#include <QMap>
#include <QFile>
#include <QModelIndex>


namespace Ui {
  class WorkSpaceUI;
}

class QSplitter;
class QMenu;
class QTableView;
class QCompleter;
class Element;
class TreeModel;
class TreeModelCompleter;
class TableModel;
class SideBar;
class AddElementForm;

struct Document;
struct Session;

class WorkSpace : public QWidget
{
  Q_OBJECT
public:
  explicit WorkSpace(QWidget *parent = nullptr);
  explicit WorkSpace(Document *doc, QWidget *parent = nullptr); // replace
  ~WorkSpace();

  void setContextMenu(const QList<QAction *> &actions);
  void setContextMenu(Element *element);


  QAbstractItemModel* initTreeModel(Document &doc);  // Init tree model

  bool copy(Element *item, Element *node);

  //===============================Get'еры===================================
  const QString &getCurrentFileName() const;            // get current fileName
  Document *getCurrentFile() const;            // get current file
  const uint &getIndexWidget() const;         //  получить индекс виджета
  /**
   * @brief
   * получить указатель на корневой элемент
   */
  Element *getRootNote();
  QCompleter *completer();

signals:
  void changeFile(const QString&);
  void changelECurrentElement(const QString&);
  void selectItem(Element*);
  void setCompleter(QCompleter *completer); // TreeModelCompleter


public  slots:
  //===============================Виджет===================================
  /**
   * @brief
   * Загрузка файла в текущий виджет
   *
   */
  void setFile(Document *doc = nullptr);
  /**
   * @brief setCurrentFile
   * @param index
   * @param wid
   *
   * Вызов initModel с параметром index Выделение
   * index  - индекс документа в массиве документов текущей сессии.
   * *wid   - указатель на виджет. (отказаться и брать значение о текщем виджете из main класса)
   *
   */
//  void setCurrentFile(const int &index, WorkSpace *wid = nullptr);            // set current file (ComboBox) -- ??
//  void setCurrentFile(QString &doc, WorkSpace *wid = nullptr);
//  void setCurentItem(Element *item);
  void on_m_pBDivid_clicked();                      // show a menu with options of split
  void updateActions();

  void highlight(const QModelIndex &index);
//===============================Редактирование===================================
  void onAction();
  void onAppliedAdd();
  void openTable();
  /**
   * @brief
   * Окно добавления нового элемента
   */
  void addElement();
  void delElement();
  void copyElement();
  void pastElement();
 //==============================Дополнительные функции============================
  void foundItem(Element *element);
  void setStyleSheetCage(QTableView *view);

protected:
  void mousePressEvent(QMouseEvent *event);

private slots:
  void tabWidgetPressed(const QModelIndex &index);
  void on_m_treeView_pressed(const QModelIndex &index);

public:
  Ui::WorkSpaceUI *m_workUI;

private:
  //===============================Документ===================================
  Document *m_doc;                  // загруженный документ
  TreeModelCompleter *m_completer;    //?! TreeModelCompleter
  TreeModel *m_treeModel;           // модель текущего документа
  TableModel *m_tableModel;           // модель текущего документа

  //====================================Виджет===================================
  QSplitter *m_splitter;            // ?!
  uint m_indexWidget;               // ?!

  //====================================Элемент===================================
  /**
   * @brief m_dWTableElement;
   * Боковая панель, в которой находятся представления тегов в табличной моделе
   */
  QTableView *m_tableElement;
  QModelIndex m_currentIndex;
  Element *m_currentItem;
  QModelIndex m_storableIndex;
  Element *m_storableItem;
  QModelIndex m_oldIndex;
  Element *m_oldItem;

  //===========================//===========================//===========================
//  QMap<QString, QFile *> m_files; // !

//  QVector<TableModel *> m_tableModels;    // ?!


};



#endif // WORKSPACE_H
