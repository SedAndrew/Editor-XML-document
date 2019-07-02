#ifndef ELEMENT_H
#define ELEMENT_H

#include <QList>
#include <QString>
#include <QModelIndex>


class Element
{
public:
  Element(const QVariant &data, Element *parent = nullptr);
  Element(const Element &);
  ~Element();

//  enum Type {
//    Tag = 1, Attributes, Values, Content, Comment
//  };

  void appChild(Element *element);  // добавление потомока
  bool insertChild(const int row, Element *element);  // вставка потомка
  bool dropChild(const int row); // удаление потомка

  bool setData(const QVariant &data); // задает значения веток после изменения (flags Qt::ItemIsEditable)
  QVariant data() const;  // данные элемента (тег, атрибут, значение, контент, коммент-й)
  /**
   * @brief
   * num - это порядковый номер атрибута.
   * (1 - имя, 2..n-2 - атрибуты, n-1 - контент, n - коммент)
   */
  QVariant data(const int num) const;
  QVariant dataItem() const;

  /**
   * @brief
   * потомок по номеру строки: 0..n
   */
  Element *getChild(int row) const; //
  Element *getParent() const; // текущий родитель
  int getRow() const;
  int getChildrenCount() const;

  bool isParent();
  bool hasContent();
  bool hasComment();
  QVariant getValue(const int index);
  QVariant getAttribut(const int index);
  QVariant getContent();
  QVariant getComment();

  const QVariant getXml() const;
  const QVariant getTag() const;
  const QList<QString> &getAttributes() const;

public:

    Element &operator = (Element&);

private:
  Element *m_parent;
  QVector<Element* > m_children;

  QVariant m_xml;
  QString m_tag;
  QString m_content;
  QList<QString > m_attributes;
  QList<QString > m_values;
  QString m_comment;
  QString m_data;

};

#endif // ELEMENT_H
