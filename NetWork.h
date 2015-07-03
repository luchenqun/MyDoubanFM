#ifndef NETWORK_H
#define NETWORK_H

#include <QObject>
#include <QThread>
#include <QFile>
#include "curl.h"

class NetWork : public QObject
{
    Q_OBJECT
public:
	/** @brief 网络任务类型 */
    enum TaskType
    {
		TASK_HTTP_GET,			/**< HTTP_GET 任务类型 */
		TASK_HTTP_POST,			/**< HTTP_POST 任务类型 */
		TASK_DOWNLOAD_FILE,		/**< 文件下载 */
    };

	/** @brief 网络运行模式 */
	enum RunMode
	{
		ASYNCHRONOUS,			/**< 异步执行 */
		SYNCHRONIZATION,		/**< 同步执行（阻塞式进行） */
	};

    explicit NetWork(QObject *parent = 0);
	~NetWork();
	void moveToNewThread();
    QThread* getThread();
    void setTaskType(TaskType taskType);
    TaskType getTaskType();
	void setRunMode(RunMode runMode);
	RunMode getRunMode();

    int initHttpGet(QString url);
	static size_t writeData(void* buffer, size_t size, size_t n, void *user);
    static size_t httpGetWriteData(void* buffer, size_t size, size_t n, void *user);
signals:

public slots:
	int startTask(NetWork *netWork);
    int startHttpGet();
private:
    CURL *m_curl;				/**< CURL 指针 */
	QThread *m_thread;			/**< 网络任务运行的线程 */
    QString m_url;				/**< 网络链接 */ 
	TaskType m_taskType;		/**< 任务类型 */
	RunMode m_runMode;			/**< 执行方式：异步，同步 */
	QString m_receiveData;		/**< 接收数据 */
};

#endif // NETWORK_H
