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

Util *Util::instance = NULL;

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

ButtonStruct *Util::getMenuDataFromFile()
{
    QFile file("data.json");
    if( !file.open(QIODevice::ReadOnly | QIODevice::Truncate))
    {
        //弹框警告
        QMessageBox::warning(NULL, QString("警告"), QString("文件打开失败"));
        return NULL;
    }
    QByteArray contents = file.readAll();
//qDebug() << "content:" << contents;
    //开始解析json文件
    QJsonParseError json_error;
    QJsonDocument parse_document = QJsonDocument::fromJson(contents, &json_error);
//    ButtonStruct buttons[LENGTH];
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
//qDebug() << "root:" << root;
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
                        QJsonArray subBtn = sObj.take("sub_button").toArray();
                        int subCount = subBtn.count();
                        for(int j = 0; j < subCount; j++)
                        {
                            QJsonObject menuObj = subBtn.at(j).toObject();
                            MyButton *mBtn = new MyButton;
                            //设置坐标
                            mBtn->setCoord(i, j+1);
                            QString s_name, s_url, s_type;
                            int s_key;
                            if( menuObj.contains("name"))
                            {
                                s_name = menuObj.take("name").toString();
                                mBtn->setName(s_name);
                                mBtn->setText(s_name);
                            }
                            if( menuObj.contains("type"))
                            {
                                s_type = menuObj.take("type").toString();
                                mBtn->setType(s_type);
                            }
                            if( menuObj.contains("url"))
                            {
                                s_url = menuObj.take("url").toString();
                                mBtn->setUrl(s_url);
                            }
                            if( menuObj.contains("key"))
                            {
                                s_key = menuObj.take("key").toInt();
                                mBtn->setKey(s_key);
                            } else
                            {
                                mBtn->setKey(NO_KEY);
                            }
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
//qDebug() << "menuData:" << menuData;
    return buttons;
}


void Util::getMenu(ButtonStruct *b, MenuLayout *menuLayout)
{
    //生成菜单数据
    ButtonStruct *b1 = getMenuDataFromFile();
    //将生成的菜单数据复制到menuLayout里面的变量里面去
    if( !b1 )
    {
        //TODO 文件解析出错，异常处理
    }
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

/*
 * 刷新按钮菜单
 */
void Util::refreshMenu(int i, ButtonStruct *b)
{
    QJsonValue value = menuData.at(i);

    if( value.isNull() )
    {
        QMessageBox::warning(NULL, QString("警告"), QString("刷新菜单失败!"));
        return;
    }
    QJsonObject obj = value.toObject();
    if( obj.contains("sub_button") && b[i].btn && !b[i].subBtns[0])
    {
        QJsonArray array = obj.take("sub_button").toArray();
        for(int j = 0; j < array.count(); j++)
        {
            QJsonObject s_obj = array.at(j).toObject();
            MyButton *mBtn = getButtonFromJson(s_obj);
            mBtn->setCoord(i, j+1);
            b[i].subBtns[j] = mBtn;

        }
    }

}

MyButton *Util::getButtonFromJson(QJsonObject &obj)
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
        key = obj.take("key").toInt();
        mBtn->setKey(key);
    } else {
        mBtn->setKey(NO_KEY);
    }
    return mBtn;
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
    QFile file("test.json");
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
        obj.insert("key", btn->getKey());
    }
}

/*
 * 析构器
 */
Util::~Util()
{

}
