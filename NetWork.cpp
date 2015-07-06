#include "NetWork.h"
#include <QDebug>
#include <QTime>
#include <QTextCodec>

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
	m_runMode(ASYNCHRONOUS),
	m_status(NETWORK_OK)
{
    m_curl = curl_easy_init();
}

/** 
* @brief ��������
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
* @brief ��ȡ��ǰ��״̬��
* @author LuChenQun
* @date 2015/07/05
* @return int
*/
int NetWork::getStatus()
{
	return m_status;
}

/** 
* @brief ��ȡ���յ������ݣ����磺ʹ��http get�����ݣ�http post���ص����ݵȣ�
* @author LuChenQun
* @date 2015/07/05
* @return QT_NAMESPACE::QString ����
*/
QString NetWork::getReceiveData()
{
	return m_receiveData;
}

/** 
* @brief ��������
* @author LuChenQun
* @date 2015/07/05
* @param[in] url ��������
* @return int ��������������
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
* @brief ��ʼ����
* @author LuChenQun
* @date 2015/07/03
* @param[in] netWork
* @return int ����ִ�н��
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
* @brief ��ʼ HTTP_GET ����
* @author LuChenQun
* @date 2015/07/03
* @return int ����ִ�н��
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

/*
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <curl/curl.h>
#include <QDebug>

#define POSTURL "https://www.douban.com/j/app/login"
#define POSTFIELDS "app_name=radio_desktop_win&version=100&email=luchenqun@qq.com&password=fendoubuxi596320"
#define FILENAME "curlposttest.log"

size_t write_data(void *buffer, size_t size, size_t nmemb, void *userp);

int main(int argc, char *argv[]) {
CURL *curl;
CURLcode res;
FILE *fptr;
struct curl_slist *http_header = NULL;

if ((fptr = fopen(FILENAME, "w")) == NULL) {
fprintf(stderr, "fopen file error: %s\n", FILENAME);
exit(1);
}

curl = curl_easy_init();
curl_easy_setopt(curl, CURLOPT_URL, POSTURL);
curl_easy_setopt(curl, CURLOPT_POSTFIELDS, POSTFIELDS);
curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);
curl_easy_setopt(curl, CURLOPT_WRITEDATA, fptr);
curl_easy_setopt(curl, CURLOPT_POST, 1);
curl_easy_setopt(curl, CURLOPT_VERBOSE, 1);
curl_easy_setopt(curl, CURLOPT_HEADER, 1);
curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1);
curl_easy_setopt(curl, CURLOPT_COOKIEFILE, "/Users/zhu/CProjects/curlposttest.cookie");
res = curl_easy_perform(curl);
curl_easy_cleanup(curl);
}

size_t write_data(void *buffer, size_t size, size_t nmemb, void *userp) {
char *p = (char *)buffer;
qDebug() << p;
FILE *fptr = (FILE*)userp;
fwrite(buffer, size, nmemb, fptr);
return size * nmemb;
}
*/

/** 
* @brief ��ʼhttpPost����
* @author LuChenQun
* @date 2015/07/05
* @return int ������
*/
int NetWork::startHttpPost()
{
	m_receiveData.clear();

	//int index = m_url.indexOf("?");
	//QString postUrl = m_url.left(index);
	//QString postFields = m_url.mid(index+1);

	char *postUrl = "https://www.douban.com/j/app/login";
	char *postFields = "app_name=radio_desktop_win&version=100&email=luchenqun@qq.com&password=fendoubuxi596320";

	// ����������᷵��403����
#if 1
	QString useragent("Mozilla/5.0 (Windows NT 6.1; WOW64; rv:13.0) Gecko/20100101 Firefox/13.0.1");
	curl_easy_setopt(m_curl, CURLOPT_USERAGENT, useragent.toLocal8Bit().data());
	curl_easy_setopt(m_curl, CURLOPT_URL, postUrl);
	curl_easy_setopt(m_curl, CURLOPT_POSTFIELDS, postFields);
	curl_easy_setopt(m_curl, CURLOPT_WRITEFUNCTION, writeData);
	curl_easy_setopt(m_curl, CURLOPT_WRITEDATA, this);
	curl_easy_setopt(m_curl, CURLOPT_POST, 1);
#else
	curl_easy_setopt(m_curl, CURLOPT_URL, postUrl);
	curl_easy_setopt(m_curl, CURLOPT_POSTFIELDS, postFields);
	curl_easy_setopt(m_curl, CURLOPT_WRITEFUNCTION, writeData);
	curl_easy_setopt(m_curl, CURLOPT_WRITEDATA, this);
	curl_easy_setopt(m_curl, CURLOPT_POST, 1);
#endif
	int code = CURLE_OK;
	code = curl_easy_perform(m_curl);

	emitStatus((code == CURLE_OK) ? (NETWORK_FINISH_SUCCESS) : (code));

	return code;
}

/** 
* @brief ��ʼ�ļ���������
* @author LuChenQun
* @date 2015/07/05
* @return int ������
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
* @brief ����״̬��
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
* @brief HTTP_GET ���ݻ�д����
* @author LuChenQun
* @date 2015/07/03
* @param[in] buffer ���ݿ�
* @param[in] size ���ݴ�С
* @param[in] n ���ݿ����
* @param[in] user �û���д��ָ��
* @return size_t ����д������ݴ�С
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