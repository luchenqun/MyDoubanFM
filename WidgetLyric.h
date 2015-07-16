#ifndef WIDGETLYRIC_H
#define WIDGETLYRIC_H

#include <QWidget>
#include <QDebug>
#include <QLabel>
#include <QPixmap>

#include "NetControl.h"

class WidgetLyric : public QWidget
{
    Q_OBJECT
public:
    explicit WidgetLyric(QWidget *parent = 0);

signals:

public slots:
    void songPicUpdate(QString url);
    void statusChange(NET_HANDLE handle);
private:
    NetControl *m_nc;
    NET_HANDLE m_picHandle;
    QLabel *m_pic;
};

#endif // WIDGETLYRIC_H
