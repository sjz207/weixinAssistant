#include "mybutton.h"

MyButton::MyButton(QWidget *parent) :
    QPushButton(parent)
{
    i = 0;
    j = 0;
}

MyButton::MyButton(const QString &text, QWidget *parent) :
    QPushButton(text, parent)
{
    i = 0;
    j = 0;
}

MyButton::MyButton(const QString &text,int i, int j, QWidget *parent) :
    QPushButton(text, parent)
{
    this->i = i;
    this->j = j;
    this->key = NO_KEY;
    connect(this, SIGNAL(clicked()), this, SLOT(on_click_slot()));
}


void MyButton::on_click_slot()
{
    emit myCoord(i, j);
}

void MyButton::setCoord(int i, int j)
{
    this->i = i;
    this->j = j;
}

void MyButton::setName(const QString &name)
{
    this->name = name;
}

void MyButton::setType(const QString &type)
{
    this->type = type;
}

void MyButton::setKey(int key)
{
    this->key = key;
}

void MyButton::setUrl(const QString &url)
{
    this->url = url;
}

