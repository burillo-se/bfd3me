#-------------------------------------------------
#
# Project created by QtCreator 2015-08-10T10:17:13
#
#-------------------------------------------------

QT       += core gui xml

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = BFD3ME
TEMPLATE = app


SOURCES += main.cpp\
        bfd3me.cpp \
    kit.cpp \
    kitpiece.cpp \
    preset.cpp \
    util.cpp \
    window_boilerplate.cpp

HEADERS  += bfd3me.h \
    kit.h \
    kitpiece.h \
    preset.h \
    util.h \
    helper.h \
    dbhelper.h \
    boilerplate.h \
    itemmodel.h

FORMS    += bfd3me.ui
