#include "menucontent.h"
#include "ui_menucontent.h"
#include <QButtonGroup>
#include <QDebug>
#include "menulayout.h"
#include <QMessageBox>
#include "mybutton.h"

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
    connect(ui->sureBtn, SIGNAL(clicked()), this, SLOT(sureBtn_slot()));
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
        //view类型
        ui->switchMenuPage->setCurrentIndex(0);
        break;
    case 1:
        //click类型
        ui->switchMenuPage->setCurrentIndex(1);
        break;
    }
}

void MenuContent::sureBtn_slot()
{
    widget->getMenuLayout()->writeMenuToFile();
}

int MenuContent::getCoordI()
{
    return this->i;
}

int MenuContent::getCoordJ()
{
    return this->j;
}

/*
 * 填充修改的数据到MyButton里面
 */
void MenuContent::fillDataToButton(MyButton *btn)
{
    QString name = ui->menuNameEdit->text();
    bool complex = btn->isComplex();
    btn->setName(name);
    if( !complex )
    {
        //TODO 如果不带子按钮
        int id = rBtnGroup->checkedId();
        //checkId  0 表示view  1 表示click
        if( id == 0)
        {
            //TODO view类型的菜单
            btn->setType("view");
            btn->setKey(NO_KEY);
            QString url = ui->urlEdit->text();
            btn->setUrl(url);
        } else
        {
            btn->setType("click");
            int key = ui->keyEdit->text().toInt();
            btn->setKey(key);
        }

    }
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
}

void MenuContent::on_deleteBtn_slot()
{
    //如果有坐标，那么执行删除操作
    if( i != NO_COORD && j != NO_COORD)
    {
        widget->getMenuLayout()->deleteMenu(i, j);
        //删除之后按钮置零
    } else {
        QMessageBox::warning(NULL, QString("警告"), QString("没有选中任何菜单!"));
    }
}

/*
 * 更新当前内容面板里面的数据
 */
void MenuContent::refreshData(MyButton *btn)
{
    QString name = btn->getName();
    int key = btn->getKey();
    QString url = btn->getUrl();
    ui->menuNameEdit->setText(name);
    //如果是view类型
    if( key == NO_KEY)
    {
        ui->viewRBtn->setChecked(true);
        ui->switchMenuPage->setCurrentIndex(0);
        ui->keyEdit->setText("");
        ui->urlEdit->setText(url);
    } else
    {
        ui->clickRBtn->setChecked(true);
        ui->switchMenuPage->setCurrentIndex(1);
        ui->urlEdit->setText("");
        ui->keyEdit->setText(QString::number(key));
    }
}

MenuContent::~MenuContent()
{
    delete ui;
}
