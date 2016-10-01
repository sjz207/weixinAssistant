#include "addsubbutton.h"
#include <QString>

AddSubButton::AddSubButton(QWidget *parent) :
    QPushButton(parent)
{
}

AddSubButton::AddSubButton(const QString &text,int i, QWidget *parent)
    : QPushButton(text, parent),
      i(i)
{
    connect(this, SIGNAL(clicked()), this, SLOT(on_click_slot()));
}

void AddSubButton::setI(int i)
{
    this->i = i;
}

void AddSubButton::on_click_slot()
{
    emit coordI(this->i);
}
