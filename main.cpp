#include "WidgetMain.h"
#include <QApplication>
#include <QFile>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    //NOTICE：加载QSS样式表（相当于Web开发里面的CSS样式表），请将所有一致的样式设置写入DownloadTool.qss文件
    QFile qss(":/qss/style");
    qss.open(QFile::ReadOnly);
    a.setStyleSheet(qss.readAll());
    qss.close();

    WidgetMain w;
    w.show();

    return a.exec();
}
