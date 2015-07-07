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
NetControl::NetControl(QObject *parent) : QObject(parent),
	m_defaultFolderName("Download")
{
    curl_global_init(CURL_GLOBAL_ALL);
	setDefautTaskDir(QApplication::applicationDirPath() + "/" + m_defaultFolderName);
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
	NetWork *netWork = new NetWork();
	netWork->setTaskType(taskType);

	int createTaskRet = netWork->createTask(url);
	if (CURLE_OK != createTaskRet)
	{
		delete netWork;
		netWork = nullptr;
	}

	// 初始化一些东西，
	if (netWork != nullptr)
	{
		connect(this, &NetControl::startTasked, netWork, &NetWork::startTask, Qt::QueuedConnection);
		connect(netWork, &NetWork::progressed, this, &NetControl::progressed, Qt::QueuedConnection);
		connect(netWork, &NetWork::statusChanged, this, &NetControl::statusChanged, Qt::QueuedConnection);
		connect(netWork, &NetWork::fileSized, this, &NetControl::fileSized, Qt::QueuedConnection);
		m_netHandleList.append(netWork);
	}

	return netWork;
}

/** 
* @brief 开始任务
* @author LuChenQun
* @date 2015/07/03
* @param[in] handle 任务句柄
* @return NetWork::NetCode 网络任务状态码
*/
NetWork::NetWokeCode NetControl::startTask(NET_HANDLE handle)
{
	if (handle == nullptr)
	{
		return NetWork::NETWORK_HANDLE_NULL_PTR;
	}

	// 是否在新线程中运行(即异步执行)
	if (handle->getRunMode() == NetWork::ASYNCHRONOUS)
	{
		handle->moveToNewThread();
		handle->getThread()->start();
	}

	switch (handle->getRunMode())
	{
	case NetWork::ASYNCHRONOUS:
		emit startTasked(handle);
		break;
	case NetWork::SYNCHRONIZATION:
		handle->startTask(handle);
		break;
	default:
		emit startTasked(handle);
		break;
	}

	return NetWork::NETWORK_OK;
}


/** 
* @brief 暂停任务
* @author LuChenQun
* @date 2015/07/05
* @param[in] handle 任务句柄
* @return NetWork::NetCode 网络任务状态码
*/
NetWork::NetWokeCode NetControl::pauseTask(NET_HANDLE handle)
{
	if (handle == nullptr)
	{
		return NetWork::NETWORK_HANDLE_NULL_PTR;
	}

	return NetWork::NETWORK_OK;
}

/**
* @brief 恢复任务
* @author LuChenQun
* @date 2015/07/05
* @param[in] handle 任务句柄
* @return NetWork::NetCode 网络任务状态码
*/
NetWork::NetWokeCode NetControl::resumeTask(NET_HANDLE handle)
{
	if (handle == nullptr)
	{
		return NetWork::NETWORK_HANDLE_NULL_PTR;
	}

	return NetWork::NETWORK_OK;
}

/** 
* @brief 检查网络句柄是否可用
* @author LuChenQun
* @date 2015/07/07
* @param[in] handle 网络句柄
* @return bool true 可用 false 不可用
*/
bool NetControl::netHandleCanUse(NET_HANDLE handle)
{
	bool ret = true;
	do 
	{
		if (handle == nullptr){ ret = false; break; }

		if (!m_netHandleList.contains(handle)){ ret = false; break; }
	} while (false);
	
	return ret;
}

/**
* @brief 删除任务
* @author LuChenQun
* @date 2015/07/05
* @param[in] handle 任务句柄
* @return NetWork::NetCode 网络任务状态码
*/
NetWork::NetWokeCode NetControl::deleteTask(NET_HANDLE handle)
{
	if (handle == nullptr)
	{
		return NetWork::NETWORK_HANDLE_NULL_PTR;
	}

	return NetWork::NETWORK_OK;
}

/** 
* @brief 设置默认的任务目录
* @author LuChenQun
* @date 2015/07/07
* @param[in] dirPath 目录路径
* @return void 
*/
void NetControl::setDefautTaskDir(QString dirPath)
{
	m_taskDir = dirPath;
	QDir dir;
	dir.setPath(m_taskDir);
	if (!dir.exists())
	{
		dir.mkpath(m_taskDir);
	}
}

/** 
* @brief 获取默认的任务目录
* @author LuChenQun
* @date 2015/07/07
* @return QT_NAMESPACE::QString 
*/
QString NetControl::getDefautTaskDir()
{
	return m_taskDir;
}

/** 
* @brief 设置一个网络任务的目录路径
* @author LuChenQun
* @date 2015/07/07
* @param[in] handle 网络句柄
* @param[in] dirPath 目录路径
* @return bool true 设置成功 false 设置失败
*/
bool NetControl::setTaskDir(NET_HANDLE handle, QString dirPath)
{
	bool ret = false;

	if (netHandleCanUse(handle))
	{
		ret = handle->setFileDir(dirPath);
	}

	return ret;
}

/** 
* @brief 获取一个网络任务的目录路径
* @author LuChenQun
* @date 2015/07/07
* @param[in] handle 网络句柄
* @return QT_NAMESPACE::QString 目录路径
*/
QString NetControl::getTaskDir(NET_HANDLE handle)
{
	return (netHandleCanUse(handle)) ? (handle->getFileDir()) : ("");
}

/** 
* @brief 获取状态代码
* @author LuChenQun
* @date 2015/07/05
* @param[in] handle 网络句柄
* @return int 状态码
*/
int NetControl::getNetCode(NET_HANDLE handle)
{
	return (netHandleCanUse(handle)) ? (handle->getStatus()) : (NetWork::NETWORK_HANDLE_NULL_PTR);
}

/** 
* @brief 获取接收到的数据
* @author LuChenQun
* @date 2015/07/05
* @param[in] handle
* @return QT_NAMESPACE::QString
*/
QString NetControl::getReceiveData(NET_HANDLE handle)
{
	return (netHandleCanUse(handle)) ? (handle->getReceiveData()) : ("");
}


/** 
* @brief 获取进度
* @author LuChenQun
* @date 2015/07/07
* @param[in] handle 网络句柄
* @return int 进度
*/
int NetControl::getProgress(NET_HANDLE handle)
{
	return (handle == nullptr) ? (0) : (handle->getProgress());
}