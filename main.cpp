#include "widget.h"
#include <QApplication>
#include "start.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
//    Widget w;
//    w.show();
    Start s;
    s.show();

    return a.exec();
}
