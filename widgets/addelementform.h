#ifndef ADDELEMENTFORM_H
#define ADDELEMENTFORM_H

#include <QDialog>

namespace Ui {
  class AddElementForm;
}

class QTableWidget;
class QTableWidgetItem;

class AddElementForm : public QDialog
{
  Q_OBJECT

public:
  explicit AddElementForm(QWidget *parent = nullptr);
  ~AddElementForm();

  const QString getParent();
  const QVariant getXml();

  const QString getName();
  const QVariant getAttributes();
  const QString getContent();
  const QString getComment();
  /**
   * @brief
   * возвращает статус добавления: 0 - в начало, 1 - в конец
   */
  int getStatusPosition();

signals:
  void myAccept();

private slots:
  void on_m_pBAdd_clicked();

  void on_m_pBDelete_clicked();

  void on_m_tableWidget_itemChanged(QTableWidgetItem *item);

  void on_m_tableWidget_clicked(const QModelIndex &index);

private:
  Ui::AddElementForm *ui;
  QModelIndex *m_currnetIndex;
  QTableWidget *m_table;
};

#endif // ADDELEMENTFORM_H
