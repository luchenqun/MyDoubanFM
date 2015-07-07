#include "NetWork.h"
#include "NetControl.h"
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
	m_status(NETWORK_OK),
	m_fileSize(0),
	m_breakPoint(0)
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
* @brief ��ȡ����
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
* @brief �����ļ�Ŀ¼
* @author LuChenQun
* @date 2015/07/07
* @param[in] dirPath 
* @return bool true ���óɹ� false ����ʧ��
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
* @brief ��ȡ�ļ�Ŀ¼
* @author LuChenQun
* @date 2015/07/07
* @return QT_NAMESPACE::QString �ļ�Ŀ¼
*/
QString NetWork::getFileDir()
{
	return m_fileDir;
}

/** 
* @brief �����ļ�·���������ļ������ļ�Ŀ¼Ŷ��
* @author LuChenQun
* @date 2015/07/07
* @param[in] filePath �ļ�·��
* @todo ��Ҫ����Ŀ¼���ļ���
* @return bool true ���óɹ� false ����ʧ��
*/
bool NetWork::setFilePath(QString filePath)
{
	m_filePath = filePath;
	return true;
}

/** 
* @brief ��ȡ�ļ�·��
* @author LuChenQun
* @date 2015/07/07
* @return QT_NAMESPACE::QString �ļ�·��
*/
QString NetWork::getFilePath()
{
	return m_filePath;
}

/** 
* @brief �����ļ���
* @author LuChenQun
* @date 2015/07/07
* @param[in] fileName 
* @return bool true ���óɹ� false ����ʧ��
*/
bool NetWork::setFileName(QString fileName)
{
	m_fileName = fileName;
	m_filePath = m_fileDir + "/" + m_fileName;
	return true;
}

/** 
* @brief ��ȡ�ļ���
* @author LuChenQun
* @date 2015/07/07
* @return QT_NAMESPACE::QString �ļ���
*/
QString NetWork::getFileName()
{
	return m_fileName;
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
* @brief ��ʼ����
* @author LuChenQun
* @date 2015/07/03
* @param[in] netWork
* @return int ����ִ�н��
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

	// ������ɣ������ź�
	emitStatus((code == CURLE_OK) ? (NETWORK_FINISH_SUCCESS) : (code));

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

    return code;
}

/** 
* @brief ��ʼhttpPost����
* @author LuChenQun
* @date 2015/07/05
* @return int ������
* @see http://finux.iteye.com/blog/715247
*/
int NetWork::startHttpPost()
{
	m_receiveData.clear();

	int index = m_url.indexOf("?");
	QString postUrl = m_url.left(index);
	QString postFields = m_url.mid(index+1);

	// ����ֱ��char *url = postUrl.toLocal8Bit().data();��Ҳ��֪��Ϊʲô
	// ���������http://www.cnblogs.com/Romi/archive/2012/03/12/2392478.html �Լ� http://blog.csdn.net/liuysheng/article/details/6744976
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
* @brief ��ʼ�ļ���������
* @author LuChenQun
* @date 2015/07/05
* @return int ������
*/
int NetWork::startDownloadFile()
{

	int code = CURLE_OK;

	QByteArray urlBa = m_url.toLocal8Bit();
	char *url = urlBa.data();

	// ��ʼ���ļ�
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

	// ��ȡ�ļ���С
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
	curl_easy_setopt(m_curl, CURLOPT_RESUME_FROM_LARGE, m_breakPoint);	// �ϵ�����
	curl_easy_setopt(m_curl, CURLOPT_XFERINFOFUNCTION, progress);		// ����
	curl_easy_setopt(m_curl, CURLOPT_XFERINFODATA, this);
	curl_easy_setopt(m_curl, CURLOPT_NOPROGRESS, 0L);
	curl_easy_setopt(m_curl, CURLOPT_NOSIGNAL, 1L);      // ���߳���Ҫע���
	curl_easy_setopt(m_curl, CURLOPT_FORBID_REUSE, 1);

	code = curl_easy_perform(m_curl);

	m_downloadFile.flush();
	m_downloadFile.close();

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
* @brief �����ļ���С
* @author LuChenQun
* @date 2015/07/07
* @param[in] fileSize �ļ���С
* @return void 
*/
void NetWork::emitFileSize(double fileSize)
{
	m_fileSize = fileSize;
	emit fileSized(this);
}


/** 
* @brief ��ʼ����Ҫ��д���ļ�
* @author LuChenQun
* @date 2015/07/07
* @return int ��ʼ�����
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
		d->m_downloadFile.write(data, size*n);
		break;
	default:
		break;
	}

	return len;
}

/** 
* @brief ����
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
* @brief ������Ϣ
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
* @brief ��ȡָ�����̵�ʣ��ռ�
* @author LuChenQun
* @date 2015/07/07
* @param[in] diskPath ����·������ C:/
* @return QT_NAMESPACE::qint64 ���̿ռ��С����λΪ�ֽ�
*/
qint64 NetWork::getDiskFreeSpace(QString diskPath)
{
	qint64 freeSpaceSize = 888888888888888888;

	return freeSpaceSize;
}

/** 
* @brief ��QStringת��Ϊchar�ַ�
* @author LuChenQun
* @date 2015/07/07
* @param[in] str �ַ���
* @return char* �ַ�ָ��
*/
char* NetWork::QStringToChar(QString str)
{
	QByteArray strBa = str.toLocal8Bit();
	char *pCh = strBa.data();
	return pCh;
}