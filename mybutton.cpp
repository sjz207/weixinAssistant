#include "mybutton.h"
#include <QDebug>

MyButton::MyButton(QWidget *parent) :
    QPushButton(parent)
{
    i = 0;
    j = 0;
    complex = false;
    this->key = NO_KEY;
    connect(this, SIGNAL(clicked()), this, SLOT(on_click_slot()));
}

MyButton::MyButton(const QString &text, QWidget *parent) :
    QPushButton(text, parent)
{
    i = 0;
    j = 0;
    complex = false;
    this->key = NO_KEY;
    connect(this, SIGNAL(clicked()), this, SLOT(on_click_slot()));
}

MyButton::MyButton(const QString &text,int i, int j, QWidget *parent) :
    QPushButton(text, parent)
{
    this->i = i;
    this->j = j;
    complex = false;
    this->key = NO_KEY;
    connect(this, SIGNAL(clicked()), this, SLOT(on_click_slot()));
}

/*
 * 判断是不是带有子类按钮的底部按钮
 */
bool MyButton::isComplex()
{
    return this->complex;
}

void MyButton::setComplex(bool c)
{
//qDebug() << "c :" << c;
    this->complex = c;
}

void MyButton::on_click_slot()
{
    //发射自定义信号
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

QString MyButton::getName()
{
    return this->name;
}

void MyButton::setType(const QString &type)
{
    this->type = type;
}

QString MyButton::getType()
{
    return this->type;
}

void MyButton::setKey(int key)
{
    this->key = key;
}

int MyButton::getKey()
{
    return this->key;
}

void MyButton::setUrl(const QString &url)
{
    this->url = url;
}

QString MyButton::getUrl()
{
    return this->url;
}
