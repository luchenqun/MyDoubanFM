#include "WidgetMain.h"
#include <QApplication>
#include <QMediaPlayer>

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

//    QMediaPlayer *player = new QMediaPlayer(this);
//    QString song_link = "http://file.qianqian.com//data2/music/41937637/41937637.mp3?xcode=f263a12b1dc7c08eaca186416cbdae39";
//    player->setMedia(QUrl(song_link));  //song_link为根据步骤二获取的歌曲链接
//    player->setVolume(50);
//    player->play();

    m_dc = DownloadControl::instance();
    QString currentDir = QApplication::applicationDirPath();
    QString dir = currentDir + QStringLiteral("\\DownloadTest");
    m_dc->setDir(dir);

    QStringList urls;
//    urls << "http://file.qianqian.com//data2/music/41937637/41937637.mp3?xcode=f263a12b1dc7c08eaca186416cbdae39";
    urls << "https://github.com/luchenqun/MyDoubanFM/archive/master.zip";
    m_handle = m_dc->createTask(urls);
    m_dc->startTask(m_handle);
}

WidgetMain::~WidgetMain()
{
    m_dc->deleteTask(m_handle);
}
