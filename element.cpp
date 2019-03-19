#include "element.h"

#include <QDebug>

Element::Element(const QVariant &data, Element *parent)
{
  m_parent = parent;
  setData(data);
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

bool Element::setData(const QVariant &data)
{
  // инициализация: тег, атрибут, значение, контент, коммент-й
  if (data.isNull()) return false;
  QString string = data.toString();
  QRegExp rElement("[\\t\\s]*"
                   "<([^\\W\\d]+[\\w\\d]*)"        // 1
                   "\\s*([^\\W\\d]+[\\w\\d]*=.*)?" // 2
                   "(?:/>|>(.*)</\\1>|>\\s*)"      // 3
                   "(\\s*<!--(.*)-->)?");          // 4
  if (!rElement.indexIn(string)) {
    m_tag = rElement.cap(1);
    m_content = rElement.cap(3);
    m_comment = rElement.cap(4);
    string = rElement.cap(2);
//    if (!rAtrVal.indexIn(string)) {
    if (!string.isEmpty()) {
      QString remToken = "";
      bool goString = false;

      foreach (QString word, string.split(" ", QString::SkipEmptyParts)) {
        QRegExp rexp("(?:(.*)=\")?([^\"]*)");
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
            remToken = QString("%1 %2").arg(remToken).arg(rexp.cap(2));
            m_values     << remToken;
            remToken = "";
            goString = false;
          }
        }
        else if (goString) {
          remToken = QString("%1 %2").arg(remToken).arg(word);
        }
        else {}
      }
    }
  }


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

const QList<QString> &Element::getAttributes() const
{
  return m_attributes;
}
