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

	// 布局
	QVBoxLayout *mainLayout = new QVBoxLayout(this);

    QHBoxLayout *layout0 = new QHBoxLayout();
	layout0->addWidget(m_titleLab);

	QHBoxLayout *layout1 = new QHBoxLayout();
	layout1->addWidget(m_artistLab);

    QHBoxLayout *layout2 = new QHBoxLayout();
	layout2->addWidget(m_playProPb, 3);
	layout2->addWidget(m_songCurTimeLab, 1);
	layout2->addWidget(m_songTotalTimeLab, 1);

    QHBoxLayout *layout3 = new QHBoxLayout();
	layout3->addWidget(m_likeBtn);
	layout3->addWidget(m_hateBtn);
	layout3->addWidget(m_pauseBtn);
	layout3->addWidget(m_nextBtn);

	mainLayout->addLayout(layout0);
	mainLayout->addLayout(layout1);
	mainLayout->addLayout(layout2);
	mainLayout->addLayout(layout3);

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
