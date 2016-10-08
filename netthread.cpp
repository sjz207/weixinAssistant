#include "netthread.h"
#include <QDebug>

NetThread::NetThread(QObject *parent) :
    QThread(parent)
{
}


void NetThread::run()
{
    for( int i = 0; i < 100000; i++)
    {
qDebug() << "i'm value:" << i;
    }
    emit finish();
}
