#include "mainwindow.h"
#include "ui_mainui.h"
#include "workspace.h"
#include "models/tree/treemodel.h"
#include "widgets/lableclicked.h"

#include <QSplitter>
#include <QGridLayout>
#include <QPushButton>
#include <QFileDialog>
#include <QMessageBox>
#include <QSettings>
#include <QDebug>

#include <QMouseEvent>

static QSettings *setting(new QSettings("FileName", "TextFile"));

MainWindow::MainWindow(QWidget *parent) :
  QMainWindow(parent),
  m_ui(new Ui::MainUI)
{
  m_ui->setupUi(this);
  m_ui->m_widgetPanel->setHidden(true);         // Hidden left Panel
  m_ui->m_actionEditor->setEnabled(false);      // Disabled pushbutton Editor
  m_ui->m_actionStart->setChecked(true);        // Check pushbutton Start

  m_gridLayout = new QGridLayout(m_ui->m_pageEdit);
  m_workSpace = new WorkSpace;
  m_gridLayout->addWidget(m_workSpace);

//  connect(m_ui->m_lOpenFile, SIGNAL(clicked()), this, SLOT(on_m_OpenFile_clicked()));
}

MainWindow::~MainWindow()
{
  delete m_ui;

}

void MainWindow::addNewWidgetAtCenter(QModelIndex index, QWidget *widget)
{

}

void MainWindow::deleteWidgetFromCenter(QModelIndex index, QWidget *widget)
{

}

bool MainWindow::loadFile()
{
  QString path = QFileDialog::getOpenFileName(this,      tr("Открыть файл..."),
                                      QString(), tr("XML (*.xml);;Все файлы (*)"));
  if (path.isEmpty())
  {
      qDebug() << "No file name specified";
      return false;
  }
  QFile file(path);
  if (!file.exists()){
      qDebug() << "File not found!";
      return false;
  }
  if (!file.open(QFile::ReadOnly)) {
      QMessageBox::warning(this, tr("Error"), tr("Access error!"), tr("Cannot read file %1:\n%2.")
                                   .arg(file.fileName())
                                   .arg(file.errorString()));
      qDebug() << "Access error!";
      return false;
  }
  addNewFileName(path);

  QTextStream stream(&file);
  QStringList fullText;
  while (!stream.atEnd()) {
      QString input = stream.readLine();
      QString indent = ""; // отступ
      while ( input[0] == "\t" ) {
          input.remove(0,1);
          indent += " ";
      }
      input = indent + input + "\n";
      fullText << input;
  }
  m_dataFile.insert(path, fullText);

  TreeModel *model(new TreeModel(fullText));

  file.flush();
  file.close();
  return true;
}

void MainWindow::on_m_pBOpenFile_pressed()
{
  if (loadFile())
  {
    if (!m_ui->m_actionEditor->isEnabled())   // If pusbutton edit disabled
      m_ui->m_actionEditor->setEnabled(true); // enabled
    if (!m_ui->m_actionEditor->isChecked()) { // If pushbutton edit uncheck
      m_ui->m_actionEditor->setChecked(true); // check
    }
  }
}

void MainWindow::addNewFileName(const QString &path)
{

}

void MainWindow::deleteFileName(const QString &path)
{

}

void MainWindow::showStartWidget()
{
  if (m_ui->m_actionEditor->isEnabled())                      // If pushbutton edit is enabled
    m_ui->m_actionEditor->setChecked(false);                  // uncheck
  m_ui->m_pBIconPanel->setEnabled(false);                     // Disabled pushbutton panel
  m_ui->m_stackedWidget->setCurrentWidget(m_ui->m_pageStart); // Show start page
}

void MainWindow::showMainWidget()
{
  m_ui->m_actionStart->setChecked(false);                       // Pushbutton Start to unchecked
  m_ui->m_pBIconPanel->setEnabled(true);                        // Enabled pushbutton panel
  m_ui->m_stackedWidget->setCurrentWidget(m_ui->m_pageEdit);    // Set page Edit

//  QSplitter *m_splitter_1(new QSplitter(m_ui->centralWidget));

//  m_gridLayout->addWidget(m_splitter_1);
//  m_gridLayout->addWidget(m_pBOpenFile);
//  m_gridLayout->setMargin(1);
//  m_gridLayout->setSpacing(0);
}

void MainWindow::replaceTextFile(const QString &filename)
{
  setting->setValue(filename, m_dataFile[filename]);
}

void MainWindow::on_m_actionStart_toggled(bool arg1)
{
  qDebug() << "This S : " << arg1;
  if (arg1) { // changed Start page
    showStartWidget();
  } else if (!m_ui->m_actionEditor->isChecked()) {
    m_ui->m_actionStart->setChecked(true);
  }
}

void MainWindow::on_m_actionEditor_toggled(bool arg1)
{
  qDebug() << "This E : " << arg1;
  if (arg1) { // changed Edit page
    showMainWidget();
  } else if (!m_ui->m_actionStart->isChecked()) {
    m_ui->m_actionEditor->setChecked(true);
  }
}

void MainWindow::on_m_NewFile_pressed()
{

}

void MainWindow::on_m_LastSetion_pressed()
{

}

void MainWindow::mouseMoveEvent(QMouseEvent *event)
{
  int x = event->x();
  int y = event->y();
//  if (x >= m_ui->m_lNewFile->x() &&
//      x <= m_ui->m_lNewFile->x() + m_ui->m_lNewFile->geometry().x() &&
//      y >= m_ui->m_lNewFile->y() &&
//      y <= m_ui->m_lNewFile->y() + m_ui->m_lNewFile->geometry().y()) {
//    m_ui->m_lNewFile->set
//  }
}

void MainWindow::on_m_actionExit_triggered()
{
    close();
}

void MainWindow::on_m_pBIconPanel_released()
{
  if (m_ui->m_pBIconPanel->isChecked()) {
    m_ui->m_widgetPanel->setHidden(false);
  }
  else {
    m_ui->m_widgetPanel->setHidden(true);
  }
}


