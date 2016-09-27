#include "widget.h"
#include "ui_widget.h"
#include "menulayout.h"
#include "menucontent.h"
#include <QHBoxLayout>

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    init();
}

void Widget::init()
{
    menuLayout = new MenuLayout(this);
    //让menuLayout对象保存容器的指针
    menuLayout->setWidgetQuote(this);
    menuContent = new MenuContent(this);
    menuContent->setWidgetQuote(this);
    QHBoxLayout *layout = new QHBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->addWidget(menuLayout);
    layout->addWidget(menuContent);
    setLayout(layout);
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
