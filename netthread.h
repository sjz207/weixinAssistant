#ifndef NETTHREAD_H
#define NETTHREAD_H

#include <QThread>

/*
 * 进行网络连接线程
 */
class NetThread : public QThread
{
    Q_OBJECT
public:
    explicit NetThread(QObject *parent = 0);

    void run();

signals:
    void finish();

    void error();

public slots:

};

#endif // NETTHREAD_H
