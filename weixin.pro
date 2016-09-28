#-------------------------------------------------
#
# Project created by QtCreator 2016-08-29T21:35:54
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = weixin
TEMPLATE = app


SOURCES += main.cpp\
        widget.cpp \
    menucontent.cpp \
    menulayout.cpp \
    mybutton.cpp \
    util.cpp

HEADERS  += widget.h \
    menucontent.h \
    menulayout.h \
    mybutton.h \
    util.h

FORMS    += widget.ui \
    menucontent.ui \
    menulayout.ui

RESOURCES += \
    res.qrc
