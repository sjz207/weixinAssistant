#ifndef MENULAYOUT_H
#define MENULAYOUT_H

#include <QWidget>
#include <QString>
#include "widget.h"

#define LENGTH 3
#define SUB_LENGTH 5

namespace Ui {
class MenuLayout;
}

class QPushButton;
class MyButton;


/*
 * 用来保存按钮的数组
 */
struct ButtonStruct {
    MyButton *subBtns[SUB_LENGTH];
    MyButton *btn;
};

//菜单布局的界面
class MenuLayout : public QWidget
{
    Q_OBJECT

public:
    explicit MenuLayout(QWidget *parent = 0);
    ~MenuLayout();

    void setWidgetQuote(Widget *widget);

    void deleteMenu(int, int);

    void writeMenuToFile();

private :
    void init();

    void generateMenu();

    void generateSubMenu(int i);

    void clear();

    void clearSubMenu(int i);

public slots :
    void addBtn_slot();
    //选中按钮的槽函数
    void select_menu_slot(int i, int j);

private:
    Ui::MenuLayout *ui;
    ButtonStruct   buttons[LENGTH];             //数组指针
    QPushButton    *addBtn;              //菜单数据
    Widget         *widget;              //容器界面的引用
    QWidget        *subMenuPanel;        //用来显示子菜单按钮的面板
    QPushButton    *addSubBtn;
    int            oldI;                 //更改之前的i坐标

};

#endif // MENULAYOUT_H
