#include "NetControl.h"
#include <QDebug>

/*< @brief 初始化单例模式 */
NetControl* NetControl::m_netControl = nullptr;

/** 
* @brief 构造函数，为了禁止构造对象，特意声明为private
* @author LuChenQun
* @date 2015/07/02
* @param[in] parent
* @return 无
*/
NetControl::NetControl(QObject *parent) : QObject(parent)
{
    curl_global_init(CURL_GLOBAL_ALL);
}


/** 
* @brief 创建单例模式，所有的网络管理类都由此控制
* @author LuChenQun
* @date 2015/07/02
* @return NetControl* 网络控制指针
* @todo 后面要加互斥锁
*/
NetControl* NetControl::singleton()
{
    if(m_netControl == nullptr)
    {
        m_netControl = new NetControl();
    }

    return m_netControl;
}

/** 
* @brief 根据类型创建网络句柄
* @author LuChenQun
* @date 2015/07/02
* @param[in] url
* @param[in] taskType
* @return NET_HANDLE
*/
NET_HANDLE NetControl::createTask(const QString url, NetWork::TaskType taskType)
{
	NET_HANDLE handle = nullptr;
	switch (taskType)
	{
	case NetWork::TASK_HTTP_GET:
		handle = createTaskHttpGet(url);
		break;
	case NetWork::TASK_DOWNLOAD_FILE:
		break;
	default:
		break;
	}

	return handle;
}

/** 
* @brief 开始任务
* @author LuChenQun
* @date 2015/07/03
* @param[in] handle 任务句柄
* @return bool true 成功开始任务 false 任务开始失败
*/
bool NetControl::startTask(NET_HANDLE handle)
{
	// 是否在新线程中运行(即异步执行)
	if (handle->getRunMode() == NetWork::ASYNCHRONOUS)
	{
		handle->moveToNewThread();
		handle->getThread()->start();
	}

	NetWork::TaskType taskType = handle->getTaskType();
    switch (taskType)
    {
    case NetWork::TASK_HTTP_GET:
		if (handle->getRunMode() == NetWork::ASYNCHRONOUS)
		{ 
			emit startHttpGeted(handle);
		}
		else
		{
			handle->startHttpGet();
		}
        break;
    case NetWork::TASK_DOWNLOAD_FILE:
        break;
    default:
        break;
    }

    return true;
}


bool NetControl::pauseTask(NET_HANDLE handle)
{
    return true;
}

bool NetControl::resumeTask(NET_HANDLE handle)
{
    return true;
}

bool NetControl::deleteTask(NET_HANDLE handle)
{
    return true;
}

/**
* @brief 创建HttpGet的网络句柄
* @author LuChenQun
* @date 2015/07/02
* @param[in] url 链接
* @return NET_HANDLE 网络句柄 nullptr 初始化失败
*/
NET_HANDLE NetControl::createTaskHttpGet(const QString url)
{
	NetWork *netWork = new NetWork();

	int initRet = CURLE_OK;
	initRet = netWork->initHttpGet(url);

	if (CURLE_OK == initRet)
	{
		netWork->setTaskType(NetWork::TASK_HTTP_GET);
		connect(this, &NetControl::startHttpGeted, netWork, &NetWork::startHttpGet, Qt::QueuedConnection);
		m_netHandleList.append(netWork);
	}
	else
	{
		delete netWork;
		netWork = nullptr;
	}

	return netWork;
}