﻿/**
* @defgroup Common.h
* @{
* @code
Copyright (c) 2014, 广东小天才科技有限公司
All rights reserved.

文件名称： Common.h
文件标识： 公共宏
文件摘要： 所有可用的公共宏均在这里面定义

修改历史：
版本      日期          作者          修改内容
-----    ----------   -------       ----------------
V1.0     2014.01.16   卢郴群         程序开始编写
* @endcode
*/
#include <QString>
#include <QJsonDocument>
#include <QFile>
#include <QProcess>

#ifndef COMMON_H
#define COMMON_H

#define DEBUG_MODE 0   // NOTICE:是否开启调试模式(0:关闭调试模式，1:开启调试模式)
#define WIDGET_CLOSE 0  // 窗口是否关闭

#define SOFTWARE_VERSION ("1.11")
#define SOFTWARE_DATA   ("2014/12/05")

#define MAIN_WIDGET_MIN_WIDTH   (992+20)     // 窗口的最小化宽度
#define MAIN_WIDGET_MIN_HEIGHT  (736+20)     // 窗口的最小化高度
#define MAIN_WIDGET_SHADOW_WIDTH    5      // 窗口的阴影宽度

#define TITLE_WIDGET_FIXED_HEIGHT   101     // 顶部标题栏固定高度
#define FOOT_WIDGET_FIXED_HEIGHT    28      // 底部菜单栏固定高度

// 底部网址链接ID
#define FOOT_HOME_HTTP_ID   1
#define FOOT_PRODUCT_ID     2
#define FOOT_MON_FORUM_ID   3
#define FOOT_SERVICE_ID     4

#define LOCAL_SYSTEM_INFO_FILE   ("systemData.json")
#define LOCAL_APP_INFO_FILE      ("appData.json")

#define APP_INFO_BY_PAGE "http://jm.okii.com/getJmDownList/"
#define APP_INFO_BY_ID   "http://jm.okii.com/getInfoByID?id="
#define APP_SYSTEM_ALL   "http://jm.okii.com/getJmDownList/allAppNew"
#define APP_SYSTEM_INFO  "http://jm.okii.com/getFirmware"
#define REWARD_INFO_PAGE "http://jm.okii.com/getCurrentTurntable"
#define HOME_PAGE_AD     "http://jm.okii.com/index.html"
#define FEEDBACK_DATA_POST  "http://jm.okii.com/jianyi/add?"

#define APP_APP_WIDGET_PIC  ":/img/UI_XZGL_LB_TP"
#define  ROW_HEIGHT  25

#define MAKE_FOURCE(c0, c1, c2, c3) (c3 | c2 << 8 | c1 << 16 | c0 << 24)
#define APP_PATH        "B:/"
#define APP_USE_PATH    "B:/UseRecord"
#define BACKUP_FILENAME "UserInfo.set"
#define USER_INFO_DIR   "B:/DownloadToolUserInfo"
#define BACKUP_FILEPATH "B:/DownloadToolUserInfo/UserInfo.set"
#define LOTTERY_INFO_PATH  "B:/DownloadToolUserInfo/LotteryInfo.info"

// 抽奖信息
typedef struct _lotteryInfo_
{
    quint32 year;
    quint32 month;
    quint32 day;
    quint32 countPay;  // 付费次数（费指智慧币）
    quint32 countFree; // 免费次数
}LotteryInfo;

// 防沉迷信息
class AntiPornInfo
{
public:
    bool isError;
    quint32 version;
    quint32 on;
    quint32 timeRun;
    quint32 timeRest;

    AntiPornInfo()
    {
        isError = false;
        version = 1;
        on = 1;
        timeRun = 30;
        timeRest = 15;
    }
};

typedef struct _tag_AvoidWowData{
    quint32 awStructVer;				///防沉迷数据结构版本号
    quint32 awIsEnable;				///防沉迷开关
    quint32 awPlayTimeSet;			///防沉迷游戏时间设置值
    quint32 awRestTimeSet;			///防沉迷休息时间设置值

    quint32 awIsPause;				///防沉迷暂停标志
    quint32 awState;					///防沉迷状态标志
    quint32 awPlayTimeCnt;			///防沉迷游戏时间计数器
    quint32 awRestTimeCnt;			///防沉迷休息时间计数器
    quint32 awPlayTimeStart;			///防沉迷开始游戏时间
    quint32 awRestTimeStart;			///防沉迷开始休息时间

    quint32 awPowerOffTime;			///防沉迷关机时间
}AvoidWowData,*P_AvoidWowData;

// NOTICE:页面切换ID(请按照在WidgetMain.cpp堆栈布局里面按照顺序依次添加宏定义)
enum TURN_PAGE_ID
{
    HOME_PAGE = 0,
    TRUN_TABLE_PAGE,
    NEW_GOODS_PAGE,
    ANTI_ADDICTION_PAGE,
};

enum MACHINT_APP_STATE
{
    APP_STATE_DAMAGE,
    APP_STATE_WAIT_UPDATE,
    APP_STATE_LATEST,
    APP_STATE_NOT_USED,

    APP_STATE_CNT,
};

QString fileSizeStr(qint64 bytes);
QString bytesToGiga(qint64 bytes);

void saveJsonData(QJsonDocument jsonDoc, QString filePath);
QJsonDocument readJsonData(QString filePath);

bool isDriveNotInstall();
void deleteUsbReg();
void installUsbDrive();
void refreshUsbDrive();
qint32 stringVerToIntVer(QString ver);
QString getMaxFreeSpaceDisk();
quint64 getDiskFreeSpace(QString drive);
bool isDiskSpaceFree(QString path, qint64 size);
bool isInstallRun();
QString getNetTime();
bool shutdownSystem();

#endif // COMMON_H