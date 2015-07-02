#include "NetControl.h"
#include <QDebug>

NetControl* NetControl::m_netControl = nullptr;
NetControl::NetControl(QObject *parent) : QObject(parent)
{
    curl_global_init(CURL_GLOBAL_ALL);
}


NetControl* NetControl::singleton()
{
    // todo 后面要加互斥锁
    if(m_netControl == nullptr)
    {
        m_netControl = new NetControl();
    }

    return m_netControl;
}

NET_HANDLE NetControl::createTaskHttpGet(const QString url)
{
    NetWork *netWork = new NetWork();
    QThread *thread = new QThread();

    netWork->initThread(thread);
    netWork->initHttpGet(url);
    netWork->setTaskType(NetWork::TASK_HTTP_GET);

    connect(this, &NetControl::startHttpGeted, netWork, &NetWork::startHttpGet, Qt::QueuedConnection);


    m_netHandleList.append(netWork);
    return netWork;
}

bool NetControl::startTask(NET_HANDLE handle)
{
    NetWork::TaskType taskType = handle->getTaskType();
    handle->getThread()->start();

    switch (taskType)
    {
    case NetWork::TASK_HTTP_GET:
        emit startHttpGeted(handle);
        break;
    case NetWork::TASK_DOWNLOAD_FILE:
        break;
    default:
        break;
    }


    return true;
}

/**
* @brief
* @author LuChenQun
* @date 2015-7-2 09:47:59
* @param[in] MethodArgName
* @return SymbolType
*/

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
