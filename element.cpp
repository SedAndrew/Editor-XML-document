#include "element.h"

#include <QDebug>

Element::Element(const QVariant &data, Element *parent)
{
  m_parent = parent;
  setData(data);
}

Element::Element(const Element &item)
{
  m_parent = item.getParent();
  setData(item.getTag());
//  for (int i = 0; i < item.getChildrenCount(); ++i) {
//    for (int j = 0; j < item.getChild(i)->getChildrenCount(); ++j) {

//    }
//  }
}

Element::~Element()
{
  qDeleteAll(m_children);
}

void Element::appChild(Element *element)
{
  m_children.append(element);
}

bool Element::insertChild(const int row, Element *element)
{
  if (row < 0 || row > m_children.size()) return false;
  m_children.insert(row, element);
  return true;
}

bool Element::dropChild(const int row)
{
  if (row < 0 || row >= m_children.size()) return false;
  delete m_children.takeAt(row);
  return true;
}


Element *Element::getChild(int row) const
{
  return m_children.value(row);
}

Element *Element::getParent() const
{
  return m_parent;
}

const QVariant Element::getTag() const
{
  return m_tag;
}

bool Element::isParent()
{
  if (getChildrenCount() > 0) return true;
  return  false;
}

bool Element::hasContent()
{
  return !m_content.isEmpty();
}

bool Element::hasComment()
{
  return !m_comment.isEmpty();
}

QVariant Element::getValue(const int index)
{
  return m_values[index];
}

QVariant Element::getAttribut(const int index)
{
  return m_attributes[index];
}

QVariant Element::getContent()
{
  return m_content;
}

QVariant Element::getComment()
{
  return  m_comment;
}

bool Element::setData(const QVariant &data)
{
  // инициализация: тег(1), атрибуты-значение(2), контент(3), комментарии(4+5)
  if (data.isNull()) return false;
  QString string = data.toString().simplified();
  QString filling;
  QRegExp rElement("" // [\\t\\s]*
                   "<([^\\W\\d]+[\\w|\\-]*)"                         // 1
                   "\\s*([^\\W\\d]+[\\w\\-]*=\"[^>]*)*\\s?/?>"       // 2
                   "([^<]*)?(?:<!--([^>]*)-->)?(?:</\\1>\\s*)?"      // 3, 4
                   "(?:\\s*<!--([^>]*)-->)?");                       // 5
    if (rElement.indexIn(string) != -1) {
    m_tag = rElement.cap(1);
    filling = rElement.cap(2);
    m_content = rElement.cap(3);
    m_comment = rElement.cap(4) + ((rElement.cap(5).isEmpty()) ? "" : " | " + rElement.cap(5));

    if (!filling.isEmpty()) {
      QString remToken = "";
      bool goString = false; // продолжается строка value
      QStringList line = filling.split(" ", QString::SkipEmptyParts);
      for (QString word : line) {
      QRegExp rexp("(?:(.*)=\")?([^\"]*)"); // разделение на атрибут(1) и значение(2)
        rexp.indexIn(word);
        if (word.count("\"")) {
          if (word.count("\"") == 2) {
            m_attributes << rexp.cap(1);
            m_values     << rexp.cap(2);
          }
          else if (!goString) {
            m_attributes << rexp.cap(1);
            remToken += rexp.cap(2);
            goString = true;
          }
          else {
            remToken += " " + rexp.cap(2);
            m_values     << remToken;
            remToken = "";
            goString = false;
          }
        }
        else if (goString) {
          remToken += " " + word;
        }
      }
    }
    else {
      m_attributes << ""; m_values << "";
    }
  } else if (!string.contains(QRegExp("</.*>"))){
      qDebug () << "Error validation: " << string;
  }

  m_data = m_tag;
  m_xml = string;

  return true;
}

QVariant Element::data() const
{
  QVariantMap varMap;
  varMap["tag"] = QVariant(m_tag);
  varMap["attributes"] = QVariant(m_attributes);
  varMap["values"] = QVariant(m_values);
  varMap["content"] = QVariant(m_content);
  varMap["comment"] = QVariant(m_comment);
  return  varMap;
}

QVariant Element::data(const int num) const
{
  int numValues = m_attributes.count() + 1;
  if (num == 1)
    return QVariant(m_tag);
  else if (num > numValues) {
    if (num == numValues + 1)
      return QVariant(m_content);
    else if (num == numValues + 2)
      return  QVariant(m_comment);
  }
  QVariantList qvl;
  qvl.push_back(QVariant(m_attributes));
  qvl.push_back(QVariant(m_values));
  return QVariant(qvl);
}

QVariant Element::dataItem() const
{
  return QVariant(m_data);
}

int Element::getRow() const
{
  if (m_parent)
    return m_parent->m_children.indexOf(const_cast<Element*>(this));
  return 0;
}

int Element::getChildrenCount() const
{
  return m_children.count();
}

const QVariant Element::getXml() const
{
  return m_xml;
}

const QList<QString> &Element::getAttributes() const
{
  return m_attributes;
}
