#include "netthread.h"
#include <QDebug>
#include "util.h"
#include <QMessageBox>
#include <QRegularExpression>

const QString NetThread::MenuUrl = "https://api.weixin.qq.com/cgi-bin/menu/create?access_token=ACCESS_TOKEN";

NetThread::NetThread(QObject *parent) :
    QThread(parent)
{
}

/*
 * 得到AccessToken的url
 */
void NetThread::setAccess(QString url)
{
    this->accessUrl = url;
}

void NetThread::on_msg_slot(QString str)
{
    result = str;
//qDebug() << "result:" << result;
    QString err = Util::getInstance()->getMsg(result, "errcode");
    if( !err.isEmpty() )
    {
//qDebug() << "err :" << err;
        emit errcode(err);
        return;
    }
    if( result.isEmpty() )
    {
//qDebug() << "result inner: "<< result;
        return;
    } else if( !Util::getInstance()->getMsg(result, "access_token").isEmpty())
    {
        QString access = Util::getInstance()->getMsg(result, "access_token");
//qDebug() << "access :" << access;
        QString url = MenuUrl;
        if( access.isEmpty() )
        {
            return;
        }
        url.replace(QRegularExpression("ACCESS_TOKEN"), access);
        QString data = Util::getInstance()->getMenuData();
        result = "";
        Util::getInstance()->httpRequest(url, "POST", data);
//        emit finish();
    }
}

void NetThread::run()
{

    Util::getInstance()->httpRequest(accessUrl, "GET", NULL);
    //开启事件循环,否则httpRequest函数将无法接收到信号
    exec();
}
