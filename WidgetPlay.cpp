#include "WidgetPlay.h"
#include <QLabel>
#include <QPalette>

WidgetPlay::WidgetPlay(QWidget *parent) : QWidget(parent)
{
    this->setFixedSize(300, 300);
    this->setAutoFillBackground(true);

    m_titleLab = new QLabel(QStringLiteral("豆瓣音乐听见好时光"), this);
    m_artistLab = new QLabel(QStringLiteral("豆瓣豆瓣豆瓣豆瓣豆瓣"), this);
    m_titleLab->setObjectName("title");
    m_artistLab->setObjectName("artist");

	m_playProPb = new QProgressBar(this);
    m_songCurTimeLab = new QLabel("00:00", this);
    m_obliqueLab = new QLabel("/", this);
    m_songTotalTimeLab = new QLabel("00:00", this);
    m_playProPb->setObjectName("playPro");
    m_playProPb->setTextVisible(false);

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

    m_curPlaytimer = new QTimer(this);
    m_curPlaytimer->setInterval(1000);
    connect(m_curPlaytimer, &QTimer::timeout, this, [=]{
        if (m_play->state() == QMediaPlayer::PlayingState)
        {
            int pos = m_play->position() / 1000;
            int minPos =  m_song.length / 100;
            m_songCurTimeLab->setText(secondToMMSS(pos));
            m_playProPb->setValue(pos <= minPos ? (minPos) : pos);
        }
    });

	m_doubanAPI->getPlaySongList(*(new SongRequestInfo()));
	connect(m_nextBtn, &PushButton::clicked, this, [=](){
		if (m_play->state() == QMediaPlayer::PlayingState)
		{
			m_play->stop();
		}

		m_titleLab->setText(m_song.title);
		m_artistLab->setText(m_song.artist);
        m_songTotalTimeLab->setText(secondToMMSS(m_song.length));
        m_playProPb->setRange(0, m_song.length);
		m_play->setMedia(QUrl(m_song.url));
		m_play->setVolume(50);
		m_play->play();
        m_curPlaytimer->start();
        emit songPicUpdated(m_song.picture);

		m_doubanAPI->getPlaySongList(*(new SongRequestInfo()));

        QPalette palette;
        palette.setColor(QPalette::Background, QColor(rand() % 255, rand() % 255, rand() % 255));
        this->setPalette(palette);
	});

	m_play = new QMediaPlayer(this);

#if 0
    this->setStyleSheet("border:1px solid red");
#endif
}

// 这么少的元素，而且窗口大小固定，使用绝对布局吧。
void WidgetPlay::absoluteLayout()
{
    int xAdd = 20;
    int yAdd = 20;
    int y[4] ={20 + yAdd, 80 + yAdd, 140 + yAdd, 200 + yAdd};
    m_titleLab->move(0 + xAdd, y[0]);
    m_artistLab->move(0 + xAdd, y[1]);


    m_playProPb->move(0 + xAdd, y[2]);

    m_songCurTimeLab->move(190 + xAdd, y[2]-8);
    m_obliqueLab->move(223 + xAdd, y[2]-9);
    m_songTotalTimeLab->move(230 + xAdd, y[2]-8);

    m_likeBtn->move(0 + xAdd, y[3]);
    m_hateBtn->move(60 + xAdd, y[3]);
    m_pauseBtn->move(120 + xAdd, y[3]);
    m_nextBtn->move(180 + xAdd, y[3]);
}

QString WidgetPlay::secondToMMSS(int sec)
{
    int minute = sec / 60;
    sec = sec % 60;
    QString ret = QString("%1:%2").arg(minute, 2, 10, QChar('0')).arg(sec, 2, 10, QChar('0'));
    return ret;
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
