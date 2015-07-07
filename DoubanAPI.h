#pragma once
#include <QObject>
#include <QJsonParseError>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include "NetControl.h"

class DoubanLogRetInfo
{
public:
	QString userId;		/**< */
	QString err;		/**< */
	QString token;		/**< */
	QString expire;		/**< */
	int r;				/**< */
	QString userName;	/**< */
	QString email;		/**< */
	int code;		/**< */
	DoubanLogRetInfo() : userId(""), err(""), token(""), expire(""), r(-1), userName(""), email(""), code(-1){};
};

class Channel
{
public:
	QString nameEn;
	int seqId;
	QString abbrEn;
	QString name;
	int channelId;
	Channel() : nameEn(""), seqId(-1), abbrEn(""), name(""), channelId(-1){};
};

class ChannelsInfo
{
public:
	QList<Channel> ChannelList;
	int code;
	ChannelsInfo() : ChannelList(QList<Channel>()), code(-1){};
};

class DoubanAPI : public QObject
{
	Q_OBJECT
public:
	/** @brief DoubanAPI������״̬������ */
	typedef enum
	{
		DOUBAN_BEGIN = NetWork::NETWORK_LAST,	/**< Ϊ������״̬�룬 ʹ��NetWork�����һ��״̬����Ϊ��ʼ���벻Ҫʹ�� */

		DOUBAN_LOGIN,							/**< ��¼ */
		DOUBAN_LOGOUT,							/**< �ǳ� */
		DOUBAN_GET_CHANNEL_SUCCESS,				/**< ��ȡ��̨�ɹ� */
		DOUBAN_JSON_ERROR,						/**< json���� */
		DOUBAN_OBJ_ERROR,						/**< jsonObj���� */
		DOUBAN_ARRARY_ERROR,					/**< jsonArray���� */

		DOUBAN_LAST,							/**< ����ǣ��벻Ҫʹ���� */
	}DoubanCode;

	DoubanAPI();
	~DoubanAPI();
	int userLogin(QString email, QString password, QString app_name = "radio_desktop_win", QString version = "100");
	int getChannelInfo();
private:
	DoubanLogRetInfo loginRetInfo(QString jsonLoginStr, int netStatus);
	ChannelsInfo channelRetInfo(QString jsonChannelStr, int netStatus);
signals:
	void userLogined(DoubanLogRetInfo loginInfo);
	void channelInfoed(ChannelsInfo channelInfo);
private slots:
	void statusChange(NET_HANDLE handle);
private:
	const QString m_loginUrl;
	const QString m_channelUrl;
	static DoubanLogRetInfo m_logifo;
	NetControl *m_nc;
	NET_HANDLE m_loginHandle;
	NET_HANDLE m_channelHandle;
};

