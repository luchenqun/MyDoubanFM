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
private:
	explicit NetControl(QObject *parent = 0);
public:
    static NetControl* singleton();
	NET_HANDLE createTask(const QString url, NetWork::TaskType taskType);
    NetWork::NetWokeCode startTask(NET_HANDLE handle);
    NetWork::NetWokeCode pauseTask(NET_HANDLE handle);
    NetWork::NetWokeCode resumeTask(NET_HANDLE handle);
    NetWork::NetWokeCode deleteTask(NET_HANDLE handle);

	int getStatus(NET_HANDLE handle);
	QString getReceiveData(NET_HANDLE handle);
signals:
	void startTasked(NET_HANDLE handle);
	void progressed(NET_HANDLE handle);
	void statusChanged(NET_HANDLE handle);
public slots:
private:
    static NetControl *m_netControl;
    QList<NET_HANDLE> m_netHandleList;
};

#endif // NETCONTROL_H
