#ifndef TABLEMODEL_H
#define TABLEMODEL_H

#include <QAbstractTableModel>
#include <QModelIndex>

enum class Type : unsigned {
  Tag = 1, Attributes, Content, Comment
};

struct Pair {

  Pair(const QString &a, const Type t) {
    typeElem = t;
    if (typeElem == Type::Tag){
      att = a;
      val = "";
    } else {
      att = typeElem == Type::Content ? "Контент" : "Комментарий";
      val = a;
    }
  }
  Pair(const QString &a, const QString &v)
    : att(a), val(v) {
    typeElem = Type::Attributes;
  }

  QVariant key() {
    return att;
  }
  QVariant value() {
    return val;
  }
  QVariant type() {
    switch (typeElem) {
      case Type::Tag:
        return QVariant("Тег");
      case Type::Attributes:
        return QVariant("Атрибут");
      case Type::Content:
        return QVariant("Контент");
      case Type::Comment:
        return QVariant("Комментарий");
    }
  }
  bool setKey(const QString newKye) {
    if (typeElem == Type::Content || typeElem == Type::Comment)
      return false;
    att = newKye;
    return true;
  }
  bool setVal(const QString newVal) {
    if (typeElem == Type::Tag)
      return false;
    val = newVal;
    return true;
  }

private:
  QString att;
  QString val;
  Type typeElem;
};

class Element;
class TableModel : public QAbstractTableModel
{
public:
  TableModel(Element &data, QObject *parent = nullptr);
  ~TableModel();

  int rowCount(const QModelIndex &) const;
  int columnCount(const QModelIndex &) const;
  QVariant data(const QModelIndex &index, int role) const;
  QVariant headerData(int section, Qt::Orientation orientation, int role) const;
  Qt::ItemFlags flags(const QModelIndex &index) const;
  bool setData(const QModelIndex &index, const QVariant &value, int role);

  const QVariant getData(const Type type, const int index ) const;
  const QVariant getData(const int row, const int column ) const;

private:
  void convertToList(const QVariant &data);

private:
  void setupModelData(const QStringList &lines, QVariant *data);
  /**
   * @brief
   * Содержит в себе QVariantList c парами QVariantMap: Тег:Значение, Атрибут:Значение, Контент:Значение,
   * Комментарий:Значение.
   */
  QList<Pair> m_data;
  Element *m_element;

};

#endif // TABLEMODEL_H
