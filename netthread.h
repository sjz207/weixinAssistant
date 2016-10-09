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

    void setAccess(QString url);

signals:
    void finish();

    void error();

    void errcode(QString);

public slots:
    void on_msg_slot(QString);

private:
    QString accessUrl;
    QString result;

    const static QString MenuUrl ;

};

#endif // NETTHREAD_H
