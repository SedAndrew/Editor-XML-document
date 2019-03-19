#include "workspace.h"
#include "ui_workspaceui.h"

WorkSpace::WorkSpace(QWidget *parent)
  : QWidget(parent),
    m_workUI(new Ui::WorkSpaceUI)
{
  m_workUI->setupUi(this);
}
