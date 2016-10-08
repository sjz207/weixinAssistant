#ifndef START_H
#define START_H
#include "widget.h"
#include <QWidget>

namespace Ui {
class Start;
}

class NetThread;
class QProgressDialog;
class QTimer;


class Start : public QWidget
{
    Q_OBJECT

public:
    explicit Start(QWidget *parent = 0);
    ~Start();

    void writeConfig();
public slots:
    void on_editBtn_slot();

    void on_submitBtn_slot();

    void on_widgetClose_slot();

    void updateProgress();

    void on_finish_slot();

private:
    void init();

private:
    Ui::Start *ui;
    Widget    *widget;
    QString   appId;
    QString   secret;
    NetThread *net;
    QProgressDialog *progDlg;
    QTimer    *timer;
    int currentValue;
};

#endif // START_H
