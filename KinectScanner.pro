#-------------------------------------------------
#
# Project created by QtCreator 2014-05-11T19:49:59
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = KinectScanner
TEMPLATE = app

#KINECTSDK = $$quote("$(KINECTSDK10_DIR)")
#KINECTSDK = "C:/Program Files/Microsoft SDKs/Kinect/v1.8"
KINECTSDK="C:\\Program Files\\Microsoft SDKs\\Kinect\\v1.8\\"

INCLUDEPATH += $$quote("$$KINECTSDK/inc")

LIBS += -L$$quote($$KINECTSDK\\lib\\amd64) -lKinect10
DEPENDPATH += $$quote($$KINECTSDK\\lib\\amd64)

SOURCES += main.cpp\
        mainwindow.cpp \
    kinectdevice.cpp \
    kinectdrawwidget.cpp

HEADERS  += mainwindow.h \
    kinectdevice.h \
    kinectdrawwidget.h

FORMS    += mainwindow.ui
