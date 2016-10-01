#ifndef ADDSUBBUTTON_H
#define ADDSUBBUTTON_H

#include <QPushButton>
#include <QWidget>

class AddSubButton : public QPushButton
{
    Q_OBJECT
public:
    explicit AddSubButton(QWidget *parent = 0);

    AddSubButton(const QString &text, int i, QWidget *parent = 0);

    void setI(int i);

signals:

    void coordI(int i);

public slots:
    void on_click_slot();

private :
    int i;
};

#endif // ADDSUBBUTTON_H
