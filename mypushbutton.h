#ifndef MYPUSHBUTTON_H
#define MYPUSHBUTTON_H
#include <QPushButton>

class MyPushButton : public QPushButton
{
    Q_OBJECT
public:
    explicit MyPushButton(QWidget *parent = 0);

    MyPushButton(const QString & text, QWidget *parent = 0);

public slots:

    void click_slot();

signals:
    void sendCoord(int, int);

private :
    int i, j;

};

#endif // MYPUSHBUTTON_H
