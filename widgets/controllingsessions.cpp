#include "controllingsessions.h"
#include "ui_controllingsessions.h"

ControllingSessions::ControllingSessions(QWidget *parent) :
  QDialog(parent),
  ui(new Ui::ControllingSessions)
{
  ui->setupUi(this);
}

ControllingSessions::~ControllingSessions()
{
  delete ui;
}

void ControllingSessions::on_mPbCreate_pressed()
{

}
