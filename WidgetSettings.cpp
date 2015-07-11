#include "WidgetSettings.h"
#include <QLabel>
#include <QVBoxLayout>
#include <QListWidget>

WidgetSettings::WidgetSettings(QWidget *parent) : QWidget(parent)
{
	QLabel *label = new QLabel("WidgetSettings", this);
	label->setFixedSize(300, 300);
}

