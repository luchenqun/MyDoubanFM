#ifndef WidgetMain_H
#define WidgetMain_H

#include "WidgetBase.h"
#include "WidgetTitle.h"
#include "WidgetSettings.h"
#include "WidgetPlay.h"
#include "DownloadControl.h"
#include "NetControl.h"

class WidgetMain : public WidgetBase
{
    Q_OBJECT

public:
    WidgetMain(QWidget *parent = 0);
    ~WidgetMain();
private slots:
	void statusChange(NET_HANDLE netHandle);
	void progress(NET_HANDLE netHandle);
private:
    WidgetTitle *m_WidgetTitle;               // 所有菜单栏集合
    WidgetSettings * m_WidgetFunction;
    WidgetPlay * m_WidgetPlay;
    const int m_WidgetMainMargins;

    NetControl *m_nc;
    NET_HANDLE m_netHttpGet;
	NET_HANDLE m_netHttpPost;
	NET_HANDLE m_netDownfile;
};

#endif // WidgetMain_H
