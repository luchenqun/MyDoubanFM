#ifndef NETCONTROL_H
#define NETCONTROL_H

#include <QObject>
#include <QList>
#include <QThread>
#include "NetWork.h"
#include "curl.h"

typedef NetWork* NET_HANDLE;

class NetControl : public QObject
{
    Q_OBJECT
public:
    static NetControl* singleton();
    NET_HANDLE createTaskHttpGet(const QString url);
    bool startTask(NET_HANDLE handle);
    bool pauseTask(NET_HANDLE handle);
    bool resumeTask(NET_HANDLE handle);
    bool deleteTask(NET_HANDLE handle);
private:
    explicit NetControl(QObject *parent = 0);

signals:
    void startHttpGeted(NET_HANDLE handle);
public slots:
private:
    static NetControl *m_netControl;
    QList<NET_HANDLE> m_netHandleList;
};

#endif // NETCONTROL_H
