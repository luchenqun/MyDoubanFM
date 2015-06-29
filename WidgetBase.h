/**
* @defgroup WidgetBase.h
* @{
* @code
Copyright (c) 2014, 广东小天才科技有限公司
All rights reserved.

文件名称： WidgetBase.h
文件标识： WidgetBase类结构
文件摘要： 作为WidgetMain类的基类使用

修改历史：
版本      日期          作者          修改内容
-----    ----------   -------       ----------------
V1.0     2014.01.16   卢郴群         程序开始编写
* @endcode
*/

#ifndef WidgetBase_H
#define WidgetBase_H

#include <QDialog>
#include <QWidget>
#include <QPainter>
#include <QMouseEvent>
#include <qmath.h>

class WidgetBase : public QDialog
{
    Q_OBJECT
public:
    explicit WidgetBase(QWidget *parent = 0);
    ~WidgetBase();

    void setWidgetMoveRange(const int y);
    int getWidgetMoveRange() const;

signals:

public slots:

protected:
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    virtual void paintEvent(QPaintEvent *event);

private:
    QPoint m_movePoint; // 移动的距离
    bool m_mousePress;  // 按下鼠标左键
    int m_y;
};

#endif // WidgetBase_H
