#ifndef DOCUMENT_H
#define DOCUMENT_H

#include <QString>
#include <QStringList>

class Element;
struct Document
{
  Document(const Document *doc);
  Document(const QString &path);
  Document(const QString &path, const QString &name, const QStringList text);

  Document *operator = (const Document *);

  QString path;
  QString name;
  QStringList text;

  Element *refElement;
};

//namespace Sad {
//  struct Doc : Document {};
//}

#endif // DOCUMENT_H
