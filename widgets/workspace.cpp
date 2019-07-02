#include "workspace.h"
#include "ui_workspaceui.h"

//#include "session.h"
#include "document.h"
#include "element.h"
#include <models/itemdelegatepaint.h>
#include "models/tree/treemodel.h"
#include "models/table/tablemodel.h"
#include "widgets/treemodelcompleter.h"
#include "widgets/addelementform.h"

#include <QAction>
#include <QSplitter>
#include <QTableView>
#include <QRandomGenerator>
#include <QMenu>

#include <QMessageBox>
#include <QDebug>
#include <QAbstractProxyModel>
#include <QStringListModel>

#include <QDirModel>

WorkSpace::WorkSpace(QWidget *parent)
  : QWidget(parent), m_workUI(new Ui::WorkSpaceUI),
    m_completer(nullptr), m_storableItem(nullptr)
{
  m_workUI->setupUi(this);
  m_workUI->m_tabWTables->setVisible(false);
  m_oldIndex = QModelIndex();

  connect(m_workUI->m_tabWTables, &TabWidget::siganlSelect,
          this, &WorkSpace::tabWidgetPressed);
}

WorkSpace::WorkSpace(Document *doc, QWidget *parent)
  : QWidget(parent), m_workUI(new Ui::WorkSpaceUI),
    m_completer(nullptr), m_storableItem(nullptr)
{
  m_workUI->setupUi(this);
  m_workUI->m_tabWTables->setVisible(false);
  // if several windows are open in the settings
  if (1) {
    m_workUI->m_pBHide->setHidden(true);
    m_workUI->m_pBDivid->setHidden(true);

  }
  m_doc = doc;
  m_oldIndex = QModelIndex();

  setFile(m_doc);

  connect(m_workUI->m_tabWTables, &TabWidget::siganlSelect,
          this, &WorkSpace::tabWidgetPressed);
}

WorkSpace::~WorkSpace()
{
  delete m_workUI;
}

void WorkSpace::setContextMenu(const QList<QAction *> &actions)
{
  m_workUI->m_treeView->addActions(actions);
  m_workUI->m_treeView->setContextMenuPolicy(Qt::ActionsContextMenu);

  for (QAction *action : m_workUI->m_treeView->actions()) {
    connect(action, SIGNAL(triggered()), SLOT(onAction()));
  }
}

void WorkSpace::setContextMenu(Element *element)
{
#ifndef QT_NO_CURSOR
    QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));
#endif

  QString operation = tr("Открыть");
  m_workUI->m_treeView->actions()[0]->setChecked(false);
  int count = m_workUI->m_tabWTables->tabBar()->count();
  for (int i = 0; i < count; ++i) {
    // ищем элемент среди открытых в QTabWidget и сравниваем internalId с текущим элементом
    if (m_workUI->m_tabWTables->tabToolTip(i).split(" ").last()
        == QString("%1").arg(m_currentIndex.internalId()))
    {
      operation = tr("Показать");
      m_workUI->m_treeView->actions()[0]->setChecked(true);
      break;
    }
  }

  QString name = "";
  if (m_currentItem != nullptr){
    for (QAction * action : m_workUI->m_treeView->actions())
      action->setEnabled(true);
    name = "\"" + element->data(1).toString() + "\"";
  }
  else
    for (QAction * action : m_workUI->m_treeView->actions())
      action->setDisabled(true);

  for (QAction *action: m_workUI->m_treeView->actions()) {
    if (action->isSeparator()) continue;
    QStringList strl = action->text().split(QRegExp("\\s"));  // список слов заголвка действия
    strl.replaceInStrings(QRegExp("^(\".*)$"), name);         // подмена шаблона с именем
    QString str = strl.front(); strl.pop_front();//front(); strl.pop_front();             // склеиваем список в строку
    if (str == tr("Открыть") || str == tr("Показать")) str = operation;
    str += " " + strl.join(" ");
    action->setText(str);
    if (m_storableItem == nullptr && action->objectName().contains("PastElement")) {
      action->setDisabled(true);
    }
  }

#ifndef QT_NO_CURSOR
  QApplication::restoreOverrideCursor();
#endif
}

void WorkSpace::setFile(Document *doc)
{

//  WorkSpace *wid = qobject_cast<WorkSpace*>(sender());
//  qDebug() << "m_currentPlace2: " << wid;

//  if (doc->path == "") return;

    m_workUI->m_lENameDoc->setText(doc->name);
    m_workUI->m_lENameDoc->setToolTip(doc->path);
    m_workUI->m_lENameDoc->setCursorPosition(0);
//    m_treeModel->setModelData(doc->text);
//    initTreeModel(doc);
    delete m_completer;
    m_completer = new TreeModelCompleter(this);
    m_completer->setMaxVisibleItems(10);

    initTreeModel(*doc);
    m_completer->setModel(m_treeModel);//new TreeModel(doc->text, m_completer
//    m_completer->setPopup(m_workUI->m_treeView);
    m_workUI->m_treeView->setModel(m_completer->model()); //m_treeModel

//    m_completer->setModel(initTreeModel(doc));
//    m_completer->setModel(new QDirModel(m_completer));

    m_completer->setSeparator(QLatin1String("/")); // CompletionPrefix(QLatin1String("/")); //
    m_completer->setCompletionPrefix(QLatin1String("/")); //
    QObject::connect(m_completer, SIGNAL(highlighted(QModelIndex)), SLOT(highlight(QModelIndex)));

    m_completer->setCaseSensitivity(Qt::CaseInsensitive);
    m_completer->setCompletionMode(QCompleter::UnfilteredPopupCompletion);
    m_completer->setWrapAround(true);

    m_workUI->m_lECompleter->setToolTip(tr("Используйте \"%1\" для перехода по узлам").arg(m_completer->separator()));

  //  m_completer->setWidget(m_workUI->m_lECompleter); //setCompleter(m_completer);

    connect(m_workUI->m_treeView->selectionModel(), &QItemSelectionModel::selectionChanged,
            this, &WorkSpace::updateActions);

    m_workUI->m_lECompleter->setCompleter(m_completer);
    m_workUI->m_lECompleter->setFocus();
    emit setCompleter(m_completer);

}

QAbstractItemModel *WorkSpace::initTreeModel(Document &doc)
{

#ifndef QT_NO_CURSOR
    QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));
#endif

  m_treeModel = new TreeModel(doc.text, m_completer);//doc.text,
  doc.refElement = getRootNote();
//  ItemDelegatePaint *delegate = new ItemDelegatePaint(this);
//  tree->setItemDelegate(delegate);


#ifndef QT_NO_CURSOR
  QApplication::restoreOverrideCursor();
#endif

  return m_treeModel;
}

bool WorkSpace::copy(Element *item, Element *node)
{

  for (int i = 0; i < item->getChildrenCount(); ++i) {
    // создаем новый элемент. Родителем является переданный в функуию элемент
    Element *temp(new Element(item->getChild(i)->getXml(), node)); // ->getParent()
    if (item->getChild(i)->isParent()) {
      copy(item->getChild(i), temp);
    }
    node->appChild(temp);
  }
  return true;
}

const QString &WorkSpace::getCurrentFileName() const
{
  return m_doc->name;
}

Document *WorkSpace::getCurrentFile() const
{
  return m_doc;
}

const uint &WorkSpace::getIndexWidget() const
{
  return m_indexWidget;
}

Element *WorkSpace::getRootNote()
{
  return m_treeModel->getRootNote();
}

void WorkSpace::on_m_pBDivid_clicked()
{

}

QCompleter *WorkSpace::completer()
{
  return  m_completer;
}

void WorkSpace::highlight(const QModelIndex &index)
{
  QAbstractItemModel *completionModel = m_completer->completionModel();
  QAbstractProxyModel *proxy = qobject_cast<QAbstractProxyModel *>(completionModel);
  QModelIndex sourceIndex = proxy->mapToSource(index);
//  m_workUI->m_treeView->scrollTo(index);//sourceIndex
  if (!proxy) return;
//  QModelIndex sourceIndex = proxy->mapToSource(index);
  m_workUI->m_treeView->selectionModel()->select(sourceIndex, QItemSelectionModel::ClearAndSelect | QItemSelectionModel::Rows);
//  m_workUI->m_treeView->scrollTo(index);//sourceIndex
  // m_workUI->m_lECompleter->setText(m_completer->currentCompletion());
}

void WorkSpace::onAction()
{
    QAction* objectName = qobject_cast<QAction*>(sender());
    int num = 0;
    for (QAction *action : m_workUI->m_treeView->actions()){
      if (action->objectName() == objectName->objectName()) break;
      num++;
    }
    switch (num) {
      case 0 :
          openTable();
        break;
      // 1 - сепаратор
      case 2:
          addElement();
        break;
      case 3:
          delElement();
        break;
      case 4:
          copyElement();
        break;
      case 5:
          pastElement();
        break;
      default :
        break;
    }
}

void WorkSpace::onAppliedAdd()
{
  if (AddElementForm *form = qobject_cast<AddElementForm*>(sender())) {
    QRegExp regx("^[^\\W\\d](\\w|\\-)*$");
    if (regx.indexIn(form->getName()) != 0) {
      QMessageBox::warning(this, tr("Ошибка заполнения!"),
                                 tr("Неверно введено имя тега.\n"
                                    "Проверьте, чтобы имя не начиналось с цифры,"
                                    "и чтобы в нем не было никаких спец сиволов, кроме"
                                    " _ и - "));
      qDebug() << "Error input tag name: " << form->getName();
      return;
    }
    form->accept();
  }
}

void WorkSpace::openTable()
{
  int count = m_workUI->m_tabWTables->tabBar()->count();
  // ищем элемент среди открытых в QTabWidget
  for (int i = 0; i < count; ++i) {
    if (m_workUI->m_tabWTables->tabToolTip(i).split(" ").last() ==
                          QString("%1").arg(m_currentIndex.internalId()))
    {
      m_workUI->m_tabWTables->setCurrentIndex(i);
      if (!m_workUI->m_pBShHTable->isChecked())
        m_workUI->m_pBShHTable->setChecked(true);
      return;
    }
  }
  // если таких нет, создаем новую вкладку
  m_tableElement = new QTableView(m_workUI->m_tabWTables);
  m_tableModel = new TableModel(*m_currentItem);
  m_tableElement->setModel(m_tableModel);
  setStyleSheetCage(m_tableElement);

  // настройка горизонтального и вертикального headeres
  QHeaderView *header;
  header = new QHeaderView(Qt::Horizontal);
  header->setStretchLastSection(true);
  m_tableElement->setHorizontalHeader(header);
  header = new QHeaderView(Qt::Vertical);
  header->setHidden(true);
  header->setResizeContentsPrecision(true);
  m_tableElement->setVerticalHeader(header);

  m_workUI->m_tabWTables->addTab(m_tableElement, m_currentItem->data(1).toString(), m_currentIndex);
  m_workUI->m_tabWTables->setCurrentIndex(count);

  m_workUI->m_tabWTables->setTabToolTip(count, tr("row: %1, index: %2")
                                                 .arg(m_currentIndex.row())
                                                 .arg(m_currentIndex.internalId()));

  if (!m_workUI->m_pBShHTable->isChecked())
    m_workUI->m_pBShHTable->setChecked(true);

  // Делегат
  m_tableElement->setItemDelegate(new TableItemDelegatePaint(m_tableElement));
}

void WorkSpace::addElement()
{
//  qDebug() << "add: " << item->data().toMap()["tag"].toString();
//  qDebug() << "index: " << m_currentIndex;
  AddElementForm m_addForm;
  connect(&m_addForm, SIGNAL(myAccept()), SLOT(onAppliedAdd()));
  switch (m_addForm.exec()) {
    case QDialog::Rejected:
      qDebug() << "Rejected";
      break;
    case QDialog::Accepted: {
        qDebug() << "Accepted";
        int pos = m_addForm.getStatusPosition() == 1 ? m_currentItem->getChildrenCount() : 0;
        m_treeModel->insertRows(pos, 1, m_currentIndex);
        QModelIndex index = m_treeModel->index(pos, 0, m_currentIndex);
        qDebug() << "xml: " << m_addForm.getXml();
        m_treeModel->setData(index, m_addForm.getXml());
        m_workUI->m_treeView->selectionModel()->setCurrentIndex(index,QItemSelectionModel::ClearAndSelect);
        break;
      }
    default:
      qDebug() << "Unexpected";
      break;
  }
}

void WorkSpace::delElement()
{
  qDebug() << "del: " << m_currentItem->data(1).toString();
  QTreeView *view = m_workUI->m_treeView;
  QModelIndex index = view->selectionModel()->currentIndex();

  QAbstractItemModel *model = view->model();
  if (model->removeRow(index.row(), index.parent()))
      updateActions();
}

void WorkSpace::copyElement()
{
  QTreeView *view = m_workUI->m_treeView;
  m_storableIndex  = view->selectionModel()->currentIndex();
  m_storableItem = static_cast<Element *>(m_storableIndex.internalPointer());
  qDebug() << "copy: " << m_storableItem->data(1).toString()
           << " index: " << m_storableIndex;

}

void WorkSpace::pastElement()
{
  qDebug() << "past " << m_storableItem->data(1).toString()
           << " in : " << m_currentItem->data(1).toString();
  if (!m_storableIndex.isValid()) return;

  QTreeView *view = m_workUI->m_treeView;
  QModelIndex index = m_currentIndex;
  QAbstractItemModel *model = view->model();

  Element *newItem(new Element(m_storableItem->getXml(), m_currentItem));
  copy(m_storableItem, newItem);

  m_currentItem->insertChild(0, newItem);

  QModelIndex child = model->index(0, 0, index);
  model->setData(child, newItem->data(1));
  view->selectionModel()->setCurrentIndex(model->index(0, 0, index),
                                          QItemSelectionModel::ClearAndSelect);
  updateActions();
}

void WorkSpace::foundItem(Element *element)
{
//  emit m_completer->highlighted("Документ/План");
  //  m_workUI->m_treeView->selectionModel()->setCurrentIndex(element);
}

void WorkSpace::setStyleSheetCage(QTableView *view)
{
  view->setStyleSheet(
    "QTableView {"
      "background-color: qlineargradient("
      "x1:0, y1:0, x2:1, y2:1,"
      "stop:0 #497a5d, stop:1 #497a5d"
      ");"
      "selection-background-color: qlineargradient("
        "x1:0.5, y1:0, x2:0.5, y2:1,"
          "stop:0 #257d48, stop:0.12 #2e6343,"
          "stop:0.88 #2e6343, stop:1 #257d48"
      ");"
      "selection-color: #e8e6d1"
    "}"
  );
  view->setWordWrap(true);
}

void WorkSpace::updateActions()
{
  QTreeView *view = m_workUI->m_treeView;
  bool hasSelection = !view->selectionModel()->selection().isEmpty();

  view->actions()[2]->setEnabled(hasSelection);
  view->actions()[3]->setEnabled(hasSelection);
  view->actions()[4]->setEnabled(hasSelection);

  bool hasCurrent = view->selectionModel()->currentIndex().isValid();
  view->actions()[1]->setEnabled(hasCurrent);

  if (hasCurrent) {
      view->closePersistentEditor(view->selectionModel()->currentIndex());

      int row = view->selectionModel()->currentIndex().row();
      int column = view->selectionModel()->currentIndex().column();
      if (view->selectionModel()->currentIndex().parent().isValid())
          m_workUI->m_lECurrentElement->setText(tr("Position: (%1,%2)").arg(row).arg(column));
      else
          m_workUI->m_lECurrentElement->setText(tr("Position: (%1,%2) in top level").arg(row).arg(column));
  }

}

void WorkSpace::mousePressEvent(QMouseEvent *event)
{
  if (event->button() == Qt::RightButton) {
//    setContextMenu(m_currentItem);
  }
}

void WorkSpace::tabWidgetPressed(const QModelIndex &index)
{
  m_workUI->m_treeView->setCurrentIndex(index);
}

void WorkSpace::on_m_treeView_pressed(const QModelIndex &index)
{
  Element *item = nullptr;
  if (index.isValid()){
//    m_oldItem = item;
    item = static_cast<Element*>(index.internalPointer());
  }
  m_currentItem = item;
  if (index != m_currentIndex) m_oldIndex = m_currentIndex;
  m_currentIndex = index;
  setContextMenu(m_currentItem);
}
