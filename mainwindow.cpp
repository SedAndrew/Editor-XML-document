#include "mainwindow.h"
#include "ui_mainui.h"

#include "session.h"
#include "document.h"

//#include "models/tree/treemodel.h"
#include "widgets/workspace.h"
#include "widgets/treemodelcompleter.h"
#include "widgets/controllingsessions.h"
#include "widgets/findform.h"
#include "element.h"
#include "xmlbuilder.h"

#include <QSplitter>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QFileDialog>
#include <QMessageBox>
#include <QSettings>
#include <QDebug>
#include <QSignalMapper>
#include <QListWidget>

#include <QMouseEvent>

MainWindow::MainWindow(QWidget *parent) :
  QMainWindow(parent),
  m_ui(new Ui::MainUI)
{
//  QMetaObject::connectSlotsByName(this);
  m_ui->setupUi(this);
  m_ui->m_widgetSidebar->setHidden(true);       // Hidden left Panel
  m_ui->m_actionStart->setChecked(true);        // Check pushbutton Start
  m_ui->m_actionEditor->setEnabled(false);      // Disabled pushbutton Editor

  // установка имени сессии по умолчанию
  m_nameSession = "Default";


  m_session = m_ui->m_widgetSidebar->m_session;
//  m_currentPlace = new WorkSpace;
  readWindowSettings();
  cfgEditPage();    // Setup edit page


  connect(m_ui->m_actionSearch, SIGNAL(triggered()), SLOT(openFindForm()));
  /**
   * @brief
   * получение сигнала от Sidebar'a
   */
  connect(m_ui->m_widgetSidebar, SIGNAL(selectItem(Document*)), SLOT(callbackSidebar(Document*)));
  connect(m_ui->m_widgetSidebar, SIGNAL(deleteLineList(int)), SLOT(closeFile(int)));

  connect(this, SIGNAL(changeFile(Document*)), m_currentPlace, SLOT(setFile(Document*)));
  connect(m_currentPlace, SIGNAL(setCompleter(QCompleter*)), SLOT(initCompleter(QCompleter*)));
  connect(this, SIGNAL(activetedDoc(QString)), m_ui->m_widgetSidebar, SLOT(onSelectItem(QString)));

  connect(m_ui->m_actionFind, SIGNAL(triggered()), SLOT(inputLineSearch()));

}

MainWindow::~MainWindow()
{
//  writeWindowSettings();
  delete m_ui;
}

void MainWindow::addNewWidgetAtCenter(QModelIndex index, QWidget *widget)
{
  Q_UNUSED(index)
  Q_UNUSED(widget)
}

void MainWindow::deleteWidgetFromCenter(QModelIndex index, QWidget *widget)
{
  Q_UNUSED(index)
  Q_UNUSED(widget)
}

void MainWindow::readWindowSettings()
{
//  QSettings settings("SOFTYRU", "Edit-XML-documents\\mainwindow");
  QSettings settings(ORGANIZATION_NAME, APPLICATION_NAME"\\mainwindow");

  restoreGeometry(settings.value("geometry").toByteArray());
  m_recentFiles = settings.value("recentFiles").toStringList();
//  m_lastFiles = settings.value("lastFiles").toStringList();
  m_lastSession = settings.value("lastSession").toString();

  initRecentDocActions();
  readSessionFile(m_mapSessions);

  /*
//  QMapIterator<QString, QStringList> i(m_mapSessions);
//  int numFile = 0;
//  while (i.hasNext()) { i.next();
//    QString nameSession = i.key();
//    QAction *action = new QAction(nameSession, m_ui->m_menuSessions);
//    action->setObjectName(tr("m_actionSessionNum_%1").arg(++numFile));
//    m_ui->m_menuSessions->addAction(action);
//  }

//  if (m_lastFiles.isEmpty())

//  foreach (const QString &file, m_lastFiles){
//    loadDoc(file);
//    m_ui->m_widgetSidebar->onAddItem(m_dataFile.last(), m_currentPlace);

//    m_ui->m_widgetSidebar->onAddItem(m_dataFile.last()->name);
//  }

//  bool showGrid = settings.value("showGrid", true).toBool();
//  showGridActions->setChecked(autoRecalc);
  */
}

void MainWindow::writeWindowSettings()
{
  QSettings settings(ORGANIZATION_NAME, APPLICATION_NAME"\\mainwindow");
  settings.setValue("geometry", saveGeometry());
  settings.setValue("recentFiles", m_recentFiles);
//  settings.setValue("lastFiles", m_lastFiles);
  settings.setValue("lastSession", m_nameSession);//m_lastSession
  if (m_ui->m_widgetSidebar->currentItem()) {
    settings.setValue("lastViewFile", m_ui->m_widgetSidebar->currentItem()->text());//m_currentPlace->getCurrentFileName());//m_lastViewFile
  }

  if (m_lastFiles.isEmpty()) return;
  writeSessionFile(m_mapSessions);

}

void fillTag(Element *node, XMLBuilder &builder) {
  for (int num = 0; num < node->getAttributes().count(); ++num)
    if(!node->getAttribut(num).toString().isEmpty())
      builder.attr(node->getAttribut(num).toString(),
                   node->getValue(num).toString());
  if(node->hasContent())
    builder.content(node->getContent().toString());
  if(node->hasComment())
    builder.comment(node->getComment().toString());
}

bool MainWindow::save(Element *node, XMLBuilder &builder)
{
  if (node == nullptr) return false;
  if (node->getTag().toString().isEmpty()) return false;
  builder.begin(node->getTag().toString());
      fillTag(node, builder);
      if (node->isParent()) {
        for (int i = 0; i < node->getChildrenCount(); ++i) {
          save(node->getChild(i), builder);
        }
      }
  builder.end();
  return true;
}
//          Element *item = node->getChild(i);
//          if (item->getChild(i)->isParent())
//            save(item->getChild(i), builder);
//          fillTag(item, builder);
//          builder.begin(item->getTag().toString());
//          builder.end();


void MainWindow::createData(const QString path) {
  Q_UNUSED(path)
//  QFile f1(path);
//  if(!f1.open(QIODevice::ReadOnly)) {
//    QMessageBox::warning(this, tr("Error"), tr("Access error!"), tr("Cannot read file %1:\n%2.")
//                                 .arg(f1.fileName())
//                                 .arg(f1.errorString()));
//    return;
//  }
//  QTextStream stream(&f1);
//  QStringList fullText;
//  while (!stream.atEnd()) { // пока не конец документа, заполняем список строками
//    QString input = stream.readLine() + "\n";
//    fullText << input;
//  }
//  f1.close();

//  Document *document = new Document(path);
//  Session *docs = mapDocs.contains(nameSession) ?
//        mapDocs[nameSession] : new Session;
//  docs->add(document);
//  mapDocs[nameSession] = docs;
}

void MainWindow::loadSession(const QString &name)
{

  for (QString path : m_mapSessions.value(name)) {
    Open_triggered(path);
  }
  QSettings settings(ORGANIZATION_NAME, APPLICATION_NAME"\\mainwindow");
  QString lastViewFile = settings.value("lastViewFile").toString();
  m_ui->m_widgetSidebar->onSelectItem(lastViewFile);

//  foreach (QString path, m_mapSessions[name]) {
//    loadDoc(path);
//  }
}

void MainWindow::initRecentDocActions()
{
  int numFile = 0;
  foreach (const QString &filename, m_recentFiles) {
   QAction *action = new QAction(filename, m_ui->m_menuRecent);
    action->setObjectName(tr("m_actionRecentFileNum_%1").arg(++numFile));
    m_ui->m_menuRecent->addAction(action);
    connect(action, SIGNAL(triggered()), SLOT(m_openFile_triggered()));
  }
}

void MainWindow::readSessionFile(QMap<QString, QStringList> &mapDocs) {
  QFile sessions(QString("%1/sessions.txt").arg(PRO_DIR_SETTING));
  if (!sessions.exists()) {
    QMessageBox::warning(this, tr("Error"), tr("Access error!"), tr("File not exists %1:\n%2.")
                                 .arg(sessions.fileName())
                                 .arg(sessions.errorString()));
    return;
  }
  if (!sessions.open(QFile::ReadOnly)) {
    QMessageBox::warning(this, tr("Error"), tr("Access error!"), tr("Cannot read file %1:\n%2.")
                                 .arg(sessions.fileName())
                                 .arg(sessions.errorString()));
      return;
  }
  QTextStream input(&sessions);

  QString nameSession;
  QStringList list;
  QStringList docs;


  while (!input.atEnd()) {
    list = input.readLine().split(" ");
    if (list[0] == "#") {
      qDebug() << "This is comment: " << list.join(" ");
      continue;
    } else if (list[0] == "s") {
      if (!docs.isEmpty()) mapDocs[nameSession] = docs;
      nameSession = list[1];
      docs.clear();
    } else if (list[0] == "d") {
      docs << list[1];
    }
  }
  if (!docs.isEmpty()) mapDocs[nameSession] = docs;

  sessions.close();

  QMapIterator<QString, QStringList> i(mapDocs);
  int numFile = 1;
  while (i.hasNext()) { i.next();
    QString nameSession = i.key();
    QAction *action = new QAction(nameSession, m_ui->m_menuSessions);
    action->setObjectName(tr("m_actionSessionNum_%1").arg(++numFile));
    m_ui->m_menuSessions->addAction(action);
    connect(action, SIGNAL(triggered()), SLOT(m_openFile_triggered()));
  }

//  m_ui->m_widgetSidebar->m_session->setFile(lastViewFile);
}

void MainWindow::writeSessionFile(QMap<QString, QStringList> &mapDocs)
{
  mapDocs[m_nameSession] = m_session->toStringList();
  QFile sessions(QString("%1/sessions.txt").arg(PRO_DIR_SETTING));
  if (!sessions.exists()) {
    QMessageBox::warning(this, tr("Error"), tr("Access error!"), tr("File not exists %1:\n%2.")
                                 .arg(sessions.fileName())
                                 .arg(sessions.errorString()));
    return;
  }
  if (!sessions.open(QIODevice::WriteOnly)) {
    QMessageBox::warning(this, tr("Error"), tr("Access error!"), tr("Cannot open file for write %1:\n%2.")
                                 .arg(sessions.fileName())
                                 .arg(sessions.errorString()));
      return;
  }
  // перезапись документов сессии
  QMapIterator<QString, QStringList> i(mapDocs);
  while (i.hasNext()) { i.next();
    QString header = QString("s %1\n").arg(i.key());
    QTextStream stream(&sessions);
//    stream << header;
//    for (QString path : m_mapSessions[header]) {
//      stream << "d " << path << "\n";
//    }
//    stream << "\n";

    stream << header;
    QStringList docs = i.value();
    foreach (QString path, docs)
      stream << "d " << path << "\n";
    stream << "\n";
  }
  sessions.close();
}

void MainWindow::on_m_actionClearPlaceRecent_triggered()
{
    qDebug() << "ClearPlaceRecent";
    m_recentFiles.clear();
    foreach (QAction *action, m_ui->m_menuRecent->actions()) {
      if (action != m_ui->m_actionClearPlaceRecent)
        m_ui->m_menuRecent->removeAction(action);
    }
}

void MainWindow::m_dividWorkSpace(const Qt::Orientation orient)
{
  Q_UNUSED(orient)

  QSplitter *splitter = new QSplitter(m_ui->m_pageEdit);
  splitter->setHandleWidth(2);

  //  connect(m_ui->m_widgetSidebar, SIGNAL(append(QString)), m_currentPlace, SLOT(appendFile(QString)));
}

//void MainWindow::highlight(const QModelIndex &index)
//{

//}

void MainWindow::foundItem(Element *item)
{
  qDebug() << "item->data(Element::Tag): " << item->data(1);
}

void MainWindow::cfgEditPage()
{
  m_layoutEditPage = new QHBoxLayout(m_ui->m_pageEdit);
  m_splitter = new QSplitter(m_ui->m_pageEdit);
  // Main layout at page edit
  m_layoutEditPage->setSpacing(2);
  m_layoutEditPage->setContentsMargins(0, 0, 0, 0);
  m_layoutEditPage->addWidget(m_splitter); // 0, 0, 1, 1
  // Root splitter
  m_splitter->setObjectName("m_splitter_1");
  m_splitter->setOrientation(Qt::Horizontal);
  m_splitter->setHandleWidth(2);

  // First visible widget
  QSplitter *splitter(new QSplitter(m_splitter));
  splitter->setObjectName("m_splitter_2");
  splitter->setOrientation(Qt::Horizontal);
  splitter->setHandleWidth(2);

  Document *doc = new Document("", "Нет документа", QStringList(""));
  WorkSpace *workSpace = new WorkSpace(doc);
  m_currentPlace = workSpace;
  workSpace->setObjectName("m_rootWorkSpace");

  splitter->addWidget(workSpace);
  m_splitter->addWidget(splitter);

  QAction *separator_0(new QAction(this));
  separator_0->setSeparator(true);

  // действия для контекстного меню WorkSpace->TreeView
  QList<QAction *> treeEdit;
  treeEdit.append(m_ui->m_actionShowTable);
  treeEdit.append(separator_0);
  treeEdit.append(m_ui->m_actionInsertElement);
  treeEdit.append(m_ui->m_actionDeleteElement);
  treeEdit.append(m_ui->m_actionCopyElement);
  treeEdit.append(m_ui->m_actionPastElement);
//  m_ui->m_actionPastElement->setObjectName("m_ui->m_actionPastElement");

  m_ui->m_menuEdit->addActions(treeEdit);
  workSpace->setContextMenu(m_ui->m_menuEdit->actions());
  workSpace->setFocus();

  QAction *separator_6(new QAction(this));
  separator_6->setSeparator(true);
  m_ui->m_menuEdit->addAction(separator_6);
  m_ui->m_menuEdit->addAction(m_ui->m_actionSearch);

  QAction *separator_1(new QAction(this));
  separator_1->setSeparator(true);
  m_ui->m_menuFile->addAction(separator_1);

  QAction *separator_2(new QAction(this));
  QAction *separator_3(new QAction(this));
  QAction *separator_4(new QAction(this));
  separator_2->setSeparator(true);
  separator_3->setSeparator(true);
  separator_4->setSeparator(true);
  // действия для контекстного меню SideBare
  QList<QAction *> sideBareEdit;
  sideBareEdit.append(m_ui->m_actionCopyPath);
  sideBareEdit.append(m_ui->m_actionCopyNameFile);
  sideBareEdit.append(separator_2);
  sideBareEdit.append(m_ui->m_actionSaveFile);
  sideBareEdit.append(m_ui->m_actionSaveFileAs);
  sideBareEdit.append(m_ui->m_actionSaveAll);
  sideBareEdit.append(m_ui->m_actionReloadeFile);
  sideBareEdit.append(separator_3);
  sideBareEdit.append(m_ui->m_actionCloseFile);
  sideBareEdit.append(m_ui->m_actionCloseAll);
  sideBareEdit.append(m_ui->m_actionCloseAllBesidesCurrent);
  sideBareEdit.append(separator_4);
  sideBareEdit.append(m_ui->m_actionVisionInExplorer);

  m_ui->m_menuFile->addActions(sideBareEdit);
  m_ui->m_widgetSidebar->setContextMenu(sideBareEdit);

  QAction *separator_5(new QAction(this));
  separator_5->setSeparator(true);
  m_ui->m_menuFile->addAction(separator_5);
  m_ui->m_menuFile->addAction(m_ui->m_actionExit);

  connect(m_ui->m_actionSaveFile, &QAction::triggered, this, &MainWindow::actionSaveFile_triggered);
  connect(m_ui->m_actionSaveFileAs, &QAction::triggered, this, &MainWindow::actionSaveFileAs_triggered);

//  m_currentPlace = workSpace;

}

void MainWindow::updateResentList(const QString &path) {

  if (m_recentFiles.indexOf(path) == -1) {  // если документ ранее не был открыт
    if (m_recentFiles.count() == 20) { // если достигнут лимит документов
      m_recentFiles.removeFirst();
      QList<QAction*> actions = m_ui->m_menuRecent->actions();  // получаем список указателей
      m_ui->m_menuRecent->actions().removeAt(1);  // удаляем первый элемент из списка
      delete m_ui->m_menuRecent->actions().at(1); // освобождаем память
      int i = 0;
      foreach (QAction* action, actions)  // перенумеруем действия
        if (action->text().indexOf("m_actionRecentFileNum_") == -1) continue; // исключаем действие очистки списка
        else action->setObjectName(tr("m_actionRecentFileNum_%1").arg(++i));
    }

    m_recentFiles << path;  // добавляем документ в список ранее открытых
    int num = m_recentFiles.count() + 1; // получаем номер для документа
    QAction *action = new QAction(path, m_ui->m_menuRecent); // создаем новое действие
    action->setObjectName(tr("m_actionRecentFileNum_%1").arg(num)); // даем действию имя
    m_ui->m_menuRecent->addAction(action); // добавляем в меню
    connect(action, SIGNAL(triggered()), SLOT(m_openFile_triggered())); // коннектим действие
  }
}

bool MainWindow::loadDoc(const QString &pathfile)
{
  Document *document;
  QString path = pathfile;
  if (path == "") {
    path = QFileDialog::getOpenFileName(this,      tr("Открыть файл..."),
                                        QString(), tr("XML (*.xml);;"));
    if (path == "") return false;
  }
  QFile *doc(new QFile(path));
  if (!doc->exists()) {
    QMessageBox::warning(this, tr("Access error!"), tr("File not exists %1\n%2.")
                                 .arg(doc->fileName())
                                 .arg(doc->errorString()));
    qDebug() << "File not found!";
    return false;
  }
  if (!doc->open(QFile::ReadOnly)) {
      QMessageBox::warning(this, tr("Error"), tr("Access error!"), tr("Cannot read file %1:\n%2.")
                                   .arg(doc->fileName())
                                   .arg(doc->errorString()));
      qDebug() << "Access error!";
      return false;
  }
  if (!m_session->existDoc(path)) {  // если этот документ новый
    m_mapSessions[m_nameSession].clear();
    m_lastFiles << path;  // добавление документа в текущую сессию
    m_mapSessions[m_nameSession]= m_lastFiles;
    QTextStream stream(doc); // использование QTextStream для чтения документа
    QStringList fullText;     // список строк документа
    while (!stream.atEnd()) { // пока не конец документа, заполняем список строками
      QString input = stream.readLine() + "\n";
      fullText << input;
    }

    document = new Document(path, doc->fileName().split("/").last(), fullText);
    // добавляем в массив текущей сессии новый документ

//    m_session->add(document);
    updateResentList(path);
  } else {
      QMessageBox::warning(this, tr("Внимание!"),tr("Файл уже загружен"));

      doc->flush();
      doc->close();

      document = m_session->getDoc(path);
      emit activetedDoc(document->name);
      return false; // выход из функции
  }

  doc->flush();
  doc->close();
  m_ui->m_widgetSidebar->onAddItem(document);
  emit changeFile(document);

  return true;
}

void MainWindow::on_m_pBOpenFile_pressed()
{
  Open_triggered();
}

void MainWindow::addNewFileName(const QString &path)
{
  Q_UNUSED(path)
}

void MainWindow::closeFile(const int &index)
{
  Q_UNUSED(index)
//  if ( m_dataFile.isEmpty() ) {
//    QMessageBox::warning(this, tr("Error"), tr("Access error!"),
//                         tr("ASSERT: \"!isEmpty()\""));
//    return;
//  }
//  for (int j = index; j < m_dataFile.size() - 1; ++j) {
//    m_dataFile[j] = m_dataFile.at(j + 1);
//  }
//  m_dataFile.pop_back();


}

void MainWindow::showStartWidget()
{
  if (m_ui->m_actionEditor->isEnabled()){
    // If pushbutton edit is enabled
    m_ui->m_actionEditor->setChecked(false);                  // uncheck
    for (QAction * action : m_ui->m_menuEdit->actions()) {
      if(action->isSeparator()) continue;
      action->setDisabled(true);
    }
  }
  m_ui->m_pBIconPanel->setDisabled(true);                     // Disabled pushbutton panel
  m_ui->m_stackedWidget->setCurrentWidget(m_ui->m_pageStart); // Show start page

  m_ui->m_pBIconPanel->setChecked(false);
  m_ui->m_widgetSidebar->setHidden(true);                       // Hidden left Panel
//  m_ui->m_pBIconPanel->setEnabled(false);
}

void MainWindow::showMainWidget()
{
  m_ui->m_actionStart->setChecked(false);                       // Pushbutton Start to unchecked
  m_ui->m_pBIconPanel->setEnabled(true);                        // Enabled pushbutton panel
  m_ui->m_pBIconPanel->setChecked(true);
  on_m_pBIconPanel_released();

  for (QAction * action : m_ui->m_menuFile->actions()) {
    action->setEnabled(true);
  }
  m_ui->m_menuEdit->actions().last()->setEnabled(true);         // Enabled Find
  m_ui->m_stackedWidget->setCurrentWidget(m_ui->m_pageEdit);    // Set page Edit
//  WorkSpace *workspace = m_ui->m_pageEdit->findChild<WorkSpace *>();
//  workspace->setFocus();
//    m_currentPlace->setFocus();
  m_ui->m_menuWindow->actions().last()->setEnabled(true);
  m_ui->m_lEFind->setEnabled(true);



}

void MainWindow::replaceTextFile(const QString &filename)
{
  Q_UNUSED(filename)
//  setting->setValue(filename, m_dataFile[filename]);
}

void MainWindow::on_m_actionStart_toggled(bool arg1)
{
  if (arg1) { // changed Start page
    showStartWidget();
  } else if (!m_ui->m_actionEditor->isChecked()) {
    m_ui->m_actionStart->setChecked(true);
  }
}

void MainWindow::on_m_actionEditor_toggled(bool arg1)
{
  if (arg1) { // changed Edit page
    showMainWidget();
  } else if (!m_ui->m_actionStart->isChecked()) {
    m_ui->m_actionEditor->setChecked(true);
  }
}

void MainWindow::on_m_pBNewFile_pressed()
{
  qDebug() << "create new file";
}

void MainWindow::on_m_pBLastSession_pressed()
{
  loadSession(m_nameSession);
}

void MainWindow::mouseMoveEvent(QMouseEvent *event)
{
  Q_UNUSED(event)
//  int x = event->x();
//  int y = event->y();
//  if (x >= m_ui->m_lNewFile->x() &&
//      x <= m_ui->m_lNewFile->x() + m_ui->m_lNewFile->geometry().x() &&
//      y >= m_ui->m_lNewFile->y() &&
//      y <= m_ui->m_lNewFile->y() + m_ui->m_lNewFile->geometry().y()) {
//    m_ui->m_lNewFile->set
//  }
}

bool MainWindow::exitApp() {
  int r = QMessageBox::question(
    this, tr("Подтвердите"),
    tr("Выйти из программы?"),
    QMessageBox::Yes,
    QMessageBox::Cancel | QMessageBox::Escape);

  if (r == QMessageBox::Yes) {
    writeWindowSettings();
    return true;
  }
  return false;
}

void MainWindow::on_m_actionExit_triggered()
{
  if (exitApp()) {
    exit(1);
  }
}

void MainWindow::on_m_pBIconPanel_released()
{
  if (m_ui->m_pBIconPanel->isChecked()) {
    m_ui->m_widgetSidebar->setHidden(false);
  }
  else {
    m_ui->m_widgetSidebar->setHidden(true);
  }
}

void MainWindow::callbackSidebar(Document *doc)
{
  if (doc->path == m_currentPlace->getCurrentFile()->path) return;
  emit changeFile(doc);
}

void MainWindow::Open_triggered(const QString &filePath)
{
  if (filePath == "" ? loadDoc() : loadDoc(filePath))
  {
    if (!m_ui->m_actionEditor->isEnabled()){
      m_ui->m_actionEditor->setEnabled(true); // If pusbutton edit disabled - enabled

    }
    if (!m_ui->m_actionEditor->isChecked()) { // If pushbutton edit uncheck
      m_ui->m_actionEditor->setChecked(true); // check
    }

    qDebug() << "m_currentPlace: " << m_currentPlace;
  }
}

void MainWindow::m_openFile_triggered()
{
  QAction* objectName = qobject_cast<QAction*>(sender());
  QString nameFile = objectName->text();
  qDebug() << "menu: " << objectName->parent()->objectName();
  if (objectName->parent()->objectName().contains("Sessions")) {
    for (QString path : m_mapSessions.value(nameFile)) {
      Open_triggered(path);
    }
    QSettings settings(ORGANIZATION_NAME, APPLICATION_NAME"\\mainwindow");
    QString lastViewFile = settings.value("lastViewFile").toString();
    m_ui->m_widgetSidebar->onSelectItem(lastViewFile);
  }
  else Open_triggered(nameFile);
}

void MainWindow::on_m_actionNew_triggered()
{
    on_m_pBNewFile_pressed();
}

void MainWindow::on_m_actionAboutQtCreator_triggered()
{
  QMessageBox* mess = new QMessageBox(this);
  mess->aboutQt(this,"Информация о программе Qt Creator");
}

void MainWindow::on_m_actionAboutEditorXMLDocuments_triggered()
{
    QMessageBox::about(
          this, tr("О программе"),
          tr("<h2>Приложение на Qt5</h2>"
             "<p>Полнофункциональное, кроссплатформенное приложение для редактированию"
             " XML документов с использованием системы виджетов для представления узлов."));
}

bool MainWindow::eventFilter(QObject *watched, QEvent *event)
{
  QList<WorkSpace *> workSpace = m_ui->m_pageEdit->findChildren<WorkSpace*>();
  if (!workSpace.isEmpty()) {
    foreach (WorkSpace *place, workSpace) {
      if (watched == place) {
        if (event->type() == QEvent::FocusIn) {
          m_currentPlace = place;
          m_ui->m_lEFind->setCompleter(m_currentPlace->completer());
          qDebug() <<"m_ui->m_lEFind->completer(): " << m_ui->m_lEFind->completer();
          emit activetedDoc(place->getCurrentFileName());
          break;
        }
      }
    }
  }
  return  false;
}

void MainWindow::on_m_actionOpen_triggered()
{
    Open_triggered();
}

void MainWindow::on_m_actionControlSessions_triggered()
{
    ControllingSessions form(this);
    form.exec();
}

void MainWindow::actionSaveFile_triggered()
{
//  QString path = m_ui->m_widgetSidebar->currentItem()->toolTip();
  QString path = qobject_cast<QAction*>(sender())->text().split("\"").at(1).trimmed();
  for (Document *doc : m_session->documents()) {
    if (doc->name == path) {
      path = doc->path;
      break;
    }
  }

  qDebug() << "path: " << path;
  QFile file(path);
  if(!file.open(QIODevice::WriteOnly | QIODevice::Truncate)) {
    qDebug() << file.errorString();
    qDebug() << path;
    return;
  }

  QTextStream stream(&file);
  QStringList text;
  Element *rootNode = m_session->getDoc(path)->refElement;

  qDebug() << "documents().count(): " << m_session->documents().count();
  QString firstLine = m_session->getDoc(path)->text.front();
  qDebug() << "instruction: " << firstLine;
  // выбираем атрибуты настройки из первой строки документа
  QRegExp regx2("<\\S+\\s([^>]+).>");
  QString instruction = "";
  if (regx2.indexIn(firstLine) != -1) {
    instruction = regx2.cap(1);
  }
  qDebug() << "regx2.cap(1): " << regx2.cap(1);
  XMLBuilder builder(instruction, "xml");
  // передаем первый корневой элемент дерева

  save(rootNode->getChild(0), builder);

  qDebug() << "text xml\n" << builder.getXML();
  stream << builder.getXML();
  file.flush();
  file.close();


}

void MainWindow::actionSaveFileAs_triggered()
{
  qDebug() << "actionSaveFileAs_triggered()";
}

void MainWindow::closeEvent(QCloseEvent *event)
{
  event->ignore();
  if (exitApp())
    event->accept();
}

void MainWindow::openFindForm()
{
  m_findForm = new FindForm(m_currentPlace->getRootNote(), this);
//  connect(m_findForm, SIGNAL(isNext(Element*)), SLOT(foundItem(Element*)));
  connect(m_findForm, &FindForm::isNext, this, &MainWindow::foundItem);
  m_findForm->exec();
//  switch() {
//    case QDialog::Rejected:
//      qDebug() << "Rejected";
//      break;
////    case QDialog::Accepted: {
////        qDebug() << "Accepted";
////        int direction = m_findForm->getStatusPosition();
//////        m_treeModel->insertRows(pos, 1, m_currentIndex);
//////        QModelIndex index = m_treeModel->index(pos, 0, m_currentIndex);
//////        m_treeModel->setData(index, addForm.getTag());
////        break;
////      }
//    default:
//        qDebug() << "Unexpected";
//        break;
//  }
  delete m_findForm;
}

void MainWindow::inputLineSearch()
{
  m_ui->m_lEFind->setFocus();
  m_ui->m_lEFind->setCursorPosition(0);
}

void MainWindow::initCompleter(QCompleter *completer)
{
  m_ui->m_lEFind->setCompleter(completer);
//  completer->setWidget(m_ui->m_lEFind);
  qDebug() <<"completer: " << completer;
  qDebug() <<"m_ui->m_lEFind->completer(): " << m_ui->m_lEFind->completer();
}

