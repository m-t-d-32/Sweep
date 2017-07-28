#-------------------------------------------------
#
# Project created by QtCreator 2017-07-23T09:27:07
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Sweep
TEMPLATE = app


SOURCES += main.cpp\
        game.cpp \
    selects.cpp \
    rank.cpp

HEADERS  += game.h \
    square.h \
    selects.h \
    rank.h

FORMS    += game.ui \
    selects.ui \
    rank.ui

RESOURCES += \
    resources.qrc
RC_FILE += image.rc
