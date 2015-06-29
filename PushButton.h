/**
* @defgroup ToolButton.h
* @{
* @code
Copyright (c) 2014, 广东小天才科技有限公司
All rights reserved.

文件名称： ToolButton.h
文件标识： ToolButton类声明
文件摘要： 作为软件功能功能菜单栏里面的菜单按钮使用（如：最小化，最大化，关闭等）

修改历史：
版本      日期          作者          修改内容
-----    ----------   -------       ----------------
V1.0     2014.01.16   卢郴群         程序开始编写
* @endcode
*/

#ifndef PUSHBUTTON_H
#define PUSHBUTTON_H
#include <QPushButton>
#include <QPainter>
#include <QMouseEvent>
#include <QDebug>

class PushButton : public QPushButton
{
    Q_OBJECT

public:

    explicit PushButton(QWidget *parent = 0);
    ~PushButton();

    //枚举按钮的几种状态
    enum ButtonStatus{NORMAL, ENTER, PRESS, DISABLED, NOSTATUS};

    void setPicName(QString picName);
    void setPicPress(const QString name);
    void setPicEnter(const QString name);
    void setPicDisabled(const QString name);
    void setState(ButtonStatus state);

protected:

    void enterEvent(QEvent *);
    void leaveEvent(QEvent *);
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void paintEvent(QPaintEvent *);

private:
    QString m_picName;
    QString m_picEnter;
    QString m_picPress;
    QString m_picDisabled;
    ButtonStatus m_status;

    bool m_mousePress; //按钮左键是否按下
};

#endif // PUSHBUTTON_H
