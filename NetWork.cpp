#include "NetWork.h"
#include "NetControl.h"
#include <QDebug>
#include <QTime>
#include <QTextCodec>

/** 
* @brief 构造函数
* @author LuChenQun
* @date 2015/07/02
* @param[in] parent 目指针
* @return 无
*/
NetWork::NetWork(QObject *parent) :
    QObject(parent),
    m_thread(nullptr),
	m_runMode(ASYNCHRONOUS),
	m_status(NETWORK_OK),
	m_fileSize(0),
	m_breakPoint(0)
{
    m_curl = curl_easy_init();
}

/** 
* @brief 析构函数
* @author LuChenQun
* @date 2015/07/03
* @return  
*/
NetWork::~NetWork()
{
	m_thread->quit();
	m_thread->wait();
}

/** 
* @brief 将任务移到新线程
* @author LuChenQun
* @date 2015/07/02
* @return void
*/
void NetWork::moveToNewThread()
{
	m_thread = new QThread();
	this->moveToThread(m_thread);
}

/** 
* @brief 获取网络任务在线程的指针
* @author LuChenQun
* @date 2015/07/02
* @return QThread* 线程指针
*/
QThread* NetWork::getThread()
{
    return m_thread;
}

/** 
* @brief 设置网络任务类型
* @author LuChenQun
* @date 2015/07/02
* @param[in] taskType 网络任务类型
* @return void
*/
void NetWork::setTaskType(TaskType taskType)
{
    m_taskType = taskType;
}

/** 
* @brief 获取网络任务类型
* @author LuChenQun
* @date 2015/07/02
* @return NetWork::TaskType 网络任务类型
*/
NetWork::TaskType NetWork::getTaskType()
{
    return m_taskType;
}

/** 
* @brief 设置运行模式：异步(同步)
* @author LuChenQun
* @date 2015/07/03
* @param[in] runMode 运行模式
* @return void
*/
void NetWork::setRunMode(RunMode runMode)
{
	m_runMode = runMode;
}

/** 
* @brief 获取运行模式
* @author LuChenQun
* @date 2015/07/03
* @return NetWork::RunMode 运行模式：异步(同步)
*/
NetWork::RunMode NetWork::getRunMode()
{
	return m_runMode;
}

/** 
* @brief 获取当前的状态码
* @author LuChenQun
* @date 2015/07/05
* @return int
*/
int NetWork::getStatus()
{
	return m_status;
}

/** 
* @brief 获取接收到的数据（比如：使用http get的数据，http post返回的数据等）
* @author LuChenQun
* @date 2015/07/05
* @return QT_NAMESPACE::QString 数据
*/
QString NetWork::getReceiveData()
{
	return m_receiveData;
}

/** 
* @brief 获取进度
* @author LuChenQun
* @date 2015/07/07
* @return int 
*/
int NetWork::getProgress()
{
	int progess = 0;
	if (m_fileSize > 0 && m_taskType == TASK_DOWNLOAD_FILE)
	{
		progess = m_downloadedSize * 100 / m_fileSize;
	}

	return progess;
}

/** 
* @brief 设置文件目录
* @author LuChenQun
* @date 2015/07/07
* @param[in] dirPath 
* @return bool true 设置成功 false 设置失败
*/
bool NetWork::setFileDir(QString dirPath)
{
	bool ret = true;
	m_fileDir = dirPath;

	QDir dir;
	dir.setPath(dirPath);
	if (!dir.exists())
	{
		ret = dir.mkpath(dirPath);
		m_filePath = dirPath + "/" + m_fileName;
	}

	return ret;
}

/** 
* @brief 获取文件目录
* @author LuChenQun
* @date 2015/07/07
* @return QT_NAMESPACE::QString 文件目录
*/
QString NetWork::getFileDir()
{
	return m_fileDir;
}

/** 
* @brief 设置文件路径（包含文件名跟文件目录哦）
* @author LuChenQun
* @date 2015/07/07
* @param[in] filePath 文件路径
* @todo 需要更新目录跟文件名
* @return bool true 设置成功 false 设置失败
*/
bool NetWork::setFilePath(QString filePath)
{
	m_filePath = filePath;
	return true;
}

/** 
* @brief 获取文件路径
* @author LuChenQun
* @date 2015/07/07
* @return QT_NAMESPACE::QString 文件路径
*/
QString NetWork::getFilePath()
{
	return m_filePath;
}

/** 
* @brief 设置文件名
* @author LuChenQun
* @date 2015/07/07
* @param[in] fileName 
* @return bool true 设置成功 false 设置失败
*/
bool NetWork::setFileName(QString fileName)
{
	m_fileName = fileName;
	m_filePath = m_fileDir + "/" + m_fileName;
	return true;
}

/** 
* @brief 获取文件名
* @author LuChenQun
* @date 2015/07/07
* @return QT_NAMESPACE::QString 文件名
*/
QString NetWork::getFileName()
{
	return m_fileName;
}

/** 
* @brief 创建任务
* @author LuChenQun
* @date 2015/07/05
* @param[in] url 网络链接
* @return int 创建任务结果返回
*/
int NetWork::createTask(const QString url)
{
	int code = (NULL == m_curl) ? (CURLE_FAILED_INIT) : (CURLE_OK);

	if (code == CURLE_OK)
	{
		m_url = url;
		QFileInfo fileInfo(m_url);
		switch (m_taskType)
		{
		case NetWork::TASK_HTTP_GET:
			break;
		case NetWork::TASK_HTTP_POST:
			break;
		case NetWork::TASK_DOWNLOAD_FILE:
			setFileDir(NetControl::singleton()->getDefautTaskDir());
			setFileName(fileInfo.fileName());
			break;
		default:
			break;
		}
	}

	return code;
}

/** 
* @brief 开始任务
* @author LuChenQun
* @date 2015/07/03
* @param[in] netWork
* @return int 任务执行结果
*/
int NetWork::startTask(NetWork *netWork)
{
	int code = NETWORK_NO_TASK;
	if (netWork == this)
	{
		switch (m_taskType)
		{
		case NetWork::TASK_HTTP_GET:
			code = startHttpGet();
			break;
		case NetWork::TASK_HTTP_POST:
			code = startHttpPost();
			break;
		case NetWork::TASK_DOWNLOAD_FILE:
			code = startDownloadFile();
			break;
		default:
			break;
		}
	}

	// 任务完成，发送信号
	emitStatus((code == CURLE_OK) ? (NETWORK_FINISH_SUCCESS) : (code));

	return code;
}

/** 
* @brief 开始 HTTP_GET 任务
* @author LuChenQun
* @date 2015/07/03
* @return int 任务执行结果
*/
int NetWork::startHttpGet()
{
	m_receiveData.clear();

    curl_easy_setopt(m_curl, CURLOPT_URL, m_url.toLocal8Bit().data());
    curl_easy_setopt(m_curl, CURLOPT_READFUNCTION, NULL);
	curl_easy_setopt(m_curl, CURLOPT_WRITEFUNCTION, writeData);
    curl_easy_setopt(m_curl, CURLOPT_WRITEDATA, this);

    curl_easy_setopt(m_curl, CURLOPT_NOSIGNAL, 1);
    curl_easy_setopt(m_curl, CURLOPT_CONNECTTIMEOUT, 3);
    curl_easy_setopt(m_curl, CURLOPT_TIMEOUT, 3);

	int code = CURLE_OK;
    code = curl_easy_perform(m_curl);

    return code;
}

/** 
* @brief 开始httpPost任务
* @author LuChenQun
* @date 2015/07/05
* @return int 任务结果
* @see http://finux.iteye.com/blog/715247
*/
int NetWork::startHttpPost()
{
	m_receiveData.clear();

	int index = m_url.indexOf("?");
	QString postUrl = m_url.left(index);
	QString postFields = m_url.mid(index+1);

	// 不能直接char *url = postUrl.toLocal8Bit().data();我也不知道为什么
	// 具体请见：http://www.cnblogs.com/Romi/archive/2012/03/12/2392478.html 以及 http://blog.csdn.net/liuysheng/article/details/6744976
	QByteArray postUrlBa = postUrl.toLocal8Bit();
	QByteArray postFieldsBa = postFields.toLocal8Bit();
	char *url = postUrlBa.data();
	char *urlFields = postFieldsBa.data();

	curl_easy_setopt(m_curl, CURLOPT_URL, url);
	curl_easy_setopt(m_curl, CURLOPT_POSTFIELDS, urlFields);
	curl_easy_setopt(m_curl, CURLOPT_WRITEFUNCTION, writeData);
	curl_easy_setopt(m_curl, CURLOPT_WRITEDATA, this);
	curl_easy_setopt(m_curl, CURLOPT_POST, 1);

	int code = CURLE_OK;
	code = curl_easy_perform(m_curl);

	return code;
}

/** 
* @brief 开始文件下载任务
* @author LuChenQun
* @date 2015/07/05
* @return int 任务结果
*/
int NetWork::startDownloadFile()
{

	int code = CURLE_OK;

	QByteArray urlBa = m_url.toLocal8Bit();
	char *url = urlBa.data();

	// 初始化文件
	code = initFile();
	if (code != NETWORK_INIT_FILE_SUCCESS)
	{
		return code;
	}

	curl_easy_reset(m_curl);
	curl_easy_setopt(m_curl, CURLOPT_URL, url);
	curl_easy_setopt(m_curl, CURLOPT_LOW_SPEED_LIMIT, 10);
	curl_easy_setopt(m_curl, CURLOPT_LOW_SPEED_TIME, 300);
	curl_easy_setopt(m_curl, CURLOPT_HEADER, 1);
	curl_easy_setopt(m_curl, CURLOPT_NOBODY, 1);

	// 获取文件大小
	code = curl_easy_perform(m_curl);
	double fileSize = 0;
	curl_easy_getinfo(m_curl, CURLINFO_CONTENT_LENGTH_DOWNLOAD, &fileSize);
	emitFileSize(fileSize);
	if (getDiskFreeSpace("e:/") <= (m_fileSize))
	{
		code = NETWORK_DISK_NO_SPACE;
		return code;
	}

	curl_easy_setopt(m_curl, CURLOPT_HEADER, 0);
	curl_easy_setopt(m_curl, CURLOPT_NOBODY, 0);
	curl_easy_setopt(m_curl, CURLOPT_WRITEFUNCTION, writeData);
	curl_easy_setopt(m_curl, CURLOPT_WRITEDATA, this);
	curl_easy_setopt(m_curl, CURLOPT_RESUME_FROM_LARGE, m_breakPoint);	// 断点续传
	curl_easy_setopt(m_curl, CURLOPT_XFERINFOFUNCTION, progress);		// 进度
	curl_easy_setopt(m_curl, CURLOPT_XFERINFODATA, this);
	curl_easy_setopt(m_curl, CURLOPT_NOPROGRESS, 0L);
	curl_easy_setopt(m_curl, CURLOPT_NOSIGNAL, 1L);      // 多线程需要注意的
	curl_easy_setopt(m_curl, CURLOPT_FORBID_REUSE, 1);

	code = curl_easy_perform(m_curl);

	m_downloadFile.flush();
	m_downloadFile.close();

	return code;
}

/** 
* @brief 发送状态码
* @author LuChenQun
* @date 2015/07/05
* @param[in] status
* @return void
*/
void NetWork::emitStatus(int status)
{
	m_status = status;
	emit statusChanged(this);
}

/** 
* @brief 发送文件大小
* @author LuChenQun
* @date 2015/07/07
* @param[in] fileSize 文件大小
* @return void 
*/
void NetWork::emitFileSize(double fileSize)
{
	m_fileSize = fileSize;
	emit fileSized(this);
}


/** 
* @brief 初始化需要读写的文件
* @author LuChenQun
* @date 2015/07/07
* @return int 初始化结果
*/
int NetWork::initFile()
{
	int initRet = NETWORK_INIT_FILE_SUCCESS;
	m_downloadFile.setFileName(m_filePath);
	bool openRet = m_downloadFile.open(QIODevice::WriteOnly | QIODevice::Append);
	if (openRet == false)
	{
		initRet = NETWORK_OPEN_FILE_FAIL;
	}

	return initRet;
}

/**
* @brief HTTP_GET 数据回写函数
* @author LuChenQun
* @date 2015/07/03
* @param[in] buffer 数据块
* @param[in] size 数据大小
* @param[in] n 数据块个数
* @param[in] user 用户会写的指针
* @return size_t 返回写入的数据大小
*/
size_t NetWork::writeData(void* buffer, size_t size, size_t n, void *user)
{
	char *data = (char*)buffer;
	size_t len = size * n;

	NetWork* d = (NetWork*)user;
	TaskType taskType = d->getTaskType();

	switch (taskType)
	{
	case NetWork::TASK_HTTP_GET:
		d->m_receiveData.append(data);
		break;
	case NetWork::TASK_HTTP_POST:
		d->m_receiveData.append(data);
		break;
	case NetWork::TASK_DOWNLOAD_FILE:
		d->m_downloadFile.write(data, size*n);
		break;
	default:
		break;
	}

	return len;
}

/** 
* @brief 进度
* @author LuChenQun
* @date 2015/07/07
* @param[in] p 
* @param[in] dltotal 
* @param[in] dlnow 
* @param[in] ultotal 
* @param[in] ulnow 
* @return int 
*/
int NetWork::progress(void *p, curl_off_t dltotal, curl_off_t dlnow, curl_off_t ultotal, curl_off_t ulnow)
{
	NetWork *netWork = (NetWork *)p;
	netWork->m_downloadedSize = dlnow;
	emit netWork->progressed(netWork);
	return 0;
}

/** 
* @brief 调试信息
* @author LuChenQun
* @date 2015/07/06
* @param[in] pcurl
* @param[in] itype
* @param[in] pData
* @param[in] size
* @param[in] *
* @return int
*/
int NetWork::curlDebug(CURL *pcurl, curl_infotype itype, char * pData, size_t size, void *)
{
	if (itype == CURLINFO_TEXT)
	{
		qDebug() << "[TEXT]:" << pData;
	}
	else if (itype == CURLINFO_HEADER_IN)
	{
		qDebug() << "[HEADER_IN]:" << pData;
	}
	else if (itype == CURLINFO_HEADER_OUT)
	{
		qDebug() << "[HEADER_OUT]:" << pData;
	}
	else if (itype == CURLINFO_DATA_IN)
	{
		qDebug() << "[DATA_IN]:" << pData;
	}
	else if (itype == CURLINFO_DATA_OUT)
	{
		qDebug() << "[DATA_OUT]:" << pData;
	}
	return 0;
}

/** 
* @brief 获取指定磁盘的剩余空间
* @author LuChenQun
* @date 2015/07/07
* @param[in] diskPath 磁盘路径，如 C:/
* @return QT_NAMESPACE::qint64 磁盘空间大小，单位为字节
*/
qint64 NetWork::getDiskFreeSpace(QString diskPath)
{
	qint64 freeSpaceSize = 888888888888888888;

	return freeSpaceSize;
}

/** 
* @brief 将QString转换为char字符
* @author LuChenQun
* @date 2015/07/07
* @param[in] str 字符串
* @return char* 字符指针
*/
char* NetWork::QStringToChar(QString str)
{
	QByteArray strBa = str.toLocal8Bit();
	char *pCh = strBa.data();
	return pCh;
}