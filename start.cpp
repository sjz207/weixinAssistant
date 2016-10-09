#include "start.h"
#include "ui_start.h"
#include <QDebug>
#include <QSettings>
#include <QMessageBox>
#include "util.h"
#include "netthread.h"
#include <QProgressDialog>
#include <QTimer>

Start::Start(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Start),
    widget(NULL),
    net(NULL),
    progDlg(NULL),
    timer(NULL)
{
    ui->setupUi(this);
    connect(ui->editBtn, SIGNAL(clicked()), this, SLOT(on_editBtn_slot()));
    connect(ui->submitBtn, SIGNAL(clicked()), this, SLOT(on_submitBtn_slot()));
    init();
}

void Start::init()
{
    //从配置文件读取配置
    QSettings setting("config.ini", QSettings::IniFormat);
    setting.beginGroup("config");
    appId = setting.value("appid").toString();
//qDebug() << "appId:" << appId;
    if( appId.isEmpty() )
    {
//        writeConfig();
    } else
    {
        ui->appidEdit->setText(appId);
        secret = setting.value("secret").toString();
        ui->secretEdit->setText(secret);
    }
}

void Start::on_editBtn_slot()
{
    this->hide();
    if( !widget )
    {
        widget = new Widget;
        connect(widget, SIGNAL(closeWindow()), this, SLOT(on_widgetClose_slot()));
        widget->setStartQuote(this);
        widget->show();
    }
}

void Start::on_widgetClose_slot()
{
    widget->close();
    delete widget;
    widget = NULL;
}

/*
 * 写配置
 */
void Start::writeConfig()
{
    QString appId = ui->appidEdit->text();
    QString secret = ui->secretEdit->text();
    if( appId.isEmpty() || secret.isEmpty() )
    {
        QMessageBox::information(NULL, tr("提示"), tr("所填的配置项不能为空"));
    } else
    {
        QSettings setting("config.ini", QSettings::IniFormat);
        setting.beginGroup("config");
        setting.setValue("appid", appId);
        setting.setValue("secret", secret);
        setting.endGroup();
    }
}

void Start::on_submitBtn_slot()
{
    writeConfig();
    //首先获取access_token
    QString accessUrl = ACCESS_URL;
    accessUrl.replace(QRegularExpression("APPID"), appId).replace(QRegularExpression("APPSECRET"), secret);
//        Util::getInstance()->httpRequest(ACCESS_URL, "GET",  NULL);
    if( !net )
    {
        net = new NetThread;
        connect(net, SIGNAL(finish()), this, SLOT(on_finish_slot()));
        connect(Util::getInstance(), SIGNAL(returnMsg(QString)), net, SLOT(on_msg_slot(QString)));
        connect(net, SIGNAL(errcode(QString)), this, SLOT(on_errcode_slot(QString)));
    }
    net->setAccess(accessUrl);
    net->start();
    if( !progDlg )
    {
        progDlg = new QProgressDialog;
        progDlg->setFixedWidth(300);
        progDlg->setRange(0, 100);
    }
    if( progDlg->wasCanceled() )
    {
        progDlg->reset();
    }
    progDlg->show();
    if( !timer )
    {
        timer = new QTimer;
        connect(timer, SIGNAL(timeout()), this, SLOT(updateProgress()));
    }
    currentValue = 0;
    progDlg->setValue(currentValue);
    timer->start(100);
}

void Start::updateProgress()
{
    currentValue ++;
    if( currentValue == 100)
    {
        currentValue = 0;
    }
    progDlg->setValue(currentValue);
//    QCoreApplication::processEvents();
    if( progDlg->wasCanceled() )
    {
        progDlg->setHidden(true);
//        delete progDlg;
//        progDlg = NULL;
    }
}

void Start::on_errcode_slot(QString str)
{
    progDlg->cancel();
    QMessageBox::information(NULL,  tr("提示!"), QString("修改菜单错误码:%1").arg(str));
    if( !net )
    {
        delete net;
        net = NULL;
    }
}

/*
 * 菜单更改成功之后的槽函数
 */
void Start::on_finish_slot()
{
    progDlg->cancel();
qDebug() << "修改成功";
}

Start::~Start()
{
    delete ui;
    if( net )
    {
        delete net;
        net = NULL;
    }
    if( progDlg )
    {
        delete progDlg;
        progDlg = NULL;
    }
    if( !timer )
    {
        delete timer;
        timer = NULL;
    }
}
