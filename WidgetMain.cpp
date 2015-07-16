#include "WidgetMain.h"
#include <QApplication>
#include <QTime>


WidgetMain::WidgetMain(QWidget *parent)
    : WidgetBase(parent),
    m_WidgetMainMargins(0)
{
    this->setFixedSize(600 + 10, 300 + 10);
    setWindowIcon(QIcon(":/img/res/img/my_douban_fm.ico"));	// 设置任务栏图标
    this->setWidgetMoveRange(50);

//    m_widgetTitle = new WidgetTitle(this);


    m_widgetSettings = new WidgetSettings(this);
    m_widgetPlay = new WidgetPlay(this);
    m_widgetLyric = new WidgetLyric(this);

    initWidgetTitle();

//    m_widgetTitle->move(5, 5);
    m_minBtn->move(520, 15);
    m_closeBtn->move(560, 15);
    m_widgetSettings->move(5, 5);
    m_widgetPlay->move(305, 5);
    m_widgetLyric->move(5, 5);

    this->setContentsMargins(m_WidgetMainMargins, m_WidgetMainMargins, m_WidgetMainMargins, m_WidgetMainMargins);

    connect(m_widgetPlay, &WidgetPlay::songPicUpdated, m_widgetLyric, &WidgetLyric::songPicUpdate);


#if 0
    this->setStyleSheet("border:1px solid green");
#endif
}

void WidgetMain::initWidgetTitle()
{
    // 按钮
    m_minBtn = new PushButton(this);
    m_closeBtn = new PushButton(this);
    m_minBtn->setFocusPolicy(Qt::NoFocus);
    m_closeBtn->setFocusPolicy(Qt::NoFocus);

    m_minBtn->setPicName(QString(":/img/res/img/minus.png"));
    m_closeBtn->setPicName(QString(":/img/res/img/delete.png"));

    connect(m_minBtn, &PushButton::clicked, [=](void)
    {
        m_minBtn->setState(PushButton::NORMAL);
        m_minBtn->repaint();
    });

    connect(m_closeBtn, &PushButton::clicked, [=](void)
    {
        m_closeBtn->setState(PushButton::NORMAL);
        m_closeBtn->repaint();
        qApp->quit();
    });

    m_minBtn->setToolTip(QStringLiteral("最小化"));
    m_closeBtn->setToolTip(QStringLiteral("关闭"));
}

WidgetMain::~WidgetMain()
{
}
