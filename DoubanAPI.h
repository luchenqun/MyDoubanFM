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
	/** @brief DoubanAPI类所有状态返回码 */
	typedef enum
	{
		DOUBAN_BEGIN = NetWork::NETWORK_LAST,	/**< 为了区分状态码， 使用NetWork的最后一个状态码作为开始，请不要使用 */

		DOUBAN_LOGIN,							/**< 登录 */
		DOUBAN_LOGOUT,							/**< 登出 */
		DOUBAN_GET_CHANNEL_SUCCESS,				/**< 获取电台成功 */
		DOUBAN_JSON_ERROR,						/**< json有误 */
		DOUBAN_OBJ_ERROR,						/**< jsonObj有误 */
		DOUBAN_ARRARY_ERROR,					/**< jsonArray有误 */

		DOUBAN_LAST,							/**< 最后标记，请不要使用用 */
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

