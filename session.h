#ifndef SESSION_H
#define SESSION_H

#include <QVector>

struct Document;
struct Session {
  Session();
  Session(Document *doc);
  Session(QVector<Document *> docs);

  ~Session();

  /**
   * @brief
   * Загружаются новый список документов.
   */
  void loadDocs(const QVector<Document *> ds);
  /**
   * @brief
   * Возвращается массив загруженных документов.
   */
  const QVector<Document *> &documents() const;
  /**
   * @brief
   * Возвращается последний документ
   */
  Document *&last();
  /**
   * @brief
   * Возвращается список документов.
   */
  const QStringList toStringList() const;
  /**
   * @brief
   * если документ есть в сессии, то возвращаяется - true, иначе - false.
   */
  bool existDoc(const QString path);
  /**
   * @brief
   * В случае успеха возвращает документ по указанному пути, иначе последний в списке.
   * Перед применением рекомендуется проверить документ на существоавние функцией existDoc()
   */
  Document *getDoc(const QString path);
  /**
   * @brief
   * Добавление нового документа в сессию.
   */
  void add(Document *doc);
  /**
   * @brief
   * Удаление документа из сессии по индексу.
   */
  void remove(const int i);
  /**
   * @brief
   * Удаление указанного документа из сессии.
   * При вызове без параметра удаляется последний документ.
   */
  void remove(const Document *doc = nullptr);
  /**
   * @brief
   * Удаление всех документов из сессии.
   */
  void removeAll();

  void setFile(const QString &filename);
  const QString &getCurrentFile() const;
private:
  QVector<Document *> docs;
  QString m_lastViewFile;
};

//namespace Sad {
//  struct Ses : Session {};
//}

#endif // SESSION_H
