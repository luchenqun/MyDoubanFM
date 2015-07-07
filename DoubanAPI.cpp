#include "DoubanAPI.h"

DoubanAPI::DoubanAPI() :
	m_loginUrl("https://www.douban.com/j/app/login"),
	m_channelUrl("http://www.douban.com/j/app/radio/channels")
{
	m_nc = NetControl::singleton();
	connect(m_nc, &NetControl::statusChanged, this, &DoubanAPI::statusChange);
}


DoubanAPI::~DoubanAPI()
{
}

/** 
* @brief �û���¼ 
* @author LuChenQun
* @date 2015/07/07
* @param[in] email �û�ע�������
* @param[in] pwd ����
* @param[in] app_name Ĭ��ֵΪradio_desktop_win
* @param[in] version Ĭ��ֵΪ100
* @return int 
*/
int DoubanAPI::userLogin(QString email, QString password, QString app_name, QString version)
{
	QString loginUrl = m_loginUrl + "?email=" + email + "password=" + password + "app_name=" + app_name + "version=" + version;
	m_loginHandle = m_nc->createTask(loginUrl, NetWork::TASK_HTTP_POST);
	return m_nc->startTask(m_loginHandle);
}

/** 
* @brief ��ȡ��̨�б�
* @author LuChenQun
* @date 2015/07/07
* @return int 
*/
int DoubanAPI::getChannelInfo()
{
	m_channelHandle = m_nc->createTask(m_channelUrl, NetWork::TASK_HTTP_GET);
	return m_nc->startTask(m_channelHandle);
}

/** 
* @brief ���緽��״̬�źŲ۸ı�
* @author LuChenQun
* @date 2015/07/07
* @param[in] handle 
* @return void 
*/
void DoubanAPI::statusChange(NET_HANDLE handle)
{
	QString data = m_nc->getReceiveData(handle);
	int status = m_nc->getNetCode(handle);

	if (m_loginHandle == handle)
	{
		emit userLogined(loginRetInfo(data, status));
	}

	if (m_channelHandle == handle)
	{
		emit channelInfoed(channelRetInfo(data, status));
	}
}

/** 
* @brief ͨ��str��ȡ��¼������Ϣ
* @author LuChenQun
* @date 2015/07/07
* @param[in] jsonLoginStr һ��Json���ַ���
* @param[in] netStatus ������Ϣ
* @return DoubanLoginRetInfo ��¼������Ϣ
*/
DoubanLogRetInfo DoubanAPI::loginRetInfo(QString jsonLoginStr, int netStatus)
{
	DoubanLogRetInfo loginInfo;
	loginInfo.code = DOUBAN_LOGIN;

	QJsonParseError jsonError;
	QJsonDocument jsonDoc = QJsonDocument::fromJson(jsonLoginStr.toLocal8Bit(), &jsonError);

	// �������
	do 
	{
		if (netStatus != NetWork::NETWORK_FINISH_SUCCESS){ loginInfo.code = netStatus; break; }
		if (jsonError.error != QJsonParseError::NoError){ loginInfo.code = DOUBAN_JSON_ERROR; break; }
		if (!jsonDoc.isObject()){ loginInfo.code = DOUBAN_OBJ_ERROR; break; }
	} while (false);


	if (DOUBAN_LOGIN == loginInfo.code)
	{
		QJsonObject jsonObj = jsonDoc.object();
		loginInfo.userId = jsonObj.take("user_id").toString();
		loginInfo.err = jsonObj.take("err").toString();
		loginInfo.token = jsonObj.take("token").toString();
		loginInfo.expire = jsonObj.take("expire").toString();
		loginInfo.r = jsonObj.take("r").toInt();
		loginInfo.userName = jsonObj.take("user_name").toString();
		loginInfo.email = jsonObj.take("email").toString();
	}

	return loginInfo;
}

/** 
* @brief ͨ��str��ȡ��¼������Ϣ
* @author LuChenQun
* @date 2015/07/07
* @param[in] jsonChannelStr 
* @param[in] netStatus 
* @return ChannelRetInfo 
*/
ChannelsInfo DoubanAPI::channelRetInfo(QString jsonChannelStr, int netStatus)
{
	ChannelsInfo channelsInfo;
	channelsInfo.code = DOUBAN_GET_CHANNEL_SUCCESS;

	QJsonParseError jsonError;
	QJsonDocument jsonDoc = QJsonDocument::fromJson(jsonChannelStr.toLocal8Bit(), &jsonError);
	QJsonObject obj = jsonDoc.object();
	QJsonValue channelsValue = obj.take("channels");

	// ���ִ�����
	do 
	{
		if (netStatus != NetWork::NETWORK_FINISH_SUCCESS){ channelsInfo.code = netStatus; break; }
		if (jsonError.error != QJsonParseError::NoError){ channelsInfo.code = DOUBAN_JSON_ERROR; break; }
		if (!jsonDoc.isObject()){ channelsInfo.code = DOUBAN_OBJ_ERROR; break; }
		if (!channelsValue.isArray()){ channelsInfo.code = DOUBAN_ARRARY_ERROR; break; }
	} while (false);

	// jsonת��
	if (DOUBAN_GET_CHANNEL_SUCCESS == channelsInfo.code)
	{
		QJsonArray array = channelsValue.toArray();
		for each (QJsonValue value in array)
		{
			Channel channel;

			QJsonObject netChannel = value.toObject();
			channel.nameEn = netChannel.take("name_en").toString();
			channel.seqId = netChannel.take("seq_id").toInt();
			channel.abbrEn = netChannel.take("abbr_en").toString();
			channel.name = netChannel.take("name").toString();
			channel.channelId = netChannel.take("channel_id").toInt();

			channelsInfo.ChannelList.append(channel);
		}
	}

	return channelsInfo;
}
