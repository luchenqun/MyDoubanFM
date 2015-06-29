#ifndef WidgetMain_H
#define WidgetMain_H

#include "WidgetBase.h"
#include "WidgetTitle.h"
#include "WidgetFunction.h"
#include "WidgetPlay.h"

class WidgetMain : public WidgetBase
{
    Q_OBJECT

public:
    WidgetMain(QWidget *parent = 0);
    ~WidgetMain();
private:
    WidgetTitle *m_WidgetTitle;               // 所有菜单栏集合
    WidgetFunction * m_WidgetFunction;
    WidgetPlay * m_WidgetPlay;
    const int WidgetMainMargins = 10;
};

#endif // WidgetMain_H
