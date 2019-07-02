#include "document.h"

Document::Document(const Document *doc)
  : path(doc->path), name(doc->name), text(doc->text), refElement(nullptr)
{

}

Document::Document(const QString &path)
  : path(path), refElement(nullptr)
{
  name = "New document";
  text = QStringList("");
}

Document::Document(const QString &path, const QString &name, const QStringList text)
  : path(path), name(name), text(text), refElement(nullptr)
{

}
