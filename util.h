#ifndef UTIL_H
#define UTIL_H

#include <QObject>
#include "menulayout.h"

/*
 * 公共类
 */

class ButtonStruct;

class Util : public QObject
{
    Q_OBJECT
public:
    static Util *getInstance();

    ButtonStruct *getMenuDataFromFile();

    void getMenu(ButtonStruct *b, MenuLayout *menuLayout);
private:
    Util();


private:
    static Util *instance;         //单例
    ButtonStruct buttons[LENGTH];

};

#endif // UTIL_H
