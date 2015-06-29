/**
* @defgroup ToolButton.c
* @{
* @code
Copyright (c) 2014, 广东小天才科技有限公司
All rights reserved.

文件名称： ToolButton.c
文件标识： ToolButton类实现
文件摘要： 作为软件功能功能菜单栏里面的菜单按钮使用（如：最小化，最大化，关闭等）

修改历史：
版本      日期          作者          修改内容
-----    ----------   -------       ----------------
V1.0     2014.01.16   卢郴群         程序开始编写
* @endcode
*/
#include "PushButton.h"

PushButton::PushButton(QWidget *parent)
    :QPushButton(parent)
{
    m_status = NORMAL;
    m_mousePress = false;
    m_picEnter = "";
    this->setFocusPolicy(Qt::NoFocus);
}

PushButton::~PushButton()
{

}

/**
* @brief 设置资源
* @param [in] picName 资源路径
* @return 无
* @par 修改历史：
* @code
  姓名          日期          说明
  --------     ----------   --------------------
  卢郴群        2014.01.19   程序开始编写
* @endcode
*/
void PushButton::setPicName(QString picName)
{
    m_picName = picName;
}

void PushButton::setPicDisabled(const QString name)
{
    m_picDisabled = name;
}

void PushButton::setPicPress(const QString name)
{
    m_picPress = name;
}

void PushButton::setPicEnter(const QString name)
{
    m_picEnter = name;
}

/**
* @brief 进入焦点
* @param [in] event
* @return 无
* @par 修改历史：
* @code
  姓名          日期          说明
  --------     ----------   --------------------
  卢郴群        2014.01.19   程序开始编写
* @endcode
*/
void PushButton::enterEvent(QEvent *)
{
    if(m_status == DISABLED)
        return;

    // 修正鼠标放上去当点击菜单鼠标悬浮到到菜单选项后移开鼠标再单击鼠标左键菜单栏显示进入的BUG
    QPoint p = this->mapFromGlobal(QCursor::pos());
    if(this->rect().contains(p))
    {
        m_status = ENTER;
        update();
    }
}

/**
* @brief 点击鼠标左键
* @param [in] event
* @return 无
* @par 修改历史：
* @code
  姓名          日期          说明
  --------     ----------   --------------------
  卢郴群        2014.01.19   程序开始编写
* @endcode
*/
void PushButton::mousePressEvent(QMouseEvent *event)
{
    if(m_status == DISABLED)
        return;

    QPoint p = this->mapFromGlobal(QCursor::pos());
    if(event->button() == Qt::LeftButton && this->rect().contains(p))
    {
        m_mousePress = true;
        m_status = PRESS;
        update();
    }
}

/**
* @brief 鼠标左键松开
* @param [in] event
* @return 无
* @par 修改历史：
* @code
  姓名          日期          说明
  --------     ----------   --------------------
  卢郴群        2014.01.19   程序开始编写
* @endcode
*/
void PushButton::mouseReleaseEvent(QMouseEvent *event)
{
    if(m_status == DISABLED)
        return;

    if(m_mousePress && this->rect().contains(event->pos()))
    {
        m_mousePress = false;
        m_status = ENTER;
        update();
        emit clicked();
    }
}

/**
* @brief 鼠标离开
* @param [in] event
* @return 无
* @par 修改历史：
* @code
  姓名          日期          说明
  --------     ----------   --------------------
  卢郴群        2014.01.19   程序开始编写
* @endcode
*/
void PushButton::leaveEvent(QEvent *)
{
    if(m_status != DISABLED)
    {
        m_status = NORMAL;
        update();
    }
}

/**
* @brief 主要实现鼠标按上去或者经过改变按钮背景色
* (NOTICE:资源的文件必须这样，在同一路径下面，命名为xxx, xxx_hover, xxx_pressed为一组。分别表示 常态，鼠标悬浮在上面，鼠标按下三种状态应该贴的图片)
* @param [in] event
* @return 无
* @par 修改历史：
* @code
  姓名          日期          说明
  --------     ----------   --------------------
  卢郴群        2014.01.19   程序开始编写
* @endcode
*/
void PushButton::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    QPixmap pixmap;

    QString tmpPicName = m_picName.left(m_picName.length() - 4);

    switch(m_status)
    {
    case NORMAL:
        if(!pixmap.load(m_picName))
        {
            pixmap.load(m_picName);
        }
        break;

    case ENTER:
        if(!pixmap.load(m_picEnter) && !pixmap.load(tmpPicName + QString("_hover.png")))
        {
            pixmap.load(m_picName);
        }
        break;

    case PRESS:
        if(!pixmap.load(m_picPress) && !pixmap.load(tmpPicName + QString("hover")))
            pixmap.load(m_picName);

        break;

    case NOSTATUS:
        pixmap.load(m_picName);
        break;

    case DISABLED:
        if(!pixmap.load(m_picDisabled))
            pixmap.load(m_picName);
        break;

    default:
        pixmap.load(m_picName);
    }

    this->setFixedSize(pixmap.size());
    painter.drawPixmap(rect(), pixmap);
}

void PushButton::setState(ButtonStatus state)
{
    m_status = state;
}
