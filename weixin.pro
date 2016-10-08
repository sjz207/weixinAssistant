#-------------------------------------------------
#
# Project created by QtCreator 2016-08-29T21:35:54
#
#-------------------------------------------------

QT       += core gui\
            network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = weixin
TEMPLATE = app


SOURCES +=\
        widget.cpp \
    menucontent.cpp \
    menulayout.cpp \
    mybutton.cpp \
    util.cpp \
    addsubbutton.cpp \
    main.cpp \
    start.cpp \
    netthread.cpp

HEADERS  += widget.h \
    menucontent.h \
    menulayout.h \
    mybutton.h \
    util.h \
    addsubbutton.h \
    start.h \
    netthread.h

FORMS    += widget.ui \
    menucontent.ui \
    menulayout.ui \
    start.ui

RESOURCES += \
    res.qrc
