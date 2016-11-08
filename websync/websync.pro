
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

QT          += core gui multimedia
CONFIG      += qt
RESOURCES   += websync.qrc
RC_FILE      = websync.rc
TEMPLATE     = app
DESTDIR      = ..
TRANSLATIONS = websync_hu.ts
TARGET       = websync

HEADERS     += dlgmain.h

SOURCES     += main.cpp\
               dlgmain.cpp

FORMS       += dlgmain.ui
