#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#define ORGANIZATION_NAME "SedoikinAndrew"
#define ORGANIZATION_DOMAIN "https://github.com/SedAndrew"
#define APPLICATION_NAME "Editor-XML-document"


#include <QMainWindow>
#include <QWidget>
#include <QModelIndex>
#include <QFile>

/**
 *  Разновидность сигналов и слотов:
 *  1.  один сигнал -> несколько слотов
 *  2.  один сигнал -> несколько сигналов
 *  3.  один слот -> несколько сигналов
 *
 *
 * dumpObjectInfo() - показывает информацию об объекте:
 *  - имя объекта,
 *  - класс, от которого был создан объект,
 *  - сигнально-слотовые соединения
 *
 */

namespace Ui {
  class MainUI;
}

class QHBoxLayout;
class QSplitter;
class QPushButton;
class LableClicked;

class WorkSpace;
class Sidebar;
class AddElementForm;
class FindForm;
class QCompleter; //TreeModelCompleter;
class XMLBuilder;

struct Session;
struct Document;

static QStringList m_recentFiles;       // ранее открытые документы
static QStringList m_lastFiles;         // последние открытые документы
static QString m_lastSession;           // последняя сессия


class Element;

class MainWindow : public QMainWindow
{
  Q_OBJECT

public:
  explicit MainWindow(QWidget *parent = nullptr);
  ~MainWindow() override;

  void addNewWidgetAtCenter(QModelIndex index, QWidget *widget);
  void deleteWidgetFromCenter(QModelIndex index, QWidget *widget);

  void readWindowSettings();      // Считать настройки программы
  void writeWindowSettings();     // Записать настройки программы перед выходом

  bool save(Element *node, XMLBuilder &doc);

private:
  void cfgEditPage();             // Настройка страницы Edit
  /**
   * @brief // Загрузка списка действий в меню Recent \\
   * Инициализация recent actions перед загрузкой программы
   */
  void initRecentDocActions();
  /**
   * @brief
   * Обновление списка некогда открытых документов
   */
  void updateResentList(const QString &path);

  // Добавление документа в указанную сессию
  void createData(const QString path);
  /**
   * @brief
   * Загрузка сессии.
   */
  void loadSession(const QString &name);
  /**
   * MainWindow::readSessionFile
   * @brief
   *    Читает файл /sessions.txt и записывает в контейнер Map список документов по имени сессии.
   * @param mapDocs
   */
  void readSessionFile(QMap<QString, QStringList> &mapDocs);
  /**
   * @brief
   *    Записывает в файл /sessions.txt содержимое контейнера m_mapSessions
   * @param mapDocs
   */
  void writeSessionFile(QMap<QString, QStringList> &mapDocs);

  /**
   * @brief
   * Имеет значение по умолчанию
   * nullptr, возвращает значение True - в случае успешной загрузки;
   * False - в случае ошибки
   * @param path
   * @return
   *
   * Функция одного параметра строкогого константного типа, который имеет значение по умолчанию
   * nullptr, и которая возвращает булевское значение (True - в случае успешной загрузки;
   * False - в случае ошибки)
   *
   * Если функция вызывается без параметров, открывается окно системного проводника для выбора XML
   * документа.
   * Иначе происходит загрузка документа, путь которого был передан в параметре.
   *
   * Новый документ записывается в глобальную переменную типа QSetting (кол-во документов <= 20),
   * в массив документов текущей сессии, а также добавляется новая запись в MenuBar во вкладку ранее открытых
   * документов.
   *
   * Связанные слоты:
   * m_actionOpen_triggered(QString)  - перехватывает сигналы от
   *    on_m_actionOpen_triggered     - меню
   *    on_m_pBOpenFile_pressed       - кнопки
   *    m_openRecentFile_triggered    - меню recent
   *
   */
  bool loadDoc(const QString &path = nullptr); // Загрузить документ

  void addNewFileName(const QString &path);     // ? Создание нового документа

  void showStartWidget();               // Стартовая страница
  void showMainWidget();                // Главная страница редактора
  void replaceTextFile(const QString &filename);  // !!! перезаписать текстовый документ

  bool eventFilter(QObject *watched, QEvent *event) override; // текущий активный виджет WorkSpace



protected:
  void mouseMoveEvent(QMouseEvent *event) override; // ?

signals:
  void changedRecentFile(QString);        // Сигнал, который предупреждает выбор документа из истории
  void changedListLastFiles(QString);    // Сигнал выбора ??
//  void openNewDoc(QString);               // Сигнал, который предупреждает о новом документе
//  void openNewDoc(const Document*, WorkSpace*);            // Сигнал, который посылает указатель на новый документ
  /**
   * @brief
   * Предупреждает об изменениях документов в сессии
   */
//  void changNumDocs(Document*, WorkSpace*);            // Сигнал, который посылает указатель на новый документ
  /**
   * @brief
   * Посылает указатель на новый документ
   */
  void changNumDocs(Document*);            // Сигнал, который посылает указатель на новый документ
//  void instalFile(const int &index);    // Установить файл в текщий виджет
  /**
   * @brief
   * Сигнал, передает имя документа, в котором сейчас работает пользователь
   */
  void activetedDoc(QString);             // Сигнал, передает документ в котором работает пользователь
//  void changedFocus(uint);              // Сигнал, который посылат индекс текщего виджета
  /**
   * @brief
   * сигнал, предупреждающий установку нового документа в focus-виджет
   */
  void changeFile(Document*);



//  void callbackSidebar(Document *doc);

public slots:
  //===========================Навигация===========================
  void on_m_actionStart_toggled(bool arg1);     // Событие action Start page переключения check по arg1
  void on_m_actionEditor_toggled(bool arg1);    // Событие action Edit  page переключения check по arg1
  //===============================================================

  void on_m_actionExit_triggered();     //  событие action Exit from program

  //===========================Загрузка документа===========================
  void Open_triggered(const QString &filePath = "");  //  открыть новый документ
  void on_m_actionOpen_triggered();     // Меню -> открыть док.
  void on_m_pBOpenFile_pressed();       // Кнопка -> открыть док.
  void m_openFile_triggered();    // Меню -> ранее откр. док. -> открыть док.

  void closeFile(const int &index);     // Закрытие документа
  //=========================================================================

  //===========================Создание документа===========================
  void on_m_actionNew_triggered();              //  создание нового документа
  void on_m_pBNewFile_pressed();                //  Кнопка создания документа
  //========================================================================

  //===========================Дополнительные функции===========================
  void on_m_actionClearPlaceRecent_triggered(); //  очистить список ранее открываемых документов
//  void on_m_actionRecent_triggered();         //
  void on_m_actionAboutEditorXMLDocuments_triggered(); //  открыть информацию о программе
  void on_m_actionAboutQtCreator_triggered();   //  открыть информацию по среде Qt Creator
  void closeEvent(QCloseEvent *event) override;
  void openFindForm();
  void inputLineSearch();
  void initCompleter(QCompleter *completer); // TreeModelCompleter
  void foundItem(Element *item);


  bool exitApp();
  /**
   * @brief
   * Загрузить последнюю рабочую сессию
   */
  void on_m_pBLastSession_pressed();            // ?! Вызов предыдущей cеcсии

  //===========================Настройки окон===========================
  void on_m_pBIconPanel_released();             // Открыть боковую панель
  //====================================================================

  //===========================Настройки WorkSpace===========================
  /**
   * @brief
   * Получение сообщения о выборе документа в Sidebar'e и пересылка его в currentWorkSpace
   */
  void callbackSidebar(Document *doc);
  void m_dividWorkSpace(const Qt::Orientation orient);  // деление рабочего пространства
//  void instalFile(const QString &name);                 // Установить файл в текщий виджет
//  void highlight(const QModelIndex &index);

  //=========================================================================

public:
  WorkSpace *m_currentPlace;              // текущая рабочая область
  uint m_indexCurrentWS;                  // индекс текущей рабочей области


private slots:
  /**
   * @brief
   * Открывает окно управления сессиями.
   */
  void on_m_actionControlSessions_triggered();

  void actionSaveFile_triggered();
  void actionSaveFileAs_triggered();

private:
//  QList<QWidget *> m_listFiles;           //
//  QGridLayout *m_gridLayout;
  //===============================Документ===================================
  QMap<QString, QStringList> m_mapSessions;     // сессии
  QString m_nameSession;                        // имя текущей сессии
  QVector<Document*> m_dataFile;          // документы текущей сессии
  Session *m_session;                      // документы текущей сессии <--
  QVector<QAction *> m_recentFileActions; // массив документов в меню Recent

  //====================================Редактирование===================================
  FindForm *m_findForm;
  //====================================Окно===================================
  QHBoxLayout *m_layoutEditPage;          // компановка страницы с редактором
  QSplitter *m_splitter;                  // корневой сплиттер страницы с редактором
  QPushButton *m_pBOpenFile;              // кнопка для открытия документа

  Ui::MainUI *m_ui;

//  QMap<QString, QStringList> m_files;      //
};



#endif // MAINWINDOW_H
