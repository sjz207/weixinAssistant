#include "widget.h"
#include "ui_widget.h"
#include "menulayout.h"
#include "menucontent.h"
#include <QHBoxLayout>
#include <QPushButton>
#include <QDebug>
#include <QMessageBox>
#include "start.h"

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    init();
}

void Widget::init()
{
//    menuLayout = new MenuLayout(this);
//    //让menuLayout对象保存容器的指针
//    menuLayout->setWidgetQuote(this);
//    menuContent = new MenuContent(this);
//    menuContent->setWidgetQuote(this);
//    QHBoxLayout *layout = new QHBoxLayout(this);
//    layout->setContentsMargins(0, 0, 0, 0);
//    layout->addWidget(menuLayout);
//    layout->addWidget(menuContent);
//    setLayout(layout);
    QWidget *widget = new QWidget(this);
    widget->setObjectName("mainWidget");
    menuLayout = new MenuLayout(widget);
    menuLayout->setWidgetQuote(this);
    menuContent = new MenuContent(widget);
    menuContent->setWidgetQuote(this);
    QVBoxLayout *vLayout = new QVBoxLayout(this);
    vLayout->addWidget(widget);
    btn = new QPushButton("保存菜单数据并返回主面板", this);
    vLayout->addWidget(btn);
    setLayout(vLayout);
    QHBoxLayout *hLayout = new QHBoxLayout(widget);
    hLayout->setContentsMargins(0, 0, 0, 0);
    hLayout->setMargin(0);
    hLayout->addWidget(menuLayout);
    hLayout->addWidget(menuContent);
    //设置下面颜色的边框
    widget->setStyleSheet("QWidget#mainWidget {\
                          border-bottom: 1px solid #FFB5C5;}");
    widget->setLayout(hLayout);
    //连接信号和槽
    connect(btn, SIGNAL(clicked()), this, SLOT(on_btnClick_slot()));
}

void Widget::on_btnClick_slot()
{
qDebug() << "槽触发!";
    QMessageBox::StandardButton rb = QMessageBox::question(NULL, tr("确定"), tr("你确定要返回主面板吗？"), QMessageBox::Yes | QMessageBox::No,
                                                           QMessageBox::Yes);
    if( rb == QMessageBox::Yes)
    {
qDebug() << "yes";
//        close();
        emit closeWindow();
        s->show();
    } else
    {
        //TODO 什么也不做
qDebug() << "no";
    }

}

void Widget::setStartQuote(Start *s)
{
    this->s = s;
}

MenuContent * Widget::getMenuContent()
{
    return menuContent;
}

MenuLayout *Widget::getMenuLayout()
{
    return menuLayout;
}

Widget::~Widget()
{
    delete ui;
}
