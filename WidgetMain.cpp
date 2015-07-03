#include "WidgetMain.h"
#include <QApplication>
#include <QMediaPlayer>
#include <QTime>


WidgetMain::WidgetMain(QWidget *parent)
    : WidgetBase(parent)
{
    this->setFixedSize(600 + WidgetMainMargins * 2, 300 + WidgetMainMargins * 2 + 30);
    setWindowIcon(QIcon(":/img/res/img/my_douban_fm.ico"));	// 设置任务栏图标
    this->setWidgetMoveRange(50);

    m_WidgetTitle = new WidgetTitle(this);

    m_WidgetFunction = new WidgetFunction();
    m_WidgetPlay = new WidgetPlay();
    QHBoxLayout *functionLayout = new QHBoxLayout();
    functionLayout->addWidget(m_WidgetFunction);
    functionLayout->addWidget(m_WidgetPlay);

    // 总体布局
    QVBoxLayout *mainLayout = new QVBoxLayout();
    mainLayout->addWidget(m_WidgetTitle);
    mainLayout->addLayout(functionLayout);
    mainLayout->setSpacing(0);

    mainLayout->setContentsMargins(WidgetMainMargins, WidgetMainMargins, WidgetMainMargins, WidgetMainMargins);

    this->setLayout(mainLayout);

    QMediaPlayer *player = new QMediaPlayer(this);
    QString song_link = "http://mr3.douban.com/201301221627/ca5607811b5ad84a5b66990f19135dbd/view/song/small/p425687.mp3";
    player->setMedia(QUrl(song_link));  //song_link为根据步骤二获取的歌曲链接
    player->setVolume(50);
    player->play();

    m_nc = NetControl::singleton();
    m_netHandle = m_nc->createTask("https://www.baidu.com/", NetWork::TASK_HTTP_GET);
	QTime time;
	time.start();
	//m_netHandle->setRunMode(NetWork::SYNCHRONIZATION);
    m_nc->startTask(m_netHandle);
	qDebug() << "Http Get run time = " << time.elapsed();

}

WidgetMain::~WidgetMain()
{
}
