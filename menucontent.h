#ifndef MENUCONTENT_H
#define MENUCONTENT_H

#include <QWidget>
#include "widget.h"

#define NO_COORD -1

namespace Ui {
class MenuContent;
}

class QButtonGroup;

//显示菜单内容的界面
class MenuContent : public QWidget
{
    Q_OBJECT

public:
    explicit MenuContent(QWidget *parent = 0);

    void init();

    ~MenuContent();

    void setWidgetQuote(Widget *widget);

    //当菜单按钮不为空的时候隐藏下面的内容
    void hideContent();

    void showContent();

    void setCoord(int, int);

    int getCoordI();


signals:
    void deleteMenu(int menuId);

public slots:
    void on_btnGroup_slot();
    void on_deleteBtn_slot();

    void sureBtn_slot();

private:
    Ui::MenuContent *ui;
    QButtonGroup    *rBtnGroup;
    Widget          *widget;
    int i;          //当前选中的i坐标
    int j;          //当前选中的j坐标
};

#endif // MENUCONTENT_H
