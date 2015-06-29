/**
* @defgroup WidgetTitle.h
* @{
* @code
Copyright (c) 2014, 广东小天才科技有限公司
All rights reserved.

文件名称： WidgetTitle.h
文件标识： WidgetTitle类声明
文件摘要： 所有的上面的标题栏集合

修改历史：
版本      日期          作者          修改内容
-----    ----------   -------       ----------------
V1.0     2014.01.16   卢郴群         程序开始编写
* @endcode
*/

#ifndef WidgetTitle_H
#define WidgetTitle_H

#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QMouseEvent>
#include <QHBoxLayout>
#include <QSignalMapper>
#include <QVBoxLayout>
#include <QStyleOption>
#include <QMovie>

#include "PushButton.h"

class WidgetTitle : public QWidget
{
    Q_OBJECT
public:
    explicit WidgetTitle(QWidget *parent = 0);
    ~WidgetTitle();
    void paintEvent(QPaintEvent*);
    void wordsTrip();
signals:
    void showMin();     // 最小化
    void closeWidget(); // 关闭界面
private:
    QPoint m_pressPoint;  // 鼠标按下去的点
    bool m_isMove;        // 是否移动

    PushButton *m_minBtn;         // 最小化
    PushButton *m_closeBtn;       // 关闭
};

#endif // WidgetTitle_H
