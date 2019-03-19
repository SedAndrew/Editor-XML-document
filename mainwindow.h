#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QWidget>
#include <QModelIndex>
#include <QFile>

namespace Ui {
  class MainUI;
}

class QGridLayout;
class QPushButton;
class WorkSpace;

class MainWindow : public QMainWindow
{
  Q_OBJECT

public:
  explicit MainWindow(QWidget *parent = nullptr);
  ~MainWindow();

  void addNewWidgetAtCenter(QModelIndex index, QWidget *widget);
  void deleteWidgetFromCenter(QModelIndex index, QWidget *widget);

  bool loadFile();

  void addNewFileName(const QString &path);
  void deleteFileName(const QString &path);

  void showStartWidget();
  void showMainWidget();
  void replaceTextFile(const QString &filename);


private slots:
  void on_m_actionStart_triggered();
  void on_m_actionEditor_triggered();
  void on_m_pBOpenFile_released();
  void on_m_pBNewFile_released();

private:
  QList<QWidget *> m_listFiles;
  WorkSpace *m_workSpace;
  QGridLayout *m_gridLayout;
  QPushButton *m_pBOpenFile;

  QMap<QString, QStringList> m_dataFile;

  Ui::MainUI *m_ui;
};

#endif // MAINWINDOW_H
