#ifndef BUTTONDIVIDSPACE_H
#define BUTTONDIVIDSPACE_H

#include <QPushButton>

class ButtonDividSpace : public QPushButton
{
public:
  ButtonDividSpace(QWidget *parent = nullptr);

  void setContextMenu();

  // QWidget interface
protected:
  void mousePressEvent(QMouseEvent *event);

private:
  QAction *m_actionDividHorizontal;
  QAction *m_actionDividVertical;
};

#endif // BUTTONDIVIDSPACE_H
