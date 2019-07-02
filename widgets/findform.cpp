#include "findform.h"
#include "ui_findform.h"

#include <QMessageBox>
#include <QDebug>
#include "element.h"

FindForm::FindForm(Element *note, QWidget *parent) :
  QDialog(parent),
  ui(new Ui::FindForm), m_note(note)
{
  ui->setupUi(this);

  qDebug() << "m_note->data(Element::Tag): " << m_note->data(1).toString();
  connect(ui->m_pBAccept, SIGNAL(clicked()), SLOT(myAccept()));
}

FindForm::~FindForm()
{
  delete ui;
}

void FindForm::myAccept()
{
  QString text = ui->m_lineEditSearch->text();
  if (text == "") {
    QMessageBox::warning(this, tr("Ошибка заполнения!"),
                               tr("Неверно введено значение поиска.\n"
                                  "Проверьте, чтобы поле было заполненно."));
    qDebug() << "Error input string search: " << getString();
  } else {
    if (getStatusPosition() == 0) {
      if (!preOrder(m_note)) {
        qDebug() << "Not Find : " << text;
        return;
      }
    } else {
      if (!inOrder(m_note)) {
        qDebug() << "Not Find : " << text;
        return;
      }
    }
    emit isNext(m_result);
  }
}

void FindForm::onSearch()
{
//  if (getStatusPosition() == 0) {
//    preOrder();
//  } else {
//    inOrder();
//  }
}

const QString FindForm::getString()
{
  return  ui->m_lineEditSearch->text();
}

bool FindForm::isName()
{
  return ui->m_checkBoxName->isChecked();
}

bool FindForm::isAttribut()
{
  return ui->m_checkBoxAttribute->isChecked();
}

bool FindForm::isValue()
{
  return ui->m_checkBoxValue->isChecked();
}

bool FindForm::isContent()
{
  return ui->m_checkBoxContent->isChecked();
}

bool FindForm::isComment()
{
  return ui->m_checkBoxComment->isChecked();
}

int FindForm::getStatusPosition()
{
  if (ui->m_radioButtonBegin->isChecked())
    return 0;
  else
    return 1;
}

bool FindForm::preOrder(Element *node)
{
  for (int i = 0; i < node->getChildrenCount(); ++i) {
    Element *item = node->getChild(i);
    if(isName() && item->data().toMap()["tag"].toString().split(" ").indexOf(getString()) != -1) {//item->data().toMap()["tag"].toString().contains(getString(), Qt::CaseInsensitive)) {
      m_result = item;
      return true;
    } else if (isAttribut()) {
      for (QString str : item->data().toMap()["attributes"].toStringList())
        if (str.contains(getString(), Qt::CaseInsensitive)) { // item->data().toMap()["attributes"].toStringList().indexOf(getString()) != -1
          m_result = item;
          return true;
        }
    } else if (isValue()) {
      for (QString str : item->data().toMap()["values"].toStringList()) {
        if (str.contains(getString(), Qt::CaseInsensitive)) {
          m_result = item;
          return true;
        }
      }
    } else if (isContent()) {
      if (item->data().toMap()["content"].toStringList().contains(getString(), Qt::CaseInsensitive)) {
        m_result = item;
        return true;
      }
    } else if (isComment()) {
      if (item->data().toMap()["comment"].toStringList().contains(getString(), Qt::CaseInsensitive)) {
        m_result = item;
        return true;
      }
    }
    if (item->isParent())
      if (preOrder(item))
        return true;
  }
  return  false;
}

bool FindForm::inOrder(Element *node)
{
  return  false;
//  function inOrder(node){
//    if (node == null) return;
//    inOrder(node.left);
//    console.log(node.value);
//    inOrder(node.right);
//  }
}
