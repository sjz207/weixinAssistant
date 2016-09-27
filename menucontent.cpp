#include "menucontent.h"
#include "ui_menucontent.h"
#include <QButtonGroup>
#include <QDebug>
#include "menulayout.h"
#include <QMessageBox>

MenuContent::MenuContent(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MenuContent),
    widget(NULL)
{
    i = NO_COORD;
    j = NO_COORD;
    ui->setupUi(this);
    init();
}

void MenuContent::init()
{
    rBtnGroup = new QButtonGroup(this);
    rBtnGroup->addButton(ui->clickRBtn, 1);
    rBtnGroup->addButton(ui->viewRBtn, 0);
    ui->viewRBtn->setChecked(true);
    connect(ui->clickRBtn, SIGNAL(clicked()), this, SLOT(on_btnGroup_slot()));
    connect(ui->viewRBtn, SIGNAL(clicked()), this, SLOT(on_btnGroup_slot()));
    connect(ui->deleteMenuBtn, SIGNAL(clicked()), this, SLOT(on_deleteBtn_slot()));
    ui->deleteMenuBtn->setStyleSheet("color:blue");
    ui->topWidget->setStyleSheet("QWidget#topWidget {\
                                 border-bottom : 1px solid #00FFFF;}");
}

void MenuContent::on_btnGroup_slot()
{
    int btnId = rBtnGroup->checkedId();
    switch( btnId )
    {
    case 0:
        ui->switchMenuPage->setCurrentIndex(0);
        break;
    case 1:
        ui->switchMenuPage->setCurrentIndex(1);
        break;
    }
}

int MenuContent::getCoordI()
{
    return this->i;
}

//隐藏
void MenuContent::hideContent()
{
    ui->radioWidget->hide();
    ui->switchMenuPage->hide();
}
//显示
void MenuContent::showContent()
{
    if( ui->radioWidget->isHidden() && ui->switchMenuPage->isHidden()) {
        ui->radioWidget->show();
        ui->switchMenuPage->show();
    }

}

void MenuContent::setWidgetQuote(Widget *widget)
{
    this->widget = widget;
}

/*
 * 保存当前按钮的坐标，方便删除按钮的操作
 */
void MenuContent::setCoord(int i, int j)
{
    this->i = i;
    this->j = j;
    qDebug() << "setCoord i:" << this->i << "j:" << this->j;
}

void MenuContent::on_deleteBtn_slot()
{
qDebug() << "delete_slot!";
qDebug() << "on_deleteBtn_slot i:" << i << "j:" << j;
    //如果有坐标，那么执行删除操作
    if( i != NO_COORD && j != NO_COORD)
    {
        widget->getMenuLayout()->deleteMenu(i, j);
        //删除之后按钮置零
        i = NO_COORD;
        j = NO_COORD;
    } else {
        QMessageBox::warning(NULL, QString("警告"), QString("没有选中任何菜单!"));
    }
    qDebug() << "i:" << i << "j:" << j;
}

MenuContent::~MenuContent()
{
    delete ui;
}
