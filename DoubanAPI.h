#pragma once
#include <QObject>
#include <QJsonParseError>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include "NetControl.h"

/**< 豆瓣登录返回信息*/
class DoubanLogRetInfo
{
public:
	QString userId;		/**< 用户Id */
	QString err;		/**< 如果登录返回码为1，error会有出错信息 */
	QString token;		/**< 登录令牌 */
	QString expire;		/**< token过期时间msec */
	int r;				/**< 登录返回码 1出错 */
	QString userName;	/**< 用户名 */
	QString email;		/**< 用户账号 */
	int code;			/**< 返回码 */
	DoubanLogRetInfo() : userId(""), err(""), token(""), expire(""), r(-1), userName(""), email(""), code(-1){};
};

/**< 豆瓣频道信息*/
class Channel
{
public:
	QString nameEn;	/**< 频道英文说明 */
	int seqId;		/**< 未知 */
	QString abbrEn;	/**< 频道专辑英文说明 */
	QString name;	/**< 频道 */
	int channelId;	/**< 频道Id */
	Channel() : nameEn(""), seqId(-1), abbrEn(""), name(""), channelId(-1){};
};

/**< 豆瓣频道列表返回信息*/
class ChannelsInfo
{
public:
	QList<Channel> channelList;	/**< 频道列表 */
	int code;					/**< 返回码 */
	ChannelsInfo() : channelList(QList<Channel>()), code(-1){};
};

/**< 豆瓣歌曲信息*/
class Song
{
public:
	QString album;		/**< 专辑跳转地址 */
	QString picture;	/**< 专辑图片地址 */
	int ssid;			/**< 未知 */
	QString artist;		/**< 艺术家 */
	QString url;		/**< 歌曲的URL */
	QString company;	/**< 唱片公司 */
	QString title;		/**< 歌曲名 */
	QString ratingAvg;	/**< 平均分数 */
	int length;			/**< 播放时长 */
	QString subtype;	/**< 子类型（有些广告的字类型会是T） */
	QString publicTime;	/**< 出版年份 */
	QString sid;		/**< 歌曲id */
	QString aid;		/**< 专辑id */
	int kbps;			/**< 码率 */
	QString albumtitle;	/**< 专辑名 */
	int like;			/**< 是否已喜欢，0为false，1为true */
	Song() : album(""), picture(""), ssid(0), artist(""), url(""), company(""), title(""), ratingAvg(""), 
		length(0), subtype(""), publicTime(""), sid(""), aid(""), kbps(0), albumtitle(""), like(0){};
};

/**< 豆瓣歌曲列表返回信息*/
class SongInfo
{
public:
	int versionMax;			/**< 固定值100 */
	int r;					/**< 错误返回码：0表示请求正确，1则表示出错 */
	QString err;			/**< 若上面的r=1，则err有相应的错误信息 */
	QList<Song> songList;	/**< 歌曲信息列表 */
	int code;				/**< 返回码 */
	SongInfo() : versionMax(0), r(-1), err(""), songList(QList<Song>()), code(-1){};
};

/**< 豆瓣歌曲请求信息*/
class SongRequestInfo
{
public:
	QString appName;	/**< 必选，是固定值radio_desktop_win */
	int version;		/**< 必选，是固定值100 */
	QString userId;		/**< 可选，若有则必须与expire和token搭配使用 */
	int expire;			/**< 可选，token过期时间msec */
	QString token;		/**< 可选，登录令牌 */
	QString sid;		/**< 可选，歌曲的id */
	QString h;			/**< 可选，最近播放的音乐，格式为 | song.sid:报告类型，如 |1386894:s|444482:p| */
	int channel;		/**< 必选，频道id */
	QString type;		/**< 必选，报告类型是一个字符，包括以下类型：
							 类型	需要参数		含义														报告长度
							 b		sid			bye，不再播放												短报告
							 e		sid			end，当前歌曲播放完毕，但是歌曲队列中还有歌曲					短报告
							 n					new，没有歌曲播放，歌曲队列也没有任何歌曲，需要返回新播放列表	长报告
							 p					playing，歌曲正在播放，队列中还有歌曲，需要返回新的播放列表
							 s		sid			skip，歌曲正在播放，队列中还有歌曲，适用于用户点击下一首		短报告
							 r		sid			rate，歌曲正在播放，标记喜欢当前歌曲							短报告
							 s		sid			skip，歌曲正在播放，队列中还有歌曲，适用于用户点击下一首		短报告
							 u		sid			unrate，歌曲正在播放，标记取消喜欢当前歌曲					短报告
						*/
	SongRequestInfo() : appName("radio_desktop_win"), version(100), userId(""), 
		expire(0), token(""), sid(""), h(""), channel(4), type("n"){};
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
		DOUBAN_GET_SONG_SUCCESS,				/**< 获取歌曲成功 */
		DOUBAN_JSON_ERROR,						/**< json有误 */
		DOUBAN_OBJ_ERROR,						/**< jsonObj有误 */
		DOUBAN_ARRARY_ERROR,					/**< jsonArray有误 */

		DOUBAN_LAST,							/**< 最后标记，请不要使用用 */
	}DoubanCode;

	DoubanAPI();
	~DoubanAPI();
	int userLogin(QString email, QString password, QString app_name = "radio_desktop_win", QString version = "100");
	int getChannelInfo();
	int getPlaySongList(SongRequestInfo info);
private:
	DoubanLogRetInfo loginRetInfo(QString jsonLoginStr, int netStatus);
	ChannelsInfo channelRetInfo(QString jsonChannelStr, int netStatus);
	SongInfo songRetInfo(QString jsonSongStr, int netStatus);
	QString getSongRequestUrl(SongRequestInfo);
signals:
	void userLogined(DoubanLogRetInfo loginInfo);
	void channelInfoed(ChannelsInfo channelInfo);
	void songInfoed(SongInfo songInfo);
private slots:
	void statusChange(NET_HANDLE handle);
private:
	const QString m_loginUrl;				/**< 登录请求Url */
	const QString m_channelUrl;				/**< 频道请求Url */
	const QString m_songsUrl;				/**< 歌曲列表信息请求Url */
	static DoubanLogRetInfo m_logifo;		/**< 登录返回信息 */
	NetControl *m_nc;						/**< 网络管理列表 */
	NET_HANDLE m_loginHandle;				/**< 登录句柄 */
	NET_HANDLE m_channelHandle;				/**< 频道请求网络句柄 */
	NET_HANDLE m_songsHandle;				/**< 歌曲列表信息请求网络句柄 */
};

