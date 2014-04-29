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
    relaycontrollerserver.cpp \
    videocontroller.cpp \
    videostreamview.cpp \
    dialog_addcamera.cpp \
    mylistview.cpp \
    detectmotion.cpp \
    filemeneger.cpp

HEADERS  += startwindow.h \
    modernpushbutton.h \
    relaycontroller.h \
    relaycontrollerserver.h \
    videocontroller.h \
    videostreamview.h \
    dialog_addcamera.h \
    mylistview.h \
    detectmotion.h \
    filemeneger.h

FORMS    += startwindow.ui \
    relaycontroller.ui \
    videocontroller.ui \
    videostreamview.ui \
    dialog_addcamera.ui

INCLUDEPATH += "d:\\opencv\\build\\include"
LIBS += -l"d:\\opencv\\build\\x86\\vc11\\lib\\*"
