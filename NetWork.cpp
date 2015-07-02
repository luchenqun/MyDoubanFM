#include "NetWork.h"
#include <QDebug>
#include <QTime>

/** 
* @brief ���캯��
* @author LuChenQun
* @date 2015/07/02
* @param[in] parent Ŀָ��
* @return ��
*/
NetWork::NetWork(QObject *parent) :
    QObject(parent),
    m_thread(nullptr),
	m_runMode(ASYNCHRONOUS)
{
    m_curl = curl_easy_init();
}

/** 
* @brief �������Ƶ����߳�
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
* @brief ��ȡ�����������̵߳�ָ��
* @author LuChenQun
* @date 2015/07/02
* @return QThread* �߳�ָ��
*/
QThread* NetWork::getThread()
{
    return m_thread;
}


/** 
* @brief ����������������
* @author LuChenQun
* @date 2015/07/02
* @param[in] taskType ������������
* @return void
*/
void NetWork::setTaskType(TaskType taskType)
{
    m_taskType = taskType;
}

/** 
* @brief ��ȡ������������
* @author LuChenQun
* @date 2015/07/02
* @return NetWork::TaskType ������������
*/
NetWork::TaskType NetWork::getTaskType()
{
    return m_taskType;
}

/** 
* @brief ��������ģʽ���첽(ͬ��)
* @author LuChenQun
* @date 2015/07/03
* @param[in] runMode ����ģʽ
* @return void
*/
void NetWork::setRunMode(RunMode runMode)
{
	m_runMode = runMode;
}

/** 
* @brief ��ȡ����ģʽ
* @author LuChenQun
* @date 2015/07/03
* @return NetWork::RunMode ����ģʽ���첽(ͬ��)
*/
NetWork::RunMode NetWork::getRunMode()
{
	return m_runMode;
}

/** 
* @brief ��ʼ��HTTP_GET����
* @author LuChenQun
* @date 2015/07/02
* @param[in] url ��������
* @return int ��ʼ����� CURLE_OK ��ʼ���ɹ� ��������ֵ��ʼ��ʧ��
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
* @brief ��ʼ HTTP_GET ����
* @author LuChenQun
* @date 2015/07/03
* @return int ����ִ�н��
*/
int NetWork::startHttpGet()
{
    curl_easy_setopt(m_curl, CURLOPT_URL, m_url.toLocal8Bit().data());
    curl_easy_setopt(m_curl, CURLOPT_READFUNCTION, NULL);
    curl_easy_setopt(m_curl, CURLOPT_WRITEFUNCTION, httpGetWriteData);
    curl_easy_setopt(m_curl, CURLOPT_WRITEDATA, this);

    curl_easy_setopt(m_curl, CURLOPT_NOSIGNAL, 1);
    curl_easy_setopt(m_curl, CURLOPT_CONNECTTIMEOUT, 3);
    curl_easy_setopt(m_curl, CURLOPT_TIMEOUT, 3);

	CURLcode code = CURLE_OK;
    code = curl_easy_perform(m_curl);

    return code;
}

/** 
* @brief HTTP_GET ���ݻ�д����
* @author LuChenQun
* @date 2015/07/03
* @param[in] buffer ���ݿ�
* @param[in] size ���ݴ�С
* @param[in] n ���ݿ����
* @param[in] user �û���д��ָ��
* @return size_t ����д������ݴ�С
*/
size_t NetWork::httpGetWriteData(void* buffer, size_t size, size_t n, void *user)
{
	char *data = (char*)buffer;
	NetWork* d = (NetWork*)user;
	qDebug() << data;

	return size * n;
}