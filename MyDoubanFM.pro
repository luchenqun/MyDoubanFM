#-------------------------------------------------
#
# Project created by QtCreator 2015-06-29T13:32:20
#
#-------------------------------------------------

QT       += core gui multimedia

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = MyDoubanFM
TEMPLATE = app

QMAKE_CXXFLAGS += -std=c++0x

SOURCES += main.cpp\
        WidgetMain.cpp \
    DownloadControl.cpp \
    Common.cpp \
    WidgetBase.cpp \
    WidgetTitle.cpp \
    PushButton.cpp \
    WidgetPlay.cpp \
    WidgetFunction.cpp

HEADERS  += WidgetMain.h \
    DownloadControl.h \
    Common.h \
    curl/curl.h \
    curl/curlbuild.h \
    curl/curlrules.h \
    curl/curlver.h \
    curl/easy.h \
    curl/mprintf.h \
    curl/multi.h \
    curl/stdcheaders.h \
    curl/typecheck-gcc.h \
    WidgetBase.h \
    WidgetTitle.h \
    PushButton.h \
    WidgetPlay.h \
    WidgetFunction.h

win32: LIBS += -L$$PWD/curl/ -llibcurl

RESOURCES += \
    MyDoubanFM.qrc