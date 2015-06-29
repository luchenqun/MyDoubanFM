#include "WidgetFunction.h"
#include <QLabel>
#include <QVBoxLayout>
#include <QListWidget>

WidgetFunction::WidgetFunction(QWidget *parent) : QWidget(parent)
{
    QStringList list;
    for(int i=100; i<=1030; i++)
    {
        list << QString::number(i);
    }

    QListWidget *listWidget = new QListWidget(this);
    listWidget->setMinimumSize(300,300);
    listWidget->addItems(list);
}

