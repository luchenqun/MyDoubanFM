/**
* @defgroup WidgetTitle.c
* @{
* @code
Copyright (c) 2014, 广东小天才科技有限公司
All rights reserved.

文件名称： WidgetTitle.c
文件标识： WidgetTitle类实现
文件摘要： 所有的上面的标题栏集合

修改历史：
版本      日期          作者          修改内容
-----    ----------   -------       ----------------
V1.0     2014.01.16   卢郴群         程序开始编写
* @endcode
*/
#include "WidgetTitle.h"
#include "Common.h"

WidgetTitle::WidgetTitle(QWidget *parent) :
    QWidget(parent)
{
    this->setFixedHeight(30);

    QVBoxLayout *rightLayout = new QVBoxLayout();

    // 按钮
    m_minBtn = new PushButton(this);
    m_closeBtn = new PushButton(this);
    m_minBtn->setFocusPolicy(Qt::NoFocus);
    m_closeBtn->setFocusPolicy(Qt::NoFocus);

    m_minBtn->setPicName(QString(":/img/res/img/minus.png"));
    m_closeBtn->setPicName(QString(":/img/res/img/delete.png"));

    QHBoxLayout *rightLayoutTop = new QHBoxLayout();

    rightLayoutTop->addStretch();
    rightLayoutTop->addWidget(m_minBtn, 0, Qt::AlignTop | Qt::AlignRight );
    rightLayoutTop->addWidget(m_closeBtn, 0, Qt::AlignTop | Qt::AlignRight );
    rightLayoutTop->setSpacing(10);
    rightLayoutTop->setContentsMargins(0, 0, 0, 0);

    connect(m_minBtn, &PushButton::clicked, [=](void)
    {
        m_minBtn->setState(PushButton::NORMAL);
        m_minBtn->repaint();
        emit showMin();
    });
    connect(m_closeBtn, &PushButton::clicked, [=](void)
    {
        m_closeBtn->setState(PushButton::NORMAL);
        m_closeBtn->repaint();
        qApp->quit();
        emit closeWidget();
    });

    rightLayout->addLayout(rightLayoutTop);

    // 总体布局
    QHBoxLayout *mainLayout = new QHBoxLayout();
    mainLayout->addStretch();
    mainLayout->addLayout(rightLayout);
    mainLayout->setContentsMargins(25, 0, 0, 0);

    this->wordsTrip();
    this->setLayout(mainLayout);
}

WidgetTitle::~WidgetTitle()
{
}

/**
* @brief 文字浮动提示与菜单显示
* @param [in] 无
* @return 无
* @par 修改历史：
* @code
  姓名          日期          说明
  --------     ----------   --------------------
  卢郴群        2014.01.16   程序开始编写
* @endcode
*/
void WidgetTitle::wordsTrip()
{
    m_minBtn->setToolTip(QStringLiteral("最小化"));
    m_closeBtn->setToolTip(QStringLiteral("关闭"));
}

/**
* @brief 重写paintEvent函数
* @param [in] QPaintEvent
* @return 无
* @par 修改历史：
* @code
  姓名          日期          说明
  --------     ----------   --------------------
  卢郴群        2014.01.17   程序开始编写
* @endcode
*/
void WidgetTitle::paintEvent(QPaintEvent*)
{
    QStyleOption opt;
    opt.init(this);
    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
#if DEBUG_MODE
    this->setStyleSheet("border:1px solid red");
#endif
}
