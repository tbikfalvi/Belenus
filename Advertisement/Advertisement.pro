
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

QT          += core gui sql
CONFIG      += qt
RESOURCES   += advertisement.qrc
RC_FILE      = advertisement.rc
TEMPLATE     = app
TARGET       = Advertisement
DESTDIR      = ..
TRANSLATIONS = advertisement_hu.ts \
               advertisement_de.ts \
               advertisement_it.ts

HEADERS     += advertisementwindow.h

SOURCES     += main.cpp\
               advertisementwindow.cpp

FORMS       += advertisementwindow.ui
