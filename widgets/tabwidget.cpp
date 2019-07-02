#include "tabwidget.h"
#include <QDebug>
#include <QModelIndex>

TabWidget::TabWidget(QWidget *parent)
  : QTabWidget(parent)
{
  connect(this, &TabWidget::tabBarClicked, this, &TabWidget::nextSignalSelect);
  connect(this, &TabWidget::tabCloseRequested, this, &TabWidget::closeTab);
}

void TabWidget::nextSignalSelect(int index)
{
  QWidget *widget = QTabWidget::widget(index);
  emit siganlSelect(*indexes[widget]);
}

void TabWidget::closeTab(int index)
{
  QWidget *widget = QTabWidget::widget(index);
  delete indexes[widget];
  indexes.remove(widget);
  delete widget;
}

const QModelIndex *TabWidget::getModelIndex(QWidget *widget)
{
  return indexes[widget];
}

const QModelIndex *TabWidget::getModelIndex(const int index)
{
  QWidget *widget = QTabWidget::widget(index);
  return indexes[widget];
}

int TabWidget::addTab(QWidget *widget, const QString &string, const QModelIndex &index)
{
  indexes[widget] = new QModelIndex(index);
  return QTabWidget::addTab(widget, string);
}

int TabWidget::addTab(QWidget *widget, const QIcon &icon, const QString &label, const QModelIndex &index)
{
  indexes[widget] = &index;
  return QTabWidget::addTab(widget, icon, label);
}
