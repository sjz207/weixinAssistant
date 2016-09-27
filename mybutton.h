#ifndef MYBUTTON_H
#define MYBUTTON_H

#include <QPushButton>
#define NO_KEY 0
/*
 * 抽象出来的一层按钮
 */
class MyButton : public QPushButton
{
    Q_OBJECT
public:
    explicit MyButton(QWidget *parent = 0);

    MyButton(const QString &text, QWidget *parent = 0);

    MyButton(const QString &text, int i, int j, QWidget *parent = 0);

    void setCoord(int i, int j);

    void setName(const QString &name);

    void setType(const QString &type);

    void setKey(int key);

    void setUrl(const QString &url);

signals:

    void myCoord(int, int);

public slots:

    void on_click_slot();


private:
    int i;
    int j;
    QString name;
    QString type;
    int key;
    QString url;

};

#endif // MYBUTTON_H
