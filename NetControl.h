#ifndef NETCONTROL_H
#define NETCONTROL_H

#include <QObject>
#include <QList>
#include <QThread>
#include <QDir>
#include <QApplication>
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

	bool netHandleCanUse(NET_HANDLE handle);
	void setDefautTaskDir(QString dirPath);
	QString getDefautTaskDir();
	bool setTaskDir(NET_HANDLE handle, QString dirPath);
	QString getTaskDir(NET_HANDLE handle);

	int getNetCode(NET_HANDLE handle);
	QString getReceiveData(NET_HANDLE handle);
	int getProgress(NET_HANDLE handle);
    QString getFilePath(NET_HANDLE handle);
signals:
	void startTasked(NET_HANDLE handle);
	void progressed(NET_HANDLE handle);
	void statusChanged(NET_HANDLE handle);
	void fileSized(NET_HANDLE handle);
public slots:
private:
    static NetControl *m_netControl;
	const QString m_defaultFolderName;		/**< 默认的下载文件件夹名 */
	QList<NET_HANDLE> m_netHandleList;		/**< 网络句柄集合 */
	QString m_taskDir;						/**< 任务目录 */
};

#endif // NETCONTROL_H
