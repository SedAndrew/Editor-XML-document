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
class LableClicked;

class MainWindow : public QMainWindow
{
  Q_OBJECT

public:
  explicit MainWindow(QWidget *parent = nullptr);
  ~MainWindow();

  void addNewWidgetAtCenter(QModelIndex index, QWidget *widget);
  void deleteWidgetFromCenter(QModelIndex index, QWidget *widget);

  bool loadFile();        // загрузка документа

  void addNewFileName(const QString &path);   // Добавление нового файла
  void deleteFileName(const QString &path);   // Удаление файла

  void showStartWidget();               // Стартовая страница
  void showMainWidget();                // Главная страница редактора
  void replaceTextFile(const QString &filename);

protected:
  void mouseMoveEvent(QMouseEvent *event);


private slots:
  void on_m_actionStart_toggled(bool arg1);     // Событие action Start page переключения check по arg1
  void on_m_actionEditor_toggled(bool arg1);    // Событие action Edit  page переключения check по arg1
  void on_m_actionExit_triggered();             // Событие action Exit from program
  void on_m_pBOpenFile_pressed();               // Вызов функции загрузки документа
  void on_m_NewFile_pressed();                  // Вызов функции создания документа
  void on_m_LastSetion_pressed();               // Вызов предыдущей версии
  void on_m_pBIconPanel_released();             // Открытие боковой панели
  void on_m_actionOpen_triggered();             //
  void on_m_actionNew_triggered();              //
  void on_m_actionSessions_triggered();         //
  void on_m_actionRecent_triggered();           //
  void on_m_actionAboutQtCreator_triggered();   //

private:
  QList<QWidget *> m_listFiles;
  WorkSpace *m_workSpace;
  QGridLayout *m_gridLayout;
  QPushButton *m_pBOpenFile;

  QMap<QString, QStringList> m_dataFile;

  Ui::MainUI *m_ui;


};



#endif // MAINWINDOW_H
