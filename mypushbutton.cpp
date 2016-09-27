#include "mypushbutton.h"

MyPushButton::MyPushButton(QWidget *parent) :
    QPushButton(parent)
{
    i = 0;
    j = 0;
}

MyPushButton::MyPushButton(const QString &text, QWidget *parent)
    : QPushButton(text, parent)
{
    i = 0;
    j = 0;
}

void MyPushButton::click_slot()
{
    emit sendCoord(i, j);
}
