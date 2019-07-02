#include "addelementform.h"
#include "ui_addelementform.h"
#include <xmlbuilder.h>

#include <QTableWidget>
#include <QDebug>


AddElementForm::AddElementForm(QWidget *parent) :
  QDialog (parent),
  ui(new Ui::AddElementForm), m_currnetIndex(nullptr)
{
  ui->setupUi(this);
//  setWindowIcon(parent->windowIcon());
  ui->label_2->setHidden(true);
      ui->m_cBNameParent->setHidden(true);
  QHeaderView *header;
  header = new QHeaderView(Qt::Horizontal);
  header->setStretchLastSection(true);
  ui->m_tableWidget->setHorizontalHeader(header);
  ui->m_tableWidget->setHorizontalHeaderLabels(QStringList{tr("Атрибут"), tr("Значение")});

  connect(ui->m_pBAccept, SIGNAL(clicked()), SIGNAL(myAccept()));
  connect(ui->m_pBCancel, SIGNAL(clicked()), SLOT(reject()));
}

AddElementForm::~AddElementForm()
{
  delete ui;
}

//  QString string = "<" + getName() + " " + getAttributes().toStringList().join(" ");
//  if (!getContent().isEmpty()) string += ">" + getContent() + "</" + getName() + ">";
//  else string += "/>";
//  if (!getComment().isEmpty()) string += "<!-- " + getComment() + "-->";
const QVariant AddElementForm::getXml()
{ 
  XMLBuilder builder;
  builder.begin(getName());
  for (int row = 0; row < ui->m_tableWidget->rowCount(); ++row) {
    builder.attr(ui->m_tableWidget->item(row, 0)->text(),
                 ui->m_tableWidget->item(row, 1)->text());

  }
  if (!getContent().isEmpty()) builder.content(getContent());
  if (!getComment().isEmpty()) builder.comment(getComment());

  builder.end();
  return QVariant(builder.getXML());
}

const QString AddElementForm::getName()
{
  return ui->m_lineEditNameTag->text();
}

const QVariant AddElementForm::getAttributes()
{
  QStringList attributes;
//  for (int row = 0; row < ui->m_tableWidget->rowCount(); ++row) {
//    QString str = ui->m_tableWidget->takeItem(row, 0)->text() + "=\""
//               +  ui->m_tableWidget->takeItem(row, 1)->text() + "\"";
//    attributes << str;



//  }
  return QVariant(attributes);
}

const QString AddElementForm::getContent()
{
  return ui->m_lineEditContent->text();
}

const QString AddElementForm::getComment()
{
  return  ui->m_lineEditComment->text();
}

int AddElementForm::getStatusPosition()
{
  if (ui->m_radioButtonFront->isChecked())
    return 0;
  else
    return 1;
}

void AddElementForm::on_m_pBAdd_clicked()
{
  ui->m_tableWidget->insertRow(ui->m_tableWidget->rowCount());
}

void AddElementForm::on_m_pBDelete_clicked()
{
  int row = ui->m_tableWidget->currentRow();
  ui->m_tableWidget->removeRow(row);
}

void AddElementForm::on_m_tableWidget_itemChanged(QTableWidgetItem *item)
{
  if (!m_currnetIndex->isValid()) return;
  if (m_currnetIndex->column() == 0) {
    QRegExp regx("");
  }
  if (m_currnetIndex->column() == 1) {

  }
    qDebug() << "item->text(): " << item->text();
}

void AddElementForm::on_m_tableWidget_clicked(const QModelIndex &index)
{
  if (m_currnetIndex != nullptr)
    delete m_currnetIndex;
  m_currnetIndex = new QModelIndex(index);
//  m_currnetIndex = index;
}
