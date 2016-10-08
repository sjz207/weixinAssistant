#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>

namespace Ui {
class Widget;
}

class MenuLayout;
class MenuContent;
class QPushButton;
class Start;

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();

    void init();

    MenuContent *getMenuContent();

    MenuLayout *getMenuLayout();

    void setStartQuote(Start *s);

signals:

    void closeWindow();

public slots:
    void on_btnClick_slot();

private:
    Ui::Widget  *ui;
    MenuLayout  *menuLayout;
    MenuContent *menuContent;
    QPushButton *btn;
    Start       *s;
};

#endif // WIDGET_H
