#include "WidgetPlay.h"
#include <QLabel>

WidgetPlay::WidgetPlay(QWidget *parent) : QWidget(parent)
{
	this->setFixedWidth(300);

    m_titleLab = new QLabel("我只在乎你", this);
    m_artistLab = new QLabel("邓丽君", this);
    m_titleLab->setObjectName("title");

	m_playProPb = new QProgressBar(this);
    m_songCurTimeLab = new QLabel("01:34", this);
    m_songTotalTimeLab = new QLabel("04:45", this);
    m_playProPb->setObjectName("playPro");

	m_likeBtn = new PushButton(this);
	m_hateBtn = new PushButton(this);
	m_pauseBtn = new PushButton(this);
	m_nextBtn = new PushButton(this);
	m_likeBtn->setPicName(QString(":/img/res/img/heart.png"));
	m_hateBtn->setPicName(QString(":/img/res/img/delete.png"));
    m_pauseBtn->setPicName(QString(":/img/res/img/video_play.png"));
    m_nextBtn->setPicName(QString(":/img/res/img/video_forward.png"));

    absoluteLayout();
	// 网络操作
	m_nc = NetControl::singleton();

	m_doubanAPI = new DoubanAPI();
	//m_doubanAPI->userLogin("luchenqun@qq.com", "fendoubuxi596320");
	//m_doubanAPI->getChannelInfo();
	//m_doubanAPI->getPlaySongList(*(new SongRequestInfo()));
	connect(m_doubanAPI, &DoubanAPI::userLogined, this, &WidgetPlay::userLogin);
	connect(m_doubanAPI, &DoubanAPI::channelInfoed, this, &WidgetPlay::channelInfo);
	connect(m_doubanAPI, &DoubanAPI::songInfoed, this, &WidgetPlay::songInfo);

	m_doubanAPI->getPlaySongList(*(new SongRequestInfo()));
	connect(m_nextBtn, &PushButton::clicked, this, [=](){
		if (m_play->state() == QMediaPlayer::PlayingState)
		{
			m_play->stop();
		}

		m_titleLab->setText(m_song.title);
		m_artistLab->setText(m_song.artist);

		m_play->setMedia(QUrl(m_song.url));
		m_play->setVolume(50);
		m_play->play();

		m_doubanAPI->getPlaySongList(*(new SongRequestInfo()));
	});

	m_play = new QMediaPlayer(this);
}

// 这么少的元素，而且窗口大小固定，使用绝对布局吧。
void WidgetPlay::absoluteLayout()
{
    m_titleLab->move(0, 0);
    m_artistLab->move(0, 30);

    m_playProPb->move(0, 60);
    m_songCurTimeLab->move(80, 60);
    m_songTotalTimeLab->move(120, 60);

    m_likeBtn->move(0, 100);
    m_hateBtn->move(60, 100);
    m_pauseBtn->move(120, 100);
    m_nextBtn->move(180, 100);
}

WidgetPlay::~WidgetPlay()
{
	delete m_doubanAPI;
}


void WidgetPlay::userLogin(DoubanLogRetInfo loginInfo)
{
	qDebug() << "loginInfo = " << loginInfo.r;
}

void WidgetPlay::channelInfo(ChannelsInfo channelInfo)
{
	qDebug() << "channelInfo = " << channelInfo.code;
}

void WidgetPlay::songInfo(SongInfo songInfo)
{
	if (songInfo.r == 0)
	{
		if (songInfo.songList.size() > 0)
		{
			m_song = songInfo.songList.at(0);
		}
	}
}
