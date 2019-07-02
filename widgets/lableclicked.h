#ifndef LABLECLICKED_H
#define LABLECLICKED_H

#include <QLabel>
#include <QDebug>
#include <QMouseEvent>

class LableClicked : public QLabel
{
  Q_OBJECT
public:
  LableClicked(QWidget *parent = nullptr) : QLabel(parent) {
    setMouseTracking(true);
//    connect(this, SIGNAL(clicked()), this, SLOT(slotClicked()));
  }
  LableClicked(const QString &text, QWidget *parent = nullptr) : QLabel(text, parent) {
    setMouseTracking(true);
  }
  virtual ~LableClicked() {}

signals:
  void clicked();

public slots:
  void slotClicked() {
    qDebug() << "Clicked!";
  }

protected:
  void mouseReleaseEvent(QMouseEvent *event)
  {
    Q_UNUSED(event)
    emit clicked();
  }
  void paintEvent(QPaintEvent *event)
  {
      Q_UNUSED(event)
  }
};

#endif // LABLECLICKED_H
