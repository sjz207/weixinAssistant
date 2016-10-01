#ifndef UTIL_H
#define UTIL_H

#include <QObject>
#include "menulayout.h"
#include <QJsonArray>

class ButtonStruct;

/*
 * 公共类
 */
class Util : public QObject
{
    Q_OBJECT
public:
    static Util *getInstance();

    ButtonStruct *getMenuDataFromFile(MenuLayout *);

    void getMenu(ButtonStruct *b, MenuLayout *menuLayout);

    void refreshMenu(int i, ButtonStruct *b, MenuLayout *);

    MyButton *getButtonFromJson(QJsonObject &, MenuLayout *);

    void writeDataToFile(ButtonStruct *b);

    void modifyData(int, int, ButtonStruct *);

    void deleteData(int, int, ButtonStruct *);

    void addData(int, int, ButtonStruct *);

    void refreshData();

    void switchComplexButton(int, int, ButtonStruct *);

    QJsonObject createObject(MyButton *);

    ~Util();

private:
    Util();

    void insertObj(QJsonObject &obj, MyButton *btn);

private:
    static Util *instance;         //单例
    ButtonStruct buttons[LENGTH];
    QJsonArray   menuData;         //保存按钮数据

};

#endif // UTIL_H
