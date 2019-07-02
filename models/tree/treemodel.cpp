#include "treemodel.h"
#include "element.h"
#include <typeinfo>
#include <QDebug>
#include <QStack>
#include <QFont>

TreeModel::TreeModel(QObject *parent)
  : QAbstractItemModel(parent)
{
  QVariantList rootElement;
  rootElement << "<Root>";
  QString root = "<Root>";
  rootItem = new Element(root);
}

TreeModel::TreeModel(const QStringList &strings, QObject *parent)
    : QAbstractItemModel(parent)
{
  QString root = "<Root>";
  rootItem = new Element(root);
  setupModelData(strings, rootItem);
}

TreeModel::~TreeModel()
{
    delete rootItem;
}

int TreeModel::rowCount(const QModelIndex &parent) const
{
    Element *parentItem;
    if (!parent.isValid())
        parentItem = rootItem;
    else
        parentItem = static_cast<Element*>(parent.internalPointer());
    return parentItem->getChildrenCount();
}

int TreeModel::columnCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 1; //static_cast<Element*>(parent.internalPointer())->getAttributes().count() + 1;
    else
        return 1;//rootItem->getAttributes().count();
}

QVariant TreeModel::data(const QModelIndex &index, int role) const
{   // вызывается при каждом взаимодействии с ветками (наведение, снятие, перемещение)
    // возвращает элемент данных, соответствующего аргументу-индексу
    if (!index.isValid())
        return QVariant();
    Element *item = static_cast<Element*>(index.internalPointer());
    switch(role){
      case Qt::DisplayRole : {
          // номер столбца <==> содержимое тега (1 - имя, 2..n-2 - атрибуты, n-1 - контент, n - коммент)
          QVariant out = item->dataItem(); //index.column() // Element::Tag
          return out;
        }
      case Qt::ToolTipRole : {
          return QVariant(item->getXml());
        }
      case Qt::FontRole:{
          if (item->isParent())
            return  QFont("Calibri", 12, QFont::Bold);
          return QFont("Calibri", 12);
        }
      default:
        return QVariant();
    }
}

bool TreeModel::hasChildren(const QModelIndex &parent) const
{
  if (!parent.isValid())
    return QAbstractItemModel::hasChildren(parent);
  Element *item = static_cast<Element *>(parent.internalPointer());
  Q_ASSERT(item != nullptr);
  if (item->isParent())
    return true;
  else return false;
}

Qt::ItemFlags TreeModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return Qt::ItemIsEnabled;
    return Qt::ItemIsEnabled | Qt::ItemIsSelectable; // | Qt::ItemIsEditable;
}

QVariant TreeModel::headerData(int section, Qt::Orientation orientation, int role) const
{
  Q_UNUSED(section)
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole)
        return rootItem->data(section);
    return QVariant();
}

QModelIndex TreeModel::index(int row, int column, const QModelIndex &parent) const
{
    Element *parentItem;
    if (!parent.isValid())
        parentItem = rootItem;
    else
        parentItem = static_cast<Element*>(parent.internalPointer());
    Element *childItem = parentItem->getChild(row);
    if (childItem)
        return createIndex(row, column, childItem);
    else
        return QModelIndex();
}

bool TreeModel::insertRows(int position, int rows, const QModelIndex &parent)
{
    Element *parentItem;
    if (!parent.isValid())
        parentItem = rootItem;
    else
        parentItem = static_cast<Element*>(parent.internalPointer());
    if (position < 0 || position > parentItem->getChildrenCount())
        return false;
    QList<QVariant> blankList;
    blankList << QVariant("");
    QVariant str("");
    for (int column = 0; column < columnCount(); column++)
    beginInsertRows(parent, position, position + rows - 1);
    for (int row = 0; row < rows; ++row) {
        Element *newItem = new Element(str, parentItem);
        if (!parentItem->insertChild(position, newItem))
            break;
    }
    endInsertRows();
    return true;
}

bool TreeModel::removeRows(int position, int rows, const QModelIndex &parent)
{
    Element *parentItem;
    if (!parent.isValid())
        parentItem = rootItem;
    else
        parentItem = static_cast<Element*>(parent.internalPointer());
    if (position < 0 || position > parentItem->getChildrenCount())
        return false;
    beginRemoveRows(parent, position, position + rows - 1);
    for (int row = 0; row < rows; ++row) {
        if (!parentItem->dropChild(position))
            break;
    }
    endRemoveRows();
    return true;
}

QModelIndex TreeModel::parent(const QModelIndex &index) const
{
    if (!index.isValid())
        return QModelIndex();
    Element *childItem = static_cast<Element*>(index.internalPointer());
    Element *parentItem = childItem->getParent();
    if (parentItem == rootItem)
        return QModelIndex();
    return createIndex(parentItem->getRow(), 0, parentItem);
}

bool TreeModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (!index.isValid() || role != Qt::EditRole)
        return false;
    Element *item = static_cast<Element*>(index.internalPointer());
    if (item->setData(value)) // column
        emit dataChanged(index, index);
    else
        return false;
    return true;
}

Element *TreeModel::getRootNote()
{
  return rootItem;
}

void TreeModel::setModelData(const QStringList &lines)
{
//  delete rootItem;
//  QVariantList rootElement;
//  rootElement << "Root";
//  rootItem = new Element(rootElement);
//  Element *root(new Element(rootElement));
  for (int i = 0; i < rootItem->getChildrenCount(); ++i) {
    rootItem->dropChild(i);
  }
  setupModelData(lines, rootItem);
//  rootItem = root;
}

/**
 * @brief
 * Создание модели дерева
 */
void TreeModel::setupModelData(const QStringList &lines, Element *parent)
{
  QList<Element*> parents;
  parents << parent;

//     * С помощью следующих регулярных выражений будет происходить структурирование модели дерева
//     *   regChild, отбирает потомков (тег с простым содержимым)
//     *   regParent, отбирает родителей (тег со сложным содержимым)
//     *   rEndParnt, определяет конец тега родителя
//     * Note: без проверки на валибность нескольких тегов в одной строке

  QRegExp regChild("<[^/\\d].*(?:/>|>.*</\\1>)"); //.*<[^/\\d].*(?:/>|>.*</\\1>)
  QRegExp regParent("<[^/\\d].*>\\s*\\n"); // .*<[^/\\d].*>\\s*\\n
  QRegExp rEndParent("</(.*)>");//[\\t\\s]*</(.*)>

  // при построении дерева строка настройки не учитывается, поэтому цикл начинается со второй строки
  for (int number = 1; number < lines.count(); ++number) {
    QString string = lines[number];
    if (string.startsWith("<!")) continue;
    int type = 0;
    if (regChild.indexIn(string) != -1) type = 0;
    else if (regParent.indexIn(string) != -1) type++;
    else if (rEndParent.indexIn(string) != -1) type--;
    else continue;

    Element *element(new Element(QVariant(string), parents.last()));
    switch (type)
    {
      case 1 : // begin parent tag
      {
        parents.last()->appChild(element);
        parents << element;
      }
      break;
      case 0 : // child
      {
        parents.last()->appChild(element);
      }
      break;
      case -1 : // end parent tag
      {
        parents.pop_back();
      }
      break;
    }
  }
}
