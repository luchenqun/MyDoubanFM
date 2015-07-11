#include "WidgetMain.h"
#include <QApplication>
#include <QTime>


WidgetMain::WidgetMain(QWidget *parent)
    : WidgetBase(parent),
	m_WidgetMainMargins(10)
{
	this->setFixedSize(615 + m_WidgetMainMargins * 2, 308 + m_WidgetMainMargins * 2 + 30);
    setWindowIcon(QIcon(":/img/res/img/my_douban_fm.ico"));	// 设置任务栏图标
    this->setWidgetMoveRange(50);

    m_WidgetTitle = new WidgetTitle(this);

    m_WidgetFunction = new WidgetSettings();
    m_WidgetPlay = new WidgetPlay();
    QHBoxLayout *functionLayout = new QHBoxLayout();
    functionLayout->addWidget(m_WidgetFunction);
    functionLayout->addWidget(m_WidgetPlay);

    // 总体布局
    QVBoxLayout *mainLayout = new QVBoxLayout();
    mainLayout->addWidget(m_WidgetTitle);
    mainLayout->addLayout(functionLayout);
    mainLayout->setSpacing(0);

	mainLayout->setContentsMargins(m_WidgetMainMargins, m_WidgetMainMargins, m_WidgetMainMargins, m_WidgetMainMargins);

    this->setLayout(mainLayout);
}

WidgetMain::~WidgetMain()
{
}
