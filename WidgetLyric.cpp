#include "WidgetLyric.h"

WidgetLyric::WidgetLyric(QWidget *parent) : QWidget(parent)
{
    this->setFixedSize(300, 300);
    m_nc = NetControl::singleton();
    m_pic = new QLabel(this);
    connect(m_nc, &NetControl::statusChanged, this, &WidgetLyric::statusChange);
}

void WidgetLyric::songPicUpdate(QString url)
{
    qDebug() << url;
    m_picHandle = m_nc->createTask(url, NetWork::TASK_DOWNLOAD_FILE);
    m_nc->startTask(m_picHandle);
}


void WidgetLyric::statusChange(NET_HANDLE handle)
{
    if(m_picHandle == handle)
    {
        qDebug() << "----" << m_nc->getNetCode(handle);
        if(m_nc->getNetCode(handle) == NetWork::NETWORK_FINISH_SUCCESS)
        {
            QPixmap pixmap;
            QString filePath = m_nc->getFilePath(handle);
            pixmap.load(filePath);
            m_pic->setPixmap(pixmap);
            m_pic->setFixedSize(300, 300);
            m_pic->setScaledContents(true);
            qDebug() << filePath;
        }
    }
}
