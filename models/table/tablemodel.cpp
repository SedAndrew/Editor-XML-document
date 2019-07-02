#include "tablemodel.h"
#include "element.h"

#include <QDebug>

TableModel::TableModel(Element &element, QObject *parent)
  : QAbstractTableModel(parent)
{
  convertToList(element.data());
  m_element = &element;
}
TableModel::~TableModel()
{

}

int TableModel::rowCount(const QModelIndex &) const
{
  int count = 0; // 4
  count = m_data.count();

  /*
//  QMapIterator<QString, QVariant> i(m_data.toMap());
//  while (i.hasNext()) { i.next();
//    QString header = i.key();
//    if (header == "tag") {
//      count++;
//    }
//    else if (header == "attributes") { //  || header == "values"
//      if (!i.value().toStringList().contains(""))
//        count += 1; //i.value().toStringList().count();
//    }
////    else if (header == "values") {
////      count += 1; //i.value().toStringList().count();
////    }
//    else if (header == "content") {
//      if (!i.value().toStringList().contains(""))
//        count++;
//    }
//    else if (header == "comment") {
//      if (!i.value().toStringList().contains(""))
//        count++;
//    }
//  }
  */
  return count;
}

int TableModel::columnCount(const QModelIndex &) const
{
  return 3;
}

QVariant TableModel::data(const QModelIndex &index, int role) const
{
  if (index.isValid() && role == (Qt::DisplayRole)) { //Qt::DisplayRole
    return getData(index.row(), index.column());
  }
  return QVariant();
}

QVariant TableModel::headerData(int section, Qt::Orientation orientation, int role) const
{
  if (role != Qt::DisplayRole)
      return QVariant();
  if (orientation == Qt::Horizontal) {
    switch (section) {
      case 0:
        return QVariant(tr("Тип"));
      case 1:
        return QVariant("Атрибут");
      case 2:
        return QVariant("Значение");
      default:
        return QVariant(section);
    }
  }

  return QVariant(section);
}


Qt::ItemFlags TableModel::flags(const QModelIndex &index) const
{
  if (index.column() == 1 && index.row() < m_data.count() - 2)
    return Qt::ItemIsSelectable | Qt::ItemIsEnabled | Qt::ItemIsEditable;
  if (index.column() == 2 && index.row() > 0)
    return Qt::ItemIsSelectable | Qt::ItemIsEnabled | Qt::ItemIsEditable;
  return Qt::ItemIsSelectable | Qt::ItemIsEnabled;

}

const QVariant TableModel::getData(const int row, const int column) const
{
  QVariant data;
  Pair p = m_data.at(row);
  switch (column) {
    case 0:
      return  p.type();
    case 1:
      return  p.key();
    case 2:
      return  p.value();
    default:
      return  p.type();
  }
}

void TableModel::convertToList(const QVariant &data)
{
//  QVariantList pairAttrVal;
  QMapIterator<QString, QVariant> i(data.toMap());
  QString tag;
  QStringList attributes;
  QStringList values;
  QString content;
  QString comment;

  while (i.hasNext()) { i.next();
    if ( i.key() == "tag") tag = i.value().toString();
    if ( i.key() == "attributes") attributes = i.value().toStringList();
    if ( i.key() == "values") values= i.value().toStringList();
    if ( i.key() == "content") content = i.value().toString();
    if ( i.key() == "comment") comment = i.value().toString();
  }

  Pair pairTag(tag, Type::Tag);
  m_data.append(pairTag);

  for (int i = 0; i < attributes.count(); ++i) {
    Pair pair(attributes[i], values[i]);
    m_data.append(pair);
  }

  Pair pairContent(content, Type::Content);
  m_data.append(pairContent);

  Pair pairComment(comment, Type::Comment);
  m_data.append(pairComment);
}

const QVariant TableModel::getData(const Type type, const int index) const
{






//  int state = int(type);
//  switch (state) {
//    case Type::Tag:
//        return m_data.toMap()["tag"];
//    case Type::Attributes:
//        return  QVariant(m_data.toMap()["attributes"].toStringList().at(index));
//    case Type::Values:
//        return  QVariant(m_data.toMap()["values"].toStringList().at(index));
//    case Type::Content:
//        return m_data.toMap()["content"];
//    case Type::Comment:
//        return m_data.toMap()["comment"];
//    default :
//        return QVariant();
  //  }
}



void TableModel::setupModelData(const QStringList &lines, QVariant *data)
{
//  int countColumn = 2;

}




bool TableModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
  qDebug () << "data: " << value;
  if (index.column() == 1)
    m_data[index.row()].setKey(value.toString());
  else if (index.column() == 2)
    m_data[index.row()].setVal(value.toString());
  else {
    return false;
  }
  dataChanged(index, index);
  return true;
}
