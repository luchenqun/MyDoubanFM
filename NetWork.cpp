#include "NetWork.h"
#include <QDebug>
#include <QTime>

NetWork::NetWork(QObject *parent) :
    QObject(parent),
    m_thread(nullptr)
{
    m_curl = curl_easy_init();
}

void NetWork::initThread(QThread *thread)
{
    m_thread = thread;
    this->moveToThread(thread);
}

QThread* NetWork::getThread()
{
    return m_thread;
}
void NetWork::setTaskType(TaskType taskType)
{
    m_taskType = taskType;
}

NetWork::TaskType NetWork::getTaskType()
{
    return m_taskType;
}

size_t NetWork::httpGetWriteData(void* buffer, size_t size, size_t n, void *user)
{
    char *data = (char*)buffer;
    NetWork* d = (NetWork*)user;
    qDebug() << data;
    qint64 len = d->m_file.write(data, size * n);
    return len;
}

int NetWork::initHttpGet(QString url)
{
    if(NULL == m_curl)
    {
        return CURLE_FAILED_INIT;
    }

    m_url = url;

    return CURLE_OK;
}

int NetWork::startHttpGet()
{
    QTime time;
    time.start();

    CURLcode code;

    m_file.setFileName("1.txt");
    bool ret = m_file.open(QIODevice::WriteOnly | QIODevice::Append);
    if(ret == false)
    {
        return CURLE_FAILED_INIT;
    }

    curl_easy_setopt(m_curl, CURLOPT_URL, m_url.toLocal8Bit().data());
    curl_easy_setopt(m_curl, CURLOPT_READFUNCTION, NULL);
    curl_easy_setopt(m_curl, CURLOPT_WRITEFUNCTION, httpGetWriteData);
    curl_easy_setopt(m_curl, CURLOPT_WRITEDATA, this);

    curl_easy_setopt(m_curl, CURLOPT_NOSIGNAL, 1);
    curl_easy_setopt(m_curl, CURLOPT_CONNECTTIMEOUT, 3);
    curl_easy_setopt(m_curl, CURLOPT_TIMEOUT, 3);

    code = curl_easy_perform(m_curl);

    qDebug() << "startHttpGet:" << time.elapsed();
    return code;
}
