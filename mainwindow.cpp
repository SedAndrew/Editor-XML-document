#include "mainwindow.h"
#include "ui_mainui.h"
#include "workspace.h"
#include "models/tree/treemodel.h"

#include <QSplitter>
#include <QGridLayout>
#include <QPushButton>
#include <QFileDialog>
#include <QMessageBox>
#include <QSettings>
#include <QDebug>

static QSettings *setting(new QSettings("FileName", "TextFile"));

MainWindow::MainWindow(QWidget *parent) :
  QMainWindow(parent),
  m_ui(new Ui::MainUI)
{
  m_ui->setupUi(this);
  m_ui->m_actionEditor->setEnabled(false);
  m_gridLayout = new QGridLayout(m_ui->m_pageEdit);
  m_workSpace = new WorkSpace;
  m_gridLayout->addWidget(m_workSpace);

  showStartWidget();
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

void MainWindow::addNewFileName(const QString &path)
{

}

void MainWindow::deleteFileName(const QString &path)
{

}

void MainWindow::showStartWidget()
{
  m_ui->m_stackedWidget->setCurrentWidget(m_ui->m_pageStart);
}

void MainWindow::showMainWidget()
{
  QSplitter *m_splitter_1(new QSplitter(m_ui->centralWidget));

  m_gridLayout->addWidget(m_splitter_1);
  m_gridLayout->addWidget(m_pBOpenFile);
  m_gridLayout->setMargin(1);
  m_gridLayout->setSpacing(0);
}

void MainWindow::replaceTextFile(const QString &filename)
{
  setting->setValue(filename, m_dataFile[filename]);
}

void MainWindow::on_m_actionStart_triggered()
{
  if (m_ui->m_actionStart->isChecked())
  {
    m_ui->m_stackedWidget->setCurrentWidget(m_ui->m_pageStart);
    m_ui->m_actionEditor->setChecked(false);
  }
}

void MainWindow::on_m_actionEditor_triggered()
{
  if (m_ui->m_actionEditor->isChecked())
  {
    m_ui->m_stackedWidget->setCurrentWidget(m_ui->m_pageEdit);
    m_ui->m_actionStart->setChecked(false);
  }
}

void MainWindow::on_m_pBOpenFile_released()
{
  if (loadFile()) {
    m_ui->m_actionEditor->setEnabled(true);
    m_ui->m_actionEditor->setChecked(true);

    on_m_actionEditor_triggered();
  }
}

void MainWindow::on_m_pBNewFile_released()
{

}
