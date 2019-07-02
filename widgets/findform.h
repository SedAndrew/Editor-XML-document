#ifndef FINDFORM_H
#define FINDFORM_H

#include <QDialog>
/**
 *
 * Поиск элементов по ключевому слову.
 * Реализован поиск от корневого элемента.
 * Проводит поиск до первого найденного узла.
 *
 */


namespace Ui {
  class FindForm;
}

class Element;
class FindForm : public QDialog
{
  Q_OBJECT

public:
  explicit FindForm(Element *note, QWidget *parent = nullptr);
  ~FindForm();

  const QString getString();
  bool isName();
  bool isAttribut();
  bool isValue();
  bool isContent();
  bool isComment();
  int getStatusPosition();

  bool preOrder(Element *node);
  bool inOrder(Element *node);


public slots:
  void myAccept();
  void onSearch();

signals:
  void isNext(Element*);

private:
  Ui::FindForm *ui;

  Element *m_note;
  Element *m_result;
};

#endif // FINDFORM_H
