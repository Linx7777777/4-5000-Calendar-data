QT += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = CalendarData
TEMPLATE = app
CONFIG += c++11 link_pkgconfig
PKGCONFIG += dtkwidget

SOURCES += \
        main.cpp \
    mainwindow.cpp

RESOURCES +=         resources.qrc

HEADERS += \
    mainwindow.h
