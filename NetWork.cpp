#include "NetWork.h"
#include <QDebug>
#include <QTime>

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
	m_runMode(ASYNCHRONOUS)
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
* @brief 初始化HTTP_GET任务
* @author LuChenQun
* @date 2015/07/02
* @param[in] url 网络连接
* @return int 初始化结果 CURLE_OK 初始化成功 其他返回值初始化失败
*/
int NetWork::initHttpGet(QString url)
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
	CURLcode code = CURLE_OK;
	if (netWork == this)
	{
		switch (m_taskType)
		{
		case NetWork::TASK_HTTP_GET:
			code = (CURLcode)startHttpGet();
			break;
		case NetWork::TASK_HTTP_POST:
			break;
		case NetWork::TASK_DOWNLOAD_FILE:
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
    curl_easy_setopt(m_curl, CURLOPT_URL, m_url.toLocal8Bit().data());
    curl_easy_setopt(m_curl, CURLOPT_READFUNCTION, NULL);
	curl_easy_setopt(m_curl, CURLOPT_WRITEFUNCTION, writeData);
    curl_easy_setopt(m_curl, CURLOPT_WRITEDATA, this);

    curl_easy_setopt(m_curl, CURLOPT_NOSIGNAL, 1);
    curl_easy_setopt(m_curl, CURLOPT_CONNECTTIMEOUT, 3);
    curl_easy_setopt(m_curl, CURLOPT_TIMEOUT, 3);

	CURLcode code = CURLE_OK;
    code = curl_easy_perform(m_curl);

    return code;
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
		break;
	case NetWork::TASK_DOWNLOAD_FILE:
		break;
	default:
		break;
	}

	return len;
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
size_t NetWork::httpGetWriteData(void* buffer, size_t size, size_t n, void *user)
{
	char *data = (char*)buffer;
	NetWork* d = (NetWork*)user;
	qDebug() << data;

	return size * n;
}
