#ifndef LOG_H
#define LOG_H

#include <QObject>

/*
 * qt日志类
 */
class Log : public QObject
{
    Q_OBJECT
public:
    explicit Log(QObject *parent = 0);

    static void log(QString msg);
signals:

public slots:

};

#endif // LOG_H
