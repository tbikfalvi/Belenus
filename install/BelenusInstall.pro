#-------------------------------------------------
#
# Project created by QtCreator 2010-12-08T21:54:04
#
#-------------------------------------------------

TEMPLATE  = app

FORMS    += dlgMain.ui
HEADERS  += dlgMain.h
SOURCES  += main.cpp\
            dlgMain.cpp
DESTDIR   = ..
CONFIG   += qt
TARGET    = Setup
QT       += core gui

TRANSLATIONS = setup_us.ts \
               setup_hu.ts
RESOURCES += \
    install.qrc
