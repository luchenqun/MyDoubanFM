#ifndef WidgetMain_H
#define WidgetMain_H

#include "WidgetBase.h"
#include "WidgetTitle.h"
#include "WidgetSettings.h"
#include "WidgetPlay.h"
#include "DownloadControl.h"
#include "NetControl.h"
#include "DoubanAPI.h"
#include "WidgetLyric.h"
#include "PushButton.h"

class WidgetMain : public WidgetBase
{
    Q_OBJECT

public:
    WidgetMain(QWidget *parent = 0);
    ~WidgetMain();
private:
    void initWidgetTitle();
private slots:

private:
    const int m_WidgetMainMargins;
//    WidgetTitle *m_widgetTitle;               // 所有菜单栏集合
    WidgetSettings *m_widgetSettings;
    WidgetLyric *m_widgetLyric;
    WidgetPlay *m_widgetPlay;

    PushButton *m_minBtn;         // 最小化
    PushButton *m_closeBtn;       // 关闭
};

#endif // WidgetMain_H
