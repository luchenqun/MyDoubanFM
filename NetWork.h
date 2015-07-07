#ifndef NETWORK_H
#define NETWORK_H

#include <QObject>
#include <QThread>
#include <QFile>
#include <QDir>
#include <QFileInfo>
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
		NETWORK_NO_TASK,				/**< 没有进行任务 */
		NETWORK_DISK_NO_SPACE,			/**< 磁盘没剩余空间 */
		NETWORK_INIT_FILE_SUCCESS,		/**< 文件初始化成功 */
		NETWORK_OPEN_FILE_FAIL,			/**< 打开文件失败 */
		NETWORK_FINISH_SUCCESS,			/**< 任务完成 */

		NETWORK_LAST,					/**< 最后标记，请不要使用用 */
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
	int getProgress();
	bool setFileDir(QString dirPath);
	QString getFileDir();
	bool setFilePath(QString filePath);
	QString getFilePath();
	bool setFileName(QString fileName);
	QString getFileName();

	int createTask(const QString url);
	int startHttpGet();
	int startHttpPost();
	int startDownloadFile();
	static size_t writeData(void* buffer, size_t size, size_t n, void *user);
	static int progress(void *p, curl_off_t dltotal, curl_off_t dlnow, curl_off_t ultotal, curl_off_t ulnow);
	static int curlDebug(CURL *pcurl, curl_infotype itype, char * pData, size_t size, void *);
	static qint64 getDiskFreeSpace(QString diskPath);
	static char* QStringToChar(QString str);
private:
	void emitStatus(int status);
	void emitFileSize(double fileSize);
	int initFile();
signals:
	void progressed(NetWork *netWork);
	void statusChanged(NetWork *netWork);
	void fileSized(NetWork *netWork);
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
	QFile m_downloadFile;		/**< 下载的文件 */
	QFile m_configureFile;		/**< 下载的配置文件，用于记录断点下载的起始点，url等 */
	QString m_fileDir;			/**< 文件目录 */
	QString m_filePath;			/**< 文件路径 */
	QString m_fileName;			/**< 文件名 */
	qint64 m_fileSize;			/**< 文件大小 */
	qint64 m_downloadedSize;	/**< 已结下好的大小 */
	qint64 m_breakPoint;		/**< 文件下载断点续传 */
};

#endif // NETWORK_H
