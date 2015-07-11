#ifndef WidgetMain_H
#define WidgetMain_H

#include "WidgetBase.h"
#include "WidgetTitle.h"
#include "WidgetSettings.h"
#include "WidgetPlay.h"
#include "DownloadControl.h"
#include "NetControl.h"
#include "DoubanAPI.h"

class WidgetMain : public WidgetBase
{
    Q_OBJECT

public:
    WidgetMain(QWidget *parent = 0);
    ~WidgetMain();
private slots:

private:
    const int m_WidgetMainMargins;
    WidgetTitle *m_WidgetTitle;               // 所有菜单栏集合
    WidgetSettings *m_WidgetFunction;
    WidgetPlay *m_WidgetPlay;
};

#endif // WidgetMain_H
