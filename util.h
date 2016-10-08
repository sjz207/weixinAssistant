#ifndef UTIL_H
#define UTIL_H

#include <QObject>
#include "menulayout.h"
#include <QJsonArray>

class ButtonStruct;
class QNetworkAccessManager;
class QNetworkReply;

#define ACCESS_URL "https://api.weixin.qq.com/cgi-bin/token?grant_type=client_credential&appid=APPID&secret=APPSECRET"

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

    void httpRequest(QString url, QString method, QString outputStr);

    QString getMsg(QString json, QString key);

    ~Util();

public slots:
    void on_httpRequest_slot(QNetworkReply *);

private:
    Util();

    void insertObj(QJsonObject &obj, MyButton *btn);

private:
    static Util *instance;         //单例
    ButtonStruct buttons[LENGTH];
    QJsonArray   menuData;         //保存按钮数据

    static QNetworkAccessManager * mNetManager;

};

#endif // UTIL_H
