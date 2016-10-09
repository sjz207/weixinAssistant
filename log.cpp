#include "log.h"
#include <QFile>
#include <QTextStream>

Log::Log(QObject *parent) :
    QObject(parent)
{
}

void Log::log(QString msg)
{
    QFile  file("log.txt");
    if( !file.open(QIODevice::WriteOnly | QIODevice::Append))
    {
        return;
    }
    QTextStream out(&file);
    out << msg << "\n";
    file.close();
}
