/**
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

#define MAIN_WIDGET_SHADOW_WIDTH    5      // 窗口的阴影宽度

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
