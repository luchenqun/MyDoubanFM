#include "NetWork.h"
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
	m_status(NETWORK_OK)
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
* @brief 创建任务
* @author LuChenQun
* @date 2015/07/05
* @param[in] url 网络链接
* @return int 创建任务结果返回
*/
int NetWork::createTask(const QString url)
{
	CURLcode code = CURLE_OK;

	if (NULL == m_curl)
	{
		code = CURLE_FAILED_INIT;
	}
	else
	{
		m_url = url;
		code = CURLE_OK;
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
	int code = CURLE_OK;
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

	emitStatus((code == CURLE_OK) ? (NETWORK_FINISH_SUCCESS) : (code));

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

	emitStatus((code == CURLE_OK) ? (NETWORK_FINISH_SUCCESS) : (code));

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
	//curl_easy_setopt(m_curl, CURLOPT_URL, m_url.toLocal8Bit().data());
	//curl_easy_setopt(m_curl, CURLOPT_READFUNCTION, NULL);
	//curl_easy_setopt(m_curl, CURLOPT_WRITEFUNCTION, writeData);
	//curl_easy_setopt(m_curl, CURLOPT_WRITEDATA, this);

	//curl_easy_setopt(m_curl, CURLOPT_NOSIGNAL, 1);
	//curl_easy_setopt(m_curl, CURLOPT_CONNECTTIMEOUT, 3);
	//curl_easy_setopt(m_curl, CURLOPT_TIMEOUT, 3);

	int code = CURLE_OK;
	//code = curl_easy_perform(m_curl);

	//emitStatus((code == CURLE_OK) ? (NETWORK_FINISH_SUCCESS) : (code));

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
		break;
	default:
		break;
	}

	return len;
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
int NetWork::CurlDebug(CURL *pcurl, curl_infotype itype, char * pData, size_t size, void *)
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