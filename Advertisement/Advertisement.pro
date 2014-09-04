
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

QT          += core gui sql
CONFIG      += qt
RESOURCES   += belenus.qrc
RC_FILE      = Belenus.rc
TEMPLATE     = app
TARGET       = Advertisement
DESTDIR      = ..
TRANSLATIONS = advertisement_hu.ts

HEADERS     += mainwindow.h

SOURCES     += main.cpp\
               mainwindow.cpp

FORMS       += mainwindow.ui
