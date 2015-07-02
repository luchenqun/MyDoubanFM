#include "Common.h"
#include <QCoreApplication>
#include <QDebug>
#include <QDir>
#include <QSettings>
#include <QDateTime>
#include <QDate>
#include <QTime>
#include <QtMath>

#ifdef Q_OS_WIN
#include <windows.h>
#endif

// 文件大小转换
QString fileSizeStr(qint64 bytes)
{
    QString units[] = {"B", "KB", "MB", "GB", "TB"};
    double sizeConvent = 0.0;
    quint8 index = 0;

    if (bytes >= pow((float)1024, (int)4))
    {
        sizeConvent = bytes / pow((float)1024, (int)4);
        index = 4;
    }
    else if (bytes >= pow((float)1024, (int)3))
    {
        sizeConvent = bytes / pow((float)1024, (int)3);
        index = 3;
    }
    else if (bytes >= pow((float)1024, (int)2))
    {
        sizeConvent = bytes / pow((float)1024, (int)2);
        index = 2;
    }
    else if (bytes >= pow((float)1024, (int)1))
    {
        sizeConvent = bytes / pow((float)1024, (int)1);
        index = 1;
    }
    else
    {
        sizeConvent = bytes;
        index = 0;
    }

    return (QString::number(sizeConvent, 'f', 2) + units[index]);

}

QString bytesToGiga(qint64 bytes)
{
    float giga = bytes / (1024.0*1024.0*1024.0);
    QString str = QString::number(giga, 'f', 2);
    int len;
    while(true)
    {
        len = str.length();
        if(str[len - 1] == '0')
        {
            str.remove(len-1, 1);
        }
        else
        {
            break;
        }
    }

    len = str.length();
    if(str[len - 1] == '.')
        str.remove(len-1, 1);

    return str;

}

void saveJsonData(QJsonDocument jsonDoc, QString filePath)
{
    QFile jsonFile(filePath);
    jsonFile.open(QFile::WriteOnly);
    jsonFile.write(jsonDoc.toJson(QJsonDocument::Indented));
    jsonFile.close();
}

QJsonDocument readJsonData(QString filePath)
{
    QFile jsonFile(filePath);
    jsonFile.open(QFile::ReadOnly);
    QJsonDocument jsonDoc = QJsonDocument().fromJson(jsonFile.readAll());
    jsonFile.close();

    return jsonDoc;
}

bool isDriveNotInstall()
{
    QSettings reg("HKEY_LOCAL_MACHINE\\SYSTEM\\CurrentControlSet\\services\\USBSTOR\\Enum", QSettings::NativeFormat);
    QStringList keyLst = reg.childKeys();
    foreach (QString key, keyLst) {
        QString v = reg.value(key).toString();
//        qDebug() << "reg:" << v;
        bool isVid = v.contains("VID_C7C4", Qt::CaseInsensitive);
        bool isPid = v.contains("PID_C4A5", Qt::CaseInsensitive);
        if(isVid && isPid)
            return true;
    }

    return false;
}

void deleteUsbReg()
{
    QSettings reg("HKEY_LOCAL_MACHINE\\SYSTEM\\CurrentControlSet\\services\\USBSTOR\\Enum", QSettings::NativeFormat);
    QStringList keyLst = reg.childKeys();
    foreach (QString key, keyLst) {
        reg.remove(key);
    }
}

void installUsbDrive()
{
    QProcess *p = new QProcess();
    qDebug() << "run InstallDriverN1.exe dir" << QDir::currentPath();
    bool ret = p->startDetached("InstallDriverN1.exe");
    qDebug() << "install ret:" << ret;
    p->deleteLater();
}

void refreshUsbDrive()
{
    QProcess p;
    QString exe = "reflesh.exe";
    p.start(exe);
    p.waitForFinished(-1);

    QByteArray array =  p.readAllStandardOutput();
    qDebug() << "reflesh ret:" << QString(array);
}

qint32 stringVerToIntVer(QString ver)
{
    QStringList verlist = ver.split(".");

    if(verlist.count() != 2)
    {
        return -1;
    }

    qint32 verRet = (verlist[0].toInt() << 16) + verlist[1].toInt();
    return verRet;
}

QString getMaxFreeSpaceDisk()
{
    QString drive = "";

    QFileInfoList drivesList = QDir::drives();

    if(drivesList.size() > 0)
    {
        QFileInfo info = drivesList.at(0);
        drive = info.absolutePath();
        qDebug() << "drives: " << drive;
        quint64 maxSpace = getDiskFreeSpace(drive);

        foreach (QFileInfo info, drivesList)
        {
            QString str = info.absolutePath();
            quint64 space = getDiskFreeSpace(str);
            if(space > maxSpace)
            {
                drive = str;
                maxSpace = space;
            }
        }
    }
    return drive;
}

quint64 getDiskFreeSpace(QString drive)
{
#ifdef Q_OS_WIN
    LPCWSTR  lpDrive = (LPCWSTR )drive.utf16();
    if(GetDriveType(lpDrive) == DRIVE_FIXED)
    {
        ULARGE_INTEGER freeBytesAvailable;
        ULARGE_INTEGER totalNumberOfBytes;
        ULARGE_INTEGER totalNumberOfFreeBytes;
        if(GetDiskFreeSpaceEx(lpDrive, &freeBytesAvailable, &totalNumberOfBytes, &totalNumberOfFreeBytes))
        {
            qDebug() << drive << ": " << totalNumberOfFreeBytes.QuadPart;
            return (quint64)totalNumberOfFreeBytes.QuadPart;
        }
    }
    return 0;
#else

#endif
}


bool isDiskSpaceFree(QString path, qint64 size)
{
    QString drive = path.left(2);
    quint64 space = getDiskFreeSpace(drive);
    if(size <= 0)
        return true;

    return (space > size);
}

// 防止安装时打开
bool isInstallRun()
{
    bool ret = false;
    // 创建互斥量, 和安装包、补丁包的互斥量同名
    HANDLE m_hMutex = CreateMutexA(NULL, FALSE, "downloadtool_mutex");
    // 检查错误代码
    if (GetLastError() == ERROR_ALREADY_EXISTS)
    {
        ret =  true;
    }
    CloseHandle(m_hMutex);
    return ret;
}

#if 0
bool shutdownSystem()
{
#ifdef Q_OS_WIN
    HANDLE hToken;
    TOKEN_PRIVILEGES tkp;

    qDebug() << "now shutdown system.";

    //获取进程标志
    if(!OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &hToken))
    {
        qDebug() << "OpenProcessToken failed.";
        return false;
    }

    //获取关机特权的LUID
    LookupPrivilegeValue(NULL, SE_SHUTDOWN_NAME, &tkp.Privileges[0].Luid);
    tkp.PrivilegeCount = 1;
    tkp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;

    //获取这个进程的关机特权
    AdjustTokenPrivileges(hToken, false, &tkp, 0, (PTOKEN_PRIVILEGES)NULL, 0);
    if(GetLastError() != ERROR_SUCCESS)
    {
        qDebug() << "shutdownSystem error" << GetLastError();
        return false;
    }

    // 强制关闭计算机
    if(!ExitWindowsEx(EWX_SHUTDOWN | EWX_FORCE, 0))
    {
        qDebug() << "shutdownSystem ExitWindowsEx failed.";
        return false;
    }

    return true;

#else

#endif
}
#endif
