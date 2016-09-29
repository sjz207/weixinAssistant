#include "menulayout.h"
#include "ui_menulayout.h"
#include <QHBoxLayout>
#include <QPushButton>
#include <QDebug>
#include "mybutton.h"
#include "menucontent.h"
#include <QMessageBox>
#include "util.h"


MenuLayout::MenuLayout(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MenuLayout),
    addBtn(NULL),
    widget(NULL),
    subMenuPanel(NULL),
    addSubBtn(NULL)
{
    ui->setupUi(this);
    oldI = -1;
    init();
    generateMenu();
}

//从文件当中加载菜单数据
void MenuLayout::init()
{
    //从文件加载数据
    Util::getInstance()->getMenu(buttons, this);
}

/*
 * 清除界面上面的所有组件
 */
void MenuLayout::clear()
{
    QList<QPushButton *> list= ui->menuWidget->findChildren<QPushButton *>();
    QLayout *layout = ui->menuWidget->layout();
    if( !layout )
    {
        return;
    }
    foreach(QPushButton *btn, list)
    {
        layout->removeWidget(btn);
    }
}

/*
 * 根据菜单数据生成底部的菜单按钮
 */
void MenuLayout::generateMenu()
{
    //先清除界面上面的所有组件
    clear();
    QHBoxLayout *layout = static_cast<QHBoxLayout *>(ui->menuWidget->layout());
    if( layout == NULL)
    {
        layout = new QHBoxLayout(ui->menuWidget);
        layout->setContentsMargins(0, 0, 0, 0);
        layout->setMargin(0);
        layout->setSpacing(0);
        ui->menuWidget->setLayout(layout);
    }
    for(int i = 0; i < LENGTH; i++)
    {
        //如果没有菜单数据
        if( buttons[i].btn == NULL)
        {
            if( addBtn == NULL)
            {
                addBtn = new QPushButton("+");
                connect(addBtn, SIGNAL(clicked()), this, SLOT(addBtn_slot()));
            }
            layout->addWidget(addBtn);
            break;
        } else {
            layout->addWidget(buttons[i].btn);

        }

    }

}

/*
 * 清除ui->subWidget里面的子菜单界面
 */
void MenuLayout::clearSubMenu(int i)
{
    QVBoxLayout *layout = static_cast<QVBoxLayout *>(ui->subWidget->layout());
    //判断layout是否可用，i是否合理，否则使用buttons将会发生数组越界
    if( !layout || i < 0 || i > 2)
    {
        return;
    }
    for(int j = 0; j < SUB_LENGTH; j++)
    {
        layout->removeWidget(buttons[i].subBtns[j]);
        delete buttons[i].subBtns[j];
        buttons[i].subBtns[j] = NULL;
    }
    //删除addSubBtn
    layout->removeWidget(addSubBtn);
    delete addSubBtn;
    addSubBtn = NULL;
}

/*
 * 根据选中的基层按钮生成子按钮
 */
void MenuLayout::generateSubMenu(int i)
{
    //清除SubMenuPanel
    clearSubMenu(oldI);
    Util::getInstance()->refreshMenu(i, buttons);
    int j = 0;
    QLayout *layout = ui->subWidget->layout();
    if( !layout )
    {
        QVBoxLayout * lay = new QVBoxLayout;
        lay->setContentsMargins(30, 0, 30, 0);
        lay->setSpacing(0);
        lay->setMargin(30);
        ui->subWidget->setLayout(lay);
    }
    for(j = 0; j < SUB_LENGTH; j++)
    {
        QVBoxLayout *tempLay = static_cast<QVBoxLayout *>(ui->subWidget->layout());
        if( !buttons[i].subBtns[j])
        {
            if( !addSubBtn )
            {
                addSubBtn = new QPushButton("+");
                //TODO 在此处给按钮连接信号和槽
            }
            tempLay->addWidget(addSubBtn);
            break;
        } else {
            tempLay->addWidget(buttons[i].subBtns[j]);
        }

    }
    //如果第一个子菜单为空，那么没有子菜单
    if( !buttons[i].subBtns[0] )
    {
qDebug() << "buttons[i].subBtn[0]:" << buttons[i].subBtns[1];
         widget->getMenuContent()->hideContent();
    }

}

//添加按钮的槽函数
void MenuLayout::addBtn_slot()
{
    int i =0;
    for(i = 0; i < LENGTH; i++)
    {
        if( buttons[i].btn == NULL)
            break;
    }
    if( i >= 3)
    {
        return;
    }
    buttons[i].btn = new MyButton(QString("菜单%1").arg(i + 1), i, 0);
    //连接信号和槽
    connect(buttons[i].btn, SIGNAL(myCoord(int,int)), this, SLOT(select_menu_slot(int,int)));
    generateMenu();
}

//选中按钮的槽函数
void MenuLayout::select_menu_slot(int i, int j)
{
    //保存上一个选中的菜单的坐标
    oldI = widget->getMenuContent()->getCoordI();
    widget->getMenuContent()->showContent();
    //如果选中的为最底部按钮
    if( !j )
    {
        //TODO 添加里面的所有子菜单
        generateSubMenu(i);
    }
    widget->getMenuContent()->setCoord(i, j);
}

void MenuLayout::setWidgetQuote(Widget *widget)
{
    this->widget = widget;
}

void MenuLayout::deleteMenu(int i, int j)
{
    if( j != 0)
    {
        delete buttons[i].subBtns[j];
        buttons[i].subBtns[j] = NULL;
    } else
    {
        if( buttons[i].subBtns[0] != NULL)
        {
            QMessageBox::warning(NULL, QString("警告"), QString("不能删除有子菜单的菜单!"));
            clearSubMenu(i);
            return;
        }
        delete buttons[i].btn;
        buttons[i].btn = NULL;
        //删除按钮之后将所有按钮往前移动
        for(int x = i; x  < LENGTH - 1; x ++)
        {
            buttons[x].btn = buttons[x + 1].btn;
            if( buttons[x].btn != NULL)
                buttons[x].btn->setCoord(x, 0);
            //移动子菜单
            for(int y = 0; y < SUB_LENGTH; y++)
            {
                buttons[x].subBtns[y] = buttons[x+1].subBtns[y];
                if( buttons[x].subBtns[y] != NULL)
                    buttons[x].subBtns[y]->setCoord(x, y);
            }
        }
        //TODO 移动之后的地方赋值为空
        buttons[LENGTH - 1].btn = NULL;
        for( int y = 0; y < SUB_LENGTH; y++)
        {
            buttons[LENGTH - 1].subBtns[y] = NULL;
        }
    }
    //重新生成菜单
    generateMenu();
}

/*
 * 生成菜单到文件
 */
void MenuLayout::writeMenuToFile()
{
    //Util::getInstance()->writeDataToFile(buttons);
}

MenuLayout::~MenuLayout()
{
    delete ui;
    //释放资源
    delete addBtn;
    addBtn = NULL;
    //释放添加子菜单按钮
    delete addSubBtn;
    addSubBtn = NULL;
    delete subMenuPanel;
    subMenuPanel = NULL;
    //释放buttons
    for(int i =0; i < LENGTH; i++)
    {
        for(int j = 0; j < SUB_LENGTH; j++)
        {
            if( buttons[i].subBtns[j] != NULL)
            {
                delete buttons[i].subBtns[j];
                buttons[i].subBtns[j] = NULL;
            }
        }
        if( buttons[i].btn != NULL)
        {
            delete buttons[i].btn;
            buttons[i].btn = NULL;
        }

    }
}
