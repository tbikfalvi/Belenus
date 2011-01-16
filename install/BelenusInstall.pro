#-------------------------------------------------
#
# Project created by QtCreator 2010-12-08T21:54:04
#
#-------------------------------------------------

TEMPLATE     = app
DESTDIR      = ..
TARGET       = Setup

CONFIG      += qt
QT          += core gui sql
TRANSLATIONS = setup_us.ts \
               setup_hu.ts

FORMS       += dlgMain.ui

SOURCES     += main.cpp \
               dlgMain.cpp \
               vregistry.cpp \
               vqtconvert.cpp

HEADERS     += dlgMain.h \
               vqtconvert.h \
               vregistry.h

RESOURCES   += install.qrc

