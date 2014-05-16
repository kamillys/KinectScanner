#-------------------------------------------------
#
# Project created by QtCreator 2014-05-11T19:49:59
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = KinectScanner
TEMPLATE = app

#### Kinect SDK
#KINECTSDK = $$quote("$(KINECTSDK10_DIR)")
#KINECTSDK = "C:/Program Files/Microsoft SDKs/Kinect/v1.8"
KINECTSDK="C:\\Program Files\\Microsoft SDKs\\Kinect\\v1.8\\"

INCLUDEPATH += $$quote("$$KINECTSDK/inc")

LIBS += -L$$quote($$KINECTSDK\\lib\\amd64) -lKinect10
DEPENDPATH += $$quote("$$KINECTSDK/inc")

SOURCES += main.cpp\
        mainwindow.cpp \
    kinectdevice.cpp \
    kinectdrawwidget.cpp

HEADERS  += mainwindow.h \
    kinectdevice.h \
    kinectdrawwidget.h

FORMS    += mainwindow.ui

#### OpenCV


INCLUDEPATH += $$PWD/inc_opencv
DEPENDPATH += $$PWD/inc_opencv
#OPENCV_LIBDIR = $$quote($$PWD/lib_opencv)
#OPENCV_LIBDIR = "C:/Users/Optimus Prime/KinectScanner/lib_opencv"
OPENCV_LIBDIR = "C:\Users\Optimus Prime\Downloads\opencv\build\x86\vc11\lib"

#win32:CONFIG(release, debug|release): {
 #LIBS += -L$$OPENCV_LIBDIR -lopencv_calib3d248 -lopencv_contrib248 -lopencv_core248 -lopencv_features2d248 -lopencv_flann248 -lopencv_highgui248 -lopencv_imgproc248 -lopencv_legacy248 -lopencv_ml248 -lopencv_nonfree248 -lopencv_objdetect248 -lopencv_video248 -lopencv_videostab248
 LIBS += -L$$OPENCV_LIBDIR -lopencv_core248
#} else:win32:CONFIG(debug, debug|release): {
 #LIBS += -L$$OPENCV_LIBDIR -lopencv_calib3d248d -lopencv_contrib248d -lopencv_core248d -lopencv_features2d248d -lopencv_flann248d -lopencv_highgui248d -lopencv_imgproc248d -lopencv_legacy248d -lopencv_ml248d -lopencv_nonfree248d -lopencv_objdetect248d -lopencv_video248d -lopencv_videostab248d
 #LIBS += -L$$OPENCV_LIBDIR -lopencv_core248d
#}
