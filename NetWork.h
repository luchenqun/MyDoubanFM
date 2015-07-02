#ifndef NETWORK_H
#define NETWORK_H

#include <QObject>
#include <QThread>
#include <QFile>
#include "curl.h"

class NetWork : public QObject
{
    Q_OBJECT
public:
    enum TaskType
    {
        TASK_HTTP_GET,          //
        TASK_DOWNLOAD_FILE,
    };

    explicit NetWork(QObject *parent = 0);
    void initThread(QThread *thread);
    QThread* getThread();
    TaskType getTaskType();
    void setTaskType(TaskType taskType);

    int initHttpGet(QString url);
    static size_t httpGetWriteData(void* buffer, size_t size, size_t n, void *user);
signals:

public slots:
    int startHttpGet();
private:
    CURL *m_curl;
    QThread *m_thread;
    QString m_url;      // 链接
    TaskType m_taskType;
    QFile m_file;
};

#endif // NETWORK_H
