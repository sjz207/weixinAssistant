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
        QMessageBox::warning(NULL, QString("j警告"), QString("文件解析出错!"));
        return NULL;
    }
    return buttons;
}


void Util::getMenu(ButtonStruct *b, MenuLayout *menuLayout)
{
    //生成菜单数据
    ButtonStruct *b1 = getMenuDataFromFile();
//qDebug() << "test.........";
//    for(int i = 0; i < LENGTH; i++)
//    {
//qDebug() << "buttons[" << i<< "].btn:" << b1[i].btn;
//        for (int j = 0; j < SUB_LENGTH; j++)
//        {
//qDebug() << "buttons[" << i<< "].subBtn[" << j << "]:" << b1[i].subBtns[j];
//        }
//    }
//qDebug() << "end test.................";
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
