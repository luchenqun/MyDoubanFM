#include "WidgetPlay.h"
#include <QLabel>

WidgetPlay::WidgetPlay(QWidget *parent) : QWidget(parent)
{
    QLabel *label = new QLabel("WidgetPlay", this);
    label->setFixedSize(300, 300);
}

