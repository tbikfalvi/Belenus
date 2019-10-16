
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

QT          += core gui sql xml
CONFIG      += qt
RESOURCES   += advertisement.qrc
RC_FILE      = advertisement.rc
TEMPLATE     = app
TARGET       = Advertisement
DESTDIR      = ..
TRANSLATIONS = advertisement_hu.ts \
               advertisement_de.ts \
               advertisement_it.ts \
               advertisement_ro.ts

HEADERS     += ../language/language.h \
               advertisementwindow.h

SOURCES     += ../language/language.cpp \
               main.cpp\
               advertisementwindow.cpp

FORMS       += advertisementwindow.ui
