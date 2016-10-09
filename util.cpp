#include "util.h"
#include <QMessageBox>
#include <QFile>
#include <QString>
#include <QDebug>
#include <QJsonParseError>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonValue>
#include "mybutton.h"
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>

Util *Util::instance = NULL;

QNetworkAccessManager * Util::mNetManager = NULL;

Util::Util( )
{
}

/*
 * 单例生成
 */
Util *Util::getInstance()
{
    if( !instance )
        instance = new Util;
    return instance;
}

ButtonStruct *Util::getMenuDataFromFile(MenuLayout *layout)
{
    QFile file("data.json");
    if( !file.open(QIODevice::ReadOnly | QIODevice::Truncate))
    {
        //弹框警告
        QMessageBox::warning(NULL, QString("警告"), QString("文件打开失败"));
        return NULL;
    }
    QByteArray contents = file.readAll();
    //开始解析json文件
    QJsonParseError json_error;
    QJsonDocument parse_document = QJsonDocument::fromJson(contents, &json_error);
    //数组置零
    for(int i = 0; i < LENGTH; i++)
    {
        buttons[i].btn = NULL;
        for(int j = 0; j < SUB_LENGTH; j++)
        {
            buttons[i].subBtns[j] = NULL;
        }
    }
    //如果解析没有出错
    if( json_error.error  == QJsonParseError::NoError)
    {
        if( parse_document.isObject() )
        {
            QJsonObject obj = parse_document.object();
            if( obj.contains("button"))
            {
                QJsonArray root = obj.take("button").toArray();
                //保存菜单数据
                menuData = root;
                int count = root.count();
                for(int i = 0; i < count; i++)
                {
                    if( i == 1)
                    {
                        qDebug() << "breakpoint";
                    }
                    QJsonObject sObj = root.at(i).toObject();
                    QString name;
                    if( sObj.contains("name") )
                    {
                        name = sObj.take("name").toString();
                        buttons[i].btn = new MyButton(name, i, 0);
                        buttons[i].btn->setName(name);
                    }
                    //如果包含子菜单
                    if( sObj.contains("sub_button"))
                    {
                        //因为有子菜单
                        buttons[i].btn->setComplex(true);
                        QJsonArray subBtn = sObj.take("sub_button").toArray();
                        int subCount = subBtn.count();
                        for(int j = 0; j < subCount; j++)
                        {
                            QJsonObject menuObj = subBtn.at(j).toObject();
                            MyButton *mBtn = getButtonFromJson(menuObj, layout);
                            mBtn->setCoord(i, j+1);
                            buttons[i].subBtns[j] = mBtn;
                        }
                    } else
                    {
                        QString m_url, m_type;
                        int m_key;
                        if( sObj.contains("url"))
                        {
                            m_url = sObj.take("url").toString();
                            buttons[i].btn->setUrl(m_url);
                        }
                        if( sObj.contains("type"))
                        {
                            m_type = sObj.take("type").toString();
                            buttons[i].btn->setType(m_type);
                        }
                        if( sObj.contains("key"))
                        {
                            m_key = sObj.take("key").toInt();
                            buttons[i].btn->setKey(m_key);
                        }
                    }

                }
            }
        }
    } else {
        QMessageBox::warning(NULL, QString("警告"), QString("文件解析出错!"));
        return NULL;
    }
    return buttons;
}

QString Util::getMenuData()
{
    QFile file("data.json");
    if( !file.open(QIODevice::ReadOnly | QIODevice::Truncate))
    {
        //弹框警告
        QMessageBox::warning(NULL, QString("警告"), QString("文件打开失败"));
        return NULL;
    }
    QString str(file.readAll());
    return str;
}

void Util::getMenu(ButtonStruct *b, MenuLayout *menuLayout)
{
    //生成菜单数据
    ButtonStruct *b1 = getMenuDataFromFile(menuLayout);
    //将生成的菜单数据复制到menuLayout里面的变量里面去
    if( !b1 )
    {
qDebug() << "空指针异常";
        //TODO 文件解析出错，异常处理
        QFile file("data.json");
        if( !file.exists())
        {
            file.open(QIODevice::WriteOnly);
        }
        file.close();
    } else
    {
        for(int i = 0; i < LENGTH; i++)
        {
            (b+i)->btn = (b1+i)->btn;
            for(int j = 0; j< SUB_LENGTH; j++)
            {
                (b+i)->subBtns[j] = (b1+i)->subBtns[j];
            }
            //给每个基类按钮连接信号和槽
            connect((b+i)->btn, SIGNAL(myCoord(int,int)), menuLayout, SLOT(select_menu_slot(int,int)));
        }
    }
}

/*
 * 刷新按钮菜单
 */
void Util::refreshMenu(int i, ButtonStruct *b, MenuLayout *layout)
{
    QJsonValue value = menuData.at(i);

    if( value.isUndefined() )
    {
        //TODO 代表这个地步菜单是重新添加的
        return;
    }
    QJsonObject obj = value.toObject();
    if( obj.contains("sub_button") && b[i].btn && !b[i].subBtns[0])
    {
        QJsonArray array = obj.take("sub_button").toArray();
        for(int j = 0; j < array.count(); j++)
        {
            QJsonObject s_obj = array.at(j).toObject();
            MyButton *mBtn = getButtonFromJson(s_obj, layout);
            mBtn->setCoord(i, j+1);
            b[i].subBtns[j] = mBtn;
        }
    }

}

MyButton *Util::getButtonFromJson(QJsonObject &obj, MenuLayout *layout)
{
    MyButton *mBtn = new MyButton;
    QString name, url, type;
    int key;
    if( obj.contains("name"))
    {
        name = obj.take("name").toString();
        mBtn->setName(name);
        mBtn->setText(name);
    }
    if( obj.contains("type"))
    {
        type = obj.take("type").toString();
        mBtn->setType(type);
    }
    if( obj.contains("url"))
    {
        url = obj.take("url").toString();
        mBtn->setUrl(url);
    }
    if( obj.contains("key"))
    {
        key = obj.take("key").toString().toInt();
        mBtn->setKey(key);
    } else {
        mBtn->setKey(NO_KEY);
    }
    connect(mBtn, SIGNAL(myCoord(int,int)), layout, SLOT(select_menu_slot(int,int)));
    return mBtn;
}

/*
 * 根据i和j修改menuData
 * 保证这里面的i和j不能无效
 */
void Util::modifyData(int i, int j, ButtonStruct *b)
{
    QJsonObject obj = menuData.takeAt(i).toObject();
    //如果是修改底部菜单按钮
    if( !j )
    {
        //此时的底部按钮没有子按钮
        if( !b[i].subBtns[0])
        {
            obj = createObject(b[i].btn);
            menuData.insert(i, obj);
//qDebug()  << "底部没有按钮!";
        } else
        {
            QJsonArray array = obj.take("sub_button").toArray();
            QJsonObject o;
            o.insert("name", b[i].btn->getName());
            o.insert("sub_button", array);
            menuData.insert(i, o);
        }
//qDebug() << "修改底部按钮!";
    } else
    {
        if( obj.contains("sub_button"))
        {
             QJsonArray array = obj.take("sub_button").toArray();
             array.removeAt(j - 1);
             QJsonObject o = createObject(b[i].subBtns[j - 1]);
             array.insert(j - 1, o);
             obj.insert("sub_button", array);
qDebug() << "sub_button   ......";
        }
        menuData.insert(i, obj);
    }
    refreshData();
}

/*
 * 将不包含子菜单的按钮转换为包含子菜单的按钮
 */
void Util::switchComplexButton(int i, int j, ButtonStruct *b)
{
//qDebug() << "switch complex:" << buttons[i].btn->isComplex();
    QJsonObject obj = menuData.takeAt(i).toObject();
    QJsonObject newObj;
    newObj.insert("name", b[i].btn->getName());
    QJsonObject subObj = createObject(b[i].subBtns[0]);
qDebug() << "b[i].subBtns[0]" << b[i].subBtns[j] << "j:" << j;
    QJsonArray array;
    array.append(subObj);
    newObj.insert("sub_button", array);
    menuData.insert(i, newObj);
    refreshData();
}

/*
 * 添加按钮
 */
void Util::addData(int i, int j, ButtonStruct *b)
{
    //底部按钮
    if( !j )
    {
        QJsonObject obj = createObject(b[i].btn);
        //添加
        menuData.append(obj);
    } else
    {
        QJsonObject obj = menuData.takeAt(i).toObject();
        QJsonArray array = obj.take("sub_button").toArray();
        QJsonObject o = createObject(b[i].subBtns[j - 1]);
        array.append(o);
        obj.insert("sub_button", array);
        menuData.insert(i, obj);
    }
    refreshData();
}

void Util::refreshData()
{
    //将数据写入文件
    QFile file("data.json");
    if( !file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        QMessageBox::warning(NULL, QString("提示"), QString("由于莫名其妙的BUG，更改菜单失败!"));
        return;
    }
    //清楚文件内容
    file.resize(0);
    QTextStream in(&file);
    //设置写入编码
    in.setCodec("UTF-8");
    QJsonObject obj;
    obj.insert("button", menuData);
    QJsonDocument document;
    document.setObject(obj);
    in << document.toJson(QJsonDocument::Compact);
    file.close();
}

/*
 * 删除menuData里面的数据
 */
void Util::deleteData(int i, int j, ButtonStruct *b)
{
    QJsonObject obj = menuData.takeAt(i).toObject();
    //如果不是是底部的按钮
    if( j )
    {
        if( obj.contains("sub_button"))
        {
            QJsonArray array = obj.take("sub_button").toArray();
            array.removeAt(j - 1);
            obj.insert("sub_button", array);
        }
        menuData.insert(i, obj);
    }
    refreshData();
}

/*
 * 构造http请求
 */
void Util::httpRequest(QString url, QString method, QString outputStr)
{
    if( !mNetManager ) {
        mNetManager = new QNetworkAccessManager(this);
    }
    //构造http请求
    QNetworkRequest req;
    QSslConfiguration config;
    config.setPeerVerifyMode(QSslSocket::VerifyNone);
    config.setProtocol(QSsl::TlsV1SslV3);
    req.setSslConfiguration(config);
    req.setUrl(QUrl(url));
    QNetworkReply *reply = NULL;
    if( !method.compare("GET")) {
        reply = mNetManager->get(req);
    }
    if( outputStr != NULL) {
        //使用utf-8上传post请求数据
        //如果使用outputStr.toLatin1()的话，那么微信客户端产生的菜单是乱码
        reply = mNetManager->post(req, outputStr.toUtf8());
    }
qDebug() << "http requst start....";
    connect(mNetManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(on_httpRequest_slot(QNetworkReply*)));
}

void Util::on_httpRequest_slot(QNetworkReply *reply)
{
    QString str(reply->readAll());
qDebug() << "http request content:" << str;
//    QString error = getMsg(str, "errcode");
//qDebug() << "error :" << error;

    emit returnMsg(str);
}

/*
 * 根据MyButton创建Object
 */
QJsonObject Util::createObject(MyButton *btn)
{
    QJsonObject obj;
    QString name, url, type;
    int key;
    name = btn->getName();
    url = btn->getUrl();
    type = btn->getType();
    key = btn->getKey();
    obj.insert("name", name);
    obj.insert("type", type);
    if( key == NO_KEY)
    {
        obj.insert("url", url);
    } else
    {
        obj.insert("key", QString::number(key));
    }
    return obj;
}

/*
 * 写入json数据到文件
 * 不建议使用这个方法，使用这个方法生成菜单文件的时候buttons必须有值，但是我并没有存这个值
 */
void Util::writeDataToFile(ButtonStruct *b)
{
    QJsonObject obj;
    QJsonArray array;
    for(int i = 0; i < LENGTH; i++)
    {
        QJsonObject s_obj;      //buttton
        if( !b[i].btn )
        {
            return;
        }
        s_obj.insert("name", b[i].btn->getName());
        //如果没有子按钮
        if( !b[i].subBtns[0] )
        {
            insertObj(s_obj, b[i].btn);
        } else
        {
            //TODO 子菜单是一个数组
            QJsonArray s_array;                 //sub_button
            for(int j = 0; j < SUB_LENGTH; j++)
            {
                if( !b[i].subBtns[j])
                {
                    return;
                }
                QJsonObject a_obj;
                a_obj.insert("name", b[i].subBtns[j]->getName());
                insertObj(a_obj, b[i].subBtns[j]);
                s_array.insert(j, a_obj);
            }
            s_obj.insert("sub_button", s_array);
        }
        array.insert(i, s_obj);
    }

    obj.insert("button", array);
    QJsonDocument document;
    document.setObject(obj);
    QFile file("data.json");    //打开文件
    if( !file.open(QIODevice::ReadWrite | QIODevice::Text))
    {
        QMessageBox::warning(NULL, QString("警告"), QString("文件打开失败!"));
    }
    QTextStream in(&file);
    in << document.toJson(QJsonDocument::Compact);
    file.close();
}

void Util::insertObj(QJsonObject &obj, MyButton *btn)
{
    obj.insert("type", btn->getType());
    if( btn->getKey() == NO_KEY)
    {
        obj.insert("url", btn->getUrl());
    } else
    {
        obj.insert("key",QString::number(btn->getKey()));
    }
}

/*
 * 从给定的json数据当中获取key值的value值
 */
QString Util::getMsg(QString json, QString key)
{
    QString value;
    QJsonParseError json_error;
    QJsonDocument parse_document = QJsonDocument::fromJson(json.toUtf8(), &json_error);
    if( json_error.error == QJsonParseError::NoError)
    {
        if( parse_document.isObject())
        {
            QJsonObject obj = parse_document.object();
            if( obj.contains(key))
            {
                QJsonValue temp = obj.take(key);
                if( temp.isString() )
                {
                    value = temp.toString();
                } else
                {
                    value = QString::number(temp.toInt());
                }
            }

        }
    }
    return value;

}

/*
 * 析构器
 */
Util::~Util()
{

}
