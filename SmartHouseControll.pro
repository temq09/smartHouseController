#-------------------------------------------------
#
# Project created by QtCreator 2014-04-28T13:35:00
#
#-------------------------------------------------

QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = SmartHouseControll
TEMPLATE = app


SOURCES += main.cpp\
        startwindow.cpp \
    modernpushbutton.cpp \
    relaycontroller.cpp \
    relaycontrollerserver.cpp

HEADERS  += startwindow.h \
    modernpushbutton.h \
    relaycontroller.h \
    relaycontrollerserver.h

FORMS    += startwindow.ui \
    relaycontroller.ui
