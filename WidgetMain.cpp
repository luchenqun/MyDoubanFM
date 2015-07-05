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

    //m_netHttpGet = m_nc->createTask("https://www.baidu.com/img/bdlogo.png", NetWork::TASK_HTTP_GET);
    //m_nc->startTask(m_netHttpGet);

	QString url = "http://www.douban.com/j/app/login?app_name=radio_dsktop_win&version=100&email=luchenqun@qq.com&password=fendoubuxi596320";
	m_netHttpPost = m_nc->createTask(url, NetWork::TASK_HTTP_POST);
	m_nc->startTask(m_netHttpPost);

	connect(m_nc, &NetControl::statusChanged, this, &WidgetMain::statusChange);

}

WidgetMain::~WidgetMain()
{
}


void WidgetMain::statusChange(NET_HANDLE netHandle)
{
	int status = m_nc->getStatus(netHandle);
	switch (status)
	{
	case NetWork::NETWORK_FINISH_SUCCESS:
		qDebug() << "data:" << m_nc->getReceiveData(netHandle).left(4000);
		break;
	default:
		break;
	}
}