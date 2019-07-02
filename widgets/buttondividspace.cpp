#include "widgets/buttondividspace.h"

#include <QAction>
#include <QMouseEvent>

ButtonDividSpace::ButtonDividSpace(QWidget *parent)
  : QPushButton(parent)
{
  setContextMenu();
}

void ButtonDividSpace::setContextMenu()
{
  this->addActions({
     m_actionDividHorizontal,
     m_actionDividVertical
   });
  this->setContextMenuPolicy(Qt::ActionsContextMenu);
}


void ButtonDividSpace::mousePressEvent(QMouseEvent *event)
{
  if(event->button() != Qt::LeftButton) return;

  // деление окна

}
