#include "WidgetMain.h"
#include <QApplication>
#include <QMediaPlayer>
#include <QTime>


WidgetMain::WidgetMain(QWidget *parent)
    : WidgetBase(parent),
	m_WidgetMainMargins(10)
{
	this->setFixedSize(615 + m_WidgetMainMargins * 2, 308 + m_WidgetMainMargins * 2 + 30);
    setWindowIcon(QIcon(":/img/res/img/my_douban_fm.ico"));	// 设置任务栏图标
    this->setWidgetMoveRange(50);

    m_WidgetTitle = new WidgetTitle(this);

    m_WidgetFunction = new WidgetSettings();
    m_WidgetPlay = new WidgetPlay();
    QHBoxLayout *functionLayout = new QHBoxLayout();
    functionLayout->addWidget(m_WidgetFunction);
    functionLayout->addWidget(m_WidgetPlay);

    // 总体布局
    QVBoxLayout *mainLayout = new QVBoxLayout();
    mainLayout->addWidget(m_WidgetTitle);
    mainLayout->addLayout(functionLayout);
    mainLayout->setSpacing(0);

	mainLayout->setContentsMargins(m_WidgetMainMargins, m_WidgetMainMargins, m_WidgetMainMargins, m_WidgetMainMargins);

    this->setLayout(mainLayout);

    QMediaPlayer *player = new QMediaPlayer(this);
    QString song_link = "http://mr3.douban.com/201507072108/28b8a3939233f634bc979f3dcd41d11f/view/song/small/p192248_128k.mp3";
    player->setMedia(QUrl(song_link));  //song_link为根据步骤二获取的歌曲链接
    player->setVolume(50);
    player->play();

    m_nc = NetControl::singleton();

	QString getUrl = "http://lcqlcq.sinaapp.com/2048Update.php";
	QString postUrl = "https://www.douban.com/j/app/login?app_name=radio_desktop_win&version=100&email=luchenqun@qq.com&password=fendoubuxi596320";
	QString downfileUrl = "http://dldir1.qq.com/qqfile/qq/QQ7.4/15190/QQ7.4.exe";
	//m_netHttpGet = m_nc->createTask(getUrl, NetWork::TASK_HTTP_GET);
	//m_nc->startTask(m_netHttpGet);

	//m_netHttpPost = m_nc->createTask(postUrl, NetWork::TASK_HTTP_POST);
	//m_nc->startTask(m_netHttpPost);

	//m_netDownfile = m_nc->createTask(downfileUrl, NetWork::TASK_DOWNLOAD_FILE);
	//m_nc->startTask(m_netDownfile);

	connect(m_nc, &NetControl::statusChanged, this, &WidgetMain::statusChange);
	connect(m_nc, &NetControl::progressed, this, &WidgetMain::progress);


}

WidgetMain::~WidgetMain()
{
}

void WidgetMain::statusChange(NET_HANDLE netHandle)
{
	int netCode = m_nc->getNetCode(netHandle);
	switch (netCode)
	{
	case NetWork::NETWORK_FINISH_SUCCESS:
		qDebug() << netHandle << " data:" << m_nc->getReceiveData(netHandle);
		break;
	default:
		break;
	}
}

void WidgetMain::progress(NET_HANDLE netHandle)
{
	if (netHandle->getTaskType() == NetWork::TASK_DOWNLOAD_FILE)
	{
		qDebug() << netHandle->getFileName() << " progress = " << m_nc->getProgress(netHandle) << "%";
	}
}