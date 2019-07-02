#ifndef TABWIDGET_H
#define TABWIDGET_H

#include <QMap>
#include <QTabWidget>


class TabWidget : public QTabWidget
{
  Q_OBJECT
public:
  TabWidget(QWidget *parent = nullptr);

signals:
  void siganlSelect(const QModelIndex &);

public slots:
  void nextSignalSelect(int index);
  void closeTab(int index);

public:
  const QModelIndex *getModelIndex(QWidget *widget);
  const QModelIndex *getModelIndex(const int index);

  int addTab(QWidget *widget, const QString &string, const QModelIndex &index);
  int addTab(QWidget *widget, const QIcon& icon, const QString &label, const QModelIndex &index);

private:
  QMap<QWidget*, const QModelIndex *> indexes;

};

#endif // TABWIDGET_H
