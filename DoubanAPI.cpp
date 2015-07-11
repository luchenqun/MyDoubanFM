#include "DoubanAPI.h"

DoubanAPI::DoubanAPI() :
	m_loginUrl("https://www.douban.com/j/app/login"),
	m_channelUrl("https://www.douban.com/j/app/radio/channels"),
	m_songsUrl("https://www.douban.com/j/app/radio/people")
{
	m_nc = NetControl::singleton();
	connect(m_nc, &NetControl::statusChanged, this, &DoubanAPI::statusChange);
}


DoubanAPI::~DoubanAPI()
{
}

/** 
* @brief 用户登录 
* @author LuChenQun
* @date 2015/07/07
* @param[in] email 用户注册的邮箱
* @param[in] pwd 密码
* @param[in] app_name 默认值为radio_desktop_win
* @param[in] version 默认值为100
* @return int 请求返回值
*/
int DoubanAPI::userLogin(QString email, QString password, QString app_name, QString version)
{
	QString loginUrl = m_loginUrl + "?email=" + email + "&password=" + password + "&app_name=" + app_name + "&version=" + version;
	m_loginHandle = m_nc->createTask(loginUrl, NetWork::TASK_HTTP_POST);
	return m_nc->startTask(m_loginHandle);
}

/** 
* @brief 获取电台列表
* @author LuChenQun
* @date 2015/07/07
* @return int 请求返回值
*/
int DoubanAPI::getChannelInfo()
{
	m_channelHandle = m_nc->createTask(m_channelUrl, NetWork::TASK_HTTP_GET);
	return m_nc->startTask(m_channelHandle);
}

/** 
* @brief 请求歌曲列表信息
* @author LuChenQun
* @date 2015/07/11
* @param[in] info 请求信息
* @return int 请求返回值
*/
int DoubanAPI::getPlaySongList(SongRequestInfo info)
{
	QString url = getSongRequestUrl(info);
	m_songsHandle = m_nc->createTask(url, NetWork::TASK_HTTP_GET);
	return m_nc->startTask(m_songsHandle);
}

/** 
* @brief 网络方面状态信号槽改变
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

	if (m_songsHandle == handle)
	{
		emit songInfoed(songRetInfo(data, status));
	}
}

/** 
* @brief 通过str获取登录返回信息
* @author LuChenQun
* @date 2015/07/07
* @param[in] jsonLoginStr 一段Json的字符串
* @param[in] netStatus 网络返回码
* @return DoubanLoginRetInfo 登录返回信息
*/
DoubanLogRetInfo DoubanAPI::loginRetInfo(QString jsonLoginStr, int netStatus)
{
	DoubanLogRetInfo loginInfo;
	loginInfo.code = DOUBAN_LOGIN;

	QJsonParseError jsonError;
	QByteArray data = jsonLoginStr.toUtf8();
	QJsonDocument jsonDoc = QJsonDocument::fromJson(data, &jsonError);

	// 检验错误
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
* @brief 通过str获取登录返回信息
* @author LuChenQun
* @date 2015/07/07
* @param[in] jsonChannelStr 一段频道的json字符串
* @param[in] netStatus 网络返回码
* @return ChannelRetInfo 频道信息
*/
ChannelsInfo DoubanAPI::channelRetInfo(QString jsonChannelStr, int netStatus)
{
	ChannelsInfo channelsInfo;
	channelsInfo.code = DOUBAN_GET_CHANNEL_SUCCESS;

	QJsonParseError jsonError;
	QJsonDocument jsonDoc = QJsonDocument::fromJson(jsonChannelStr.toUtf8(), &jsonError);
	QJsonObject obj = jsonDoc.object();
	QJsonValue channelsValue = obj.take("channels");

	// 部分错误检查
	do 
	{
		if (netStatus != NetWork::NETWORK_FINISH_SUCCESS){ channelsInfo.code = netStatus; break; }
		if (jsonError.error != QJsonParseError::NoError){ channelsInfo.code = DOUBAN_JSON_ERROR; break; }
		if (!jsonDoc.isObject()){ channelsInfo.code = DOUBAN_OBJ_ERROR; break; }
		if (!channelsValue.isArray()){ channelsInfo.code = DOUBAN_ARRARY_ERROR; break; }
	} while (false);

	// json转换
	if (DOUBAN_GET_CHANNEL_SUCCESS == channelsInfo.code)
	{
		QJsonArray array = channelsValue.toArray();
        foreach (QJsonValue value, array)
        {
			Channel channel;

			QJsonObject netChannel = value.toObject();
			channel.nameEn = netChannel.take("name_en").toString();
			channel.seqId = netChannel.take("seq_id").toInt();
			channel.abbrEn = netChannel.take("abbr_en").toString();
			channel.name = netChannel.take("name").toString();
			channel.channelId = netChannel.take("channel_id").toInt();

			channelsInfo.channelList.append(channel);
		}
	}

	return channelsInfo;
}

/** 
* @brief 
* @author LuChenQun
* @date 2015/07/11
* @param[in] jsonSongStr 一段含有歌曲信息的json字符串
* @param[in] netStatus 网络返回码
* @return SongInfo  歌曲信息
*/
SongInfo DoubanAPI::songRetInfo(QString jsonSongStr, int netStatus)
{
	SongInfo songInfo;
	songInfo.code = DOUBAN_GET_SONG_SUCCESS;

	QJsonParseError jsonError;
	QJsonDocument jsonDoc = QJsonDocument::fromJson(jsonSongStr.toUtf8(), &jsonError);
	QJsonObject songsObj = jsonDoc.object();
	QJsonValue channelsValue = songsObj.take("song");

	// 部分错误检查
	do
	{
		if (netStatus != NetWork::NETWORK_FINISH_SUCCESS){ songInfo.code = netStatus; break; }
		if (jsonError.error != QJsonParseError::NoError){ songInfo.code = DOUBAN_JSON_ERROR; break; }
		if (!jsonDoc.isObject()){ songInfo.code = DOUBAN_OBJ_ERROR; break; }
		if (!channelsValue.isArray()){ songInfo.code = DOUBAN_ARRARY_ERROR; break; }
	} while (false);

	// json转换
	if (DOUBAN_GET_SONG_SUCCESS == songInfo.code)
	{
		songInfo.r = songsObj.take("r").toInt();
		songInfo.versionMax = songsObj.take("version_max").toInt();
		songInfo.err = (songInfo.r == 0) ? (songsObj.take("err").toString()) : ("");

		QJsonArray array = channelsValue.toArray();

        foreach (QJsonValue value, array)
		{
			QJsonObject netSong = value.toObject();

			Song song;
			song.album = netSong.take("album").toString();
			song.picture = netSong.take("picture").toString();	
			song.ssid = netSong.take("ssid").toInt();		
			song.artist = netSong.take("artist").toString();		
			song.url = netSong.take("url").toString();		
			song.company = netSong.take("company").toString();	
			song.title = netSong.take("title").toString();		
			song.ratingAvg = netSong.take("rating_avg").toString();	
			song.length = netSong.take("length").toInt();
			song.subtype = netSong.take("subtype").toString();	
			song.publicTime = netSong.take("public_time").toString();	
			song.sid = netSong.take("sid").toString();		
			song.aid = netSong.take("aid").toString();		
			song.kbps = netSong.take("kbps").toInt();
			song.albumtitle = netSong.take("albumtitle").toString();	
			song.like = netSong.take("like").toInt();

			songInfo.songList.append(song);
		}
	}

	return songInfo;
}

/** 
* @brief 根据请求的信息，获取请求的url
* @author LuChenQun
* @date 2015/07/11
* @param[in] info 请求信息
* @return QT_NAMESPACE::QString url
*/
QString DoubanAPI::getSongRequestUrl(SongRequestInfo info)
{
	QString url = m_songsUrl;

	url += (info.appName != "") ? ("?app_name=" + info.appName) : ("");
	url += (info.version > 0) ? ("&version=" + QString::number(info.version)) : ("");
	url += (info.userId != "") ? ("&user_id=" + info.userId) : ("");
	url += (info.expire > 0) ? ("&expire=" + QString::number(info.expire)) : ("");
	url += (info.token != "") ? ("&token=" + info.token) : ("");
	url += (info.sid != "") ? ("&sid=" + info.sid) : ("");
	url += (info.h != "") ? ("&h=" + info.h) : ("");
	url += (info.channel > 0) ? ("&channel=" + QString::number(info.channel)) : ("");
	url += (info.type != "") ? ("&type=" + info.type) : ("");

	return url;
}
