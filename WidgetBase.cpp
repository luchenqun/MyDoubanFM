/**
* @defgroup WidgetBase.c
* @{
* @code
Copyright (c) 2014, 广东小天才科技有限公司
All rights reserved.

文件名称： WidgetBase.c
文件标识： WidgetBase类实现
文件摘要： 作为WidgetMain类的基类使用

修改历史：
版本      日期          作者          修改内容
-----    ----------   -------       ----------------
V1.0     2014.01.16   卢郴群         程序开始编写
* @endcode
*/

#include "WidgetBase.h"
#include "Common.h"

WidgetBase::WidgetBase(QWidget *parent) :
    QDialog(parent)
{
    setWindowFlags(Qt::FramelessWindowHint | Qt::WindowMinimizeButtonHint | Qt::Dialog);// 无边框窗口 | 点击任务栏图标进行显示/不显示切换
    setAttribute(Qt::WA_TranslucentBackground); // 设置窗体透明

    //初始化为未按下鼠标左键
    m_mousePress = false;
    m_y = 0;
}

WidgetBase::~WidgetBase()
{

}

void WidgetBase::setWidgetMoveRange(const int y)
{
    m_y = y;
}

int WidgetBase::getWidgetMoveRange() const
{
    return m_y;
}

/**
* @brief 重写绘图，主要实现窗体阴影效果
* @param [in] 无
* @return 无
* @par 修改历史：
* @code
  姓名          日期          说明
  --------     ----------   --------------------
  卢郴群        2014.01.16   程序开始编写
* @endcode
*/
void WidgetBase::paintEvent(QPaintEvent *)
{
    QPainterPath path;
    path.setFillRule(Qt::WindingFill);
    path.addRect(10, 10, this->width()-20, this->height()-20);

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);
    painter.fillPath(path, QBrush(Qt::white));

    QColor color(0, 0, 0, 50);
    for(int i=0; i<10; i++)
    {
        QPainterPath path;
        path.setFillRule(Qt::WindingFill);
        path.addRect(10-i, 10-i, this->width()-(10-i)*2, this->height()-(10-i)*2);
        color.setAlpha(150 - qSqrt(i)*50);
        painter.setPen(color);
        painter.drawPath(path);
    }

    painter.setPen(Qt::NoPen);
    painter.setBrush(Qt::white);
    painter.drawRect(QRect(MAIN_WIDGET_SHADOW_WIDTH,
                           MAIN_WIDGET_SHADOW_WIDTH,
                           this->width()-2*MAIN_WIDGET_SHADOW_WIDTH,
                           this->height()-2*MAIN_WIDGET_SHADOW_WIDTH));
}

/**
* @brief 鼠标按下事件重写
* @param [in] event
* @return 无
* @par 修改历史：
* @code
  姓名          日期          说明
  --------     ----------   --------------------
  卢郴群        2014.01.16   程序开始编写
* @endcode
*/
void WidgetBase::mousePressEvent(QMouseEvent *event)
{
    //只能是鼠标左键移动和改变大小
    if(event->button() == Qt::LeftButton && event->pos().y() <= m_y)
    {
        m_mousePress = true;
    }

    //窗口移动距离
    m_movePoint = event->globalPos() - pos();
}

/**
* @brief 鼠标释放事件重写
* @param [in] event
* @return 无
* @par 修改历史：
* @code
  姓名          日期          说明
  --------     ----------   --------------------
  卢郴群        2014.01.16   程序开始编写
* @endcode
*/
void WidgetBase::mouseReleaseEvent(QMouseEvent *)
{
    m_mousePress = false;
}

/**
* @brief 鼠标移动事件重写（实现窗口移动）
* @param [in] event
* @return 无
* @par 修改历史：
* @code
  姓名          日期          说明
  --------     ----------   --------------------
  卢郴群        2014.01.16   程序开始编写
* @endcode
*/
void WidgetBase::mouseMoveEvent(QMouseEvent *event)
{
    if(m_mousePress)
    {
        QPoint movePos = event->globalPos();
        this->move(movePos - m_movePoint);
    }
}
