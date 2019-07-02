#ifndef MYACTION_H
#define MYACTION_H

#include <QAction>


class MyAction : public QAction
{
  Q_OBJECT
public:
  explicit MyAction(const QString &text, QObject *parent = nullptr) : QAction(parent)
  {
    m_action = new QAction(text, this);
    connect(m_action, SIGNAL(changed()), this, SLOT(mySlot()));
  }
  const QAction* getAction() const {
    return m_action;
  }

signals:
  void myChanged(const QString& path);

public slots:
  void mySlot() {
    emit myChanged(m_action->text());
  }

private:
  QAction *m_action;
};

#endif // MYACTION_H
