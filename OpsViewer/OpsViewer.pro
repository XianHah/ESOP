#-------------------------------------------------
#
# Project created by QtCreator 2019-10-11T14:19:58
#
#-------------------------------------------------

QT       += core gui
QT+=websockets
QT+=network
QT+=sql
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = OpsViewer
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += \
        main.cpp \
        OpsViewer.cpp \
    AnimationButton.cpp \
    LoginServer.cpp \
    ArcFaceEngine.cpp \
    Register.cpp \
    BatchRegister.cpp \
    FaceDetect.cpp \
    FaceDetect.cpp \
    analysis.cpp \
    GetFeaFromServer.cpp \
    Warning.cpp \
    CountThread.cpp \
    counter.cpp \
    filter.cpp \
    trigger.cpp

HEADERS += \
        OpsViewer.h \
    AnimationButton.h \
    LoginServer.h \
    amcomdef.h \
    ArcFaceEngine.h \
    arcsoft_face_sdk.h \
    asvloffscreen.h \
    merror.h \
    Register.h \
    BatchRegister.h \
    FaceDetect.h \
    FaceDetect.h \
    analysis.h \
    GetFeaFromServer.h \
    Warning.h \
    CountThread.h \
    counter.h \
    filter.h \
    trigger.h



FORMS += \
        OpsViewer.ui \
    LoginServer.ui \
    register.ui \
    Warning.ui

RESOURCES += \
    res.qrc

INCLUDEPATH += $$PWD/   \
               $$PWD/opencv\
               $$PWD/opencv2


 LIBS+=     $$PWD/lib/libarcsoft_face_engine.lib \
            $$PWD/lib/opencv_core249.lib \
            $$PWD/lib/opencv_core249d.lib\
            $$PWD/lib/opencv_highgui249.lib\
            $$PWD/lib/opencv_highgui249.lib\
            $$PWD/lib/opencv_highgui249d.lib\
            $$PWD/lib/opencv_imgproc249.lib\
            $$PWD/lib/opencv_imgproc249d.lib
