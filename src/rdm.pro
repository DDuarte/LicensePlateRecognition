#-------------------------------------------------
#
# Project created by QtCreator 2011-01-14T15:26:51
#
#-------------------------------------------------

QT       += core gui sql

TARGET = rdm
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    dbviewer.cpp \
    RegisterPlate.cpp

HEADERS  += mainwindow.h \
    dbviewer.h \
    connection.h \
    RegisterPlate.h

LIBS        += C:/OpenCV2.2/lib/opencv_core220d.lib \
            C:/OpenCV2.2/lib/opencv_highgui220d.lib \
            C:/OpenCV2.2/lib/opencv_imgproc220d.lib
INCLUDEPATH += C:/OpenCV2.2/include

RESOURCES += resources.qrc

RC_FILE = rdm.rc
