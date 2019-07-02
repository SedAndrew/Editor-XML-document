#include "session.h"
#include "document.h"

Session::Session() {}

Session::Session(Document *doc) {
  docs.push_back(doc);
}

Session::Session(QVector<Document *> docs) : docs(docs)
{

}

Session::~Session()
{
  while (!docs.isEmpty()) {
    delete docs.last();
  }
}

void Session::loadDocs(const QVector<Document *> ds) {
  if (!ds.isEmpty())
    docs = ds;
}

const QVector<Document *> &Session::documents() const {
  return docs;
}

Document *&Session::last() {
  return docs.last();
}

const QStringList Session::toStringList() const {
  QStringList paths;
  foreach (Document *doc, documents())
    paths << doc->path;
  return paths;
}

bool Session::existDoc(const QString path) {
  foreach (Document *doc, docs)
    if (doc->path == path)
      return true;
  return false;
}

Document *Session::getDoc(const QString path)
{
  Document *d = docs.last();
  foreach (Document *doc, docs) {
    if (doc->path == path) {
      d = doc;
      break;
    }
  }
  return d;
}

void Session::add(Document *doc) {
  docs.push_back(doc);
  m_lastViewFile = doc->name;
}

void Session::remove(const int i) {
  docs.remove(i);
}

void Session::remove(const Document *doc) {
  int i = 0;
  if (doc == nullptr) docs.pop_back();
  else
    foreach (Document *d, docs) {
      if (d == doc) {
        delete d;
        docs.remove(i);
        break;
      }
      i++;
    }
}

void Session::removeAll() {
  foreach (Document *d, docs) {
    delete d;
    docs.pop_front();
  }
}

void Session::setFile(const QString &filename)
{
  m_lastViewFile = filename;
}

const QString &Session::getCurrentFile() const
{
  return m_lastViewFile;
}
