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

	/** @brief NetWoke类所有状态返回码 */
	typedef enum
	{
		NETWORK_OK = 0,					/**< 正常 */
		NETWORK_CODE_BEGIN = CURL_LAST,	/**< 为了区分CURLcode，将CURLcode最后一个状态置为这里的初始状态 */
		NETWORK_HANDLE_NULL_PTR,		/**< 空指针 */
		NETWORK_FINISH_SUCCESS,			/**< 任务完成 */
	}NetWokeCode;

    explicit NetWork(QObject *parent = 0);
	~NetWork();
	void moveToNewThread();
    QThread* getThread();
    void setTaskType(TaskType taskType);
    TaskType getTaskType();
	void setRunMode(RunMode runMode);
	RunMode getRunMode();
	int getStatus();
	QString getReceiveData();

	int createTask(const QString url);
	int startHttpGet();
	int startHttpPost();
	int startDownloadFile();
	static size_t writeData(void* buffer, size_t size, size_t n, void *user);
	static int CurlDebug(CURL *pcurl, curl_infotype itype, char * pData, size_t size, void *);
private:
	void emitStatus(int status);
signals:
	void progressed(NetWork *netWork);
	void statusChanged(NetWork *netWork);
public slots:
	int startTask(NetWork *netWork);
private:
    CURL *m_curl;				/**< CURL 指针 */
	QThread *m_thread;			/**< 网络任务运行的线程 */
    QString m_url;				/**< 网络链接 */ 
	TaskType m_taskType;		/**< 任务类型 */
	RunMode m_runMode;			/**< 执行方式：异步，同步 */
	QString m_receiveData;		/**< 接收数据 */
	int m_status;				/**< 最新的network状态码(一旦有新的状态码，会覆盖旧状态码，所以程序要及时处理) */
};

#endif // NETWORK_H
