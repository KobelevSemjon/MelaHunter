#-------------------------------------------------
#
# Project created by QtCreator 2017-11-01T15:24:20
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Melaroi
TEMPLATE = app

include(..\..\..\common.pri)

SOURCES += main.cpp\
    widget.cpp

HEADERS  += widget.h \
    imageview.h

FORMS    += widget.ui
