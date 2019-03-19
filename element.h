#ifndef ELEMENT_H
#define ELEMENT_H

#include <QList>
#include <QString>

class Element
{
public:
  Element(const QVariant &data, Element *parent = nullptr);
  ~Element();

  void appChild(Element *element);  // добавление потомока
  bool insertChild(const int row, Element *element);  // вставка потомка
  bool dropChild(const int row); // удаление потомка

  Element *getChild(int row) const; // потомко по номеру строки
  Element *getParent() const; // текущий родитель

  bool setData(const QVariant &data); // задает значения веток после изменения (flags Qt::ItemIsEditable)
  QVariant data() const;  // данные элемента (тег, атрибут, значение, контент, коммент-й)
  QVariant data(const int num) const;
  int getRow() const;
  int getChildrenCount() const;
  const QList<QString> &getAttributes() const;

private:
  QVector<Element* > m_children;
  QList<QString > m_attributes;
  QList<QString > m_values;
  QString m_tag;
  QString m_content;
  QString m_comment;
  Element *m_parent;
};

#endif // ELEMENT_H
