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
    items/kit.cpp \
    items/kitpiece.cpp \
    items/preset.cpp \
    utils/util.cpp \
    window_boilerplate.cpp \
    helpers/helperbase.cpp \
    window_signals.cpp \
    window_util.cpp \
    items/item.cpp

HEADERS  += bfd3me.h \
    items/kit.h \
    items/kitpiece.h \
    items/preset.h \
    utils/util.h \
    helpers/helper.h \
    helpers/dbhelper.h \
    utils/boilerplate.h \
    models/itemmodel.h \
    helpers/helperbase.h \
    models/filtermodel.h \
    items/item.h \
    models/selectionmodel.h

FORMS    += bfd3me.ui
