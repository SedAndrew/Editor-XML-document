#ifndef CONTROLLINGSESSIONS_H
#define CONTROLLINGSESSIONS_H

#include <QDialog>

namespace Ui {
  class ControllingSessions;
}

class ControllingSessions : public QDialog
{
  Q_OBJECT

public:
  explicit ControllingSessions(QWidget *parent = nullptr);
  ~ControllingSessions();

private slots:
  void on_mPbCreate_pressed();

private:
  Ui::ControllingSessions *ui;
};

#endif // CONTROLLINGSESSIONS_H
