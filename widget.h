#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>

namespace Ui {
class Widget;
}

class MenuLayout;
class MenuContent;

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();

    void init();

    MenuContent *getMenuContent();

    MenuLayout *getMenuLayout();

private:
    Ui::Widget  *ui;
    MenuLayout  *menuLayout;
    MenuContent *menuContent;
};

#endif // WIDGET_H
