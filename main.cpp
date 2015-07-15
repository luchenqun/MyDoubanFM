#include "WidgetMain.h"
#include <QApplication>
#include <QFile>
#include <QFont>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    // NOTICE：加载QSS样式表（相当于Web开发里面的CSS样式表），请将所有一致的样式设置写入DownloadTool.qss文件
    QFile qss(":/qss/style");
    qss.open(QFile::ReadOnly);
    a.setStyleSheet(qss.readAll());
    qss.close();

    // 设置全局字体
    QFont font;
    QString fontFamily("Microsoft YaHei");
    font.setFamily(fontFamily);
    a.setFont(font);

    WidgetMain w;
    w.show();

    return a.exec();
}
