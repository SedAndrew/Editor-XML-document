#ifndef WORKSPACE_H
#define WORKSPACE_H

#include <QWidget>

namespace Ui {
  class WorkSpaceUI;
}

class WorkSpace : public QWidget
{
  Q_OBJECT
public:
  explicit WorkSpace(QWidget *parent = nullptr);

signals:

public slots:

public:
  Ui::WorkSpaceUI *m_workUI;
};

#endif // WORKSPACE_H
