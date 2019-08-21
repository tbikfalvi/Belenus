#-------------------------------------------------
#
# Project created by QtCreator 2014-06-08T13:59:12
#
#-------------------------------------------------

QT          += core gui xml
TARGET       = IniToDB
DESTDIR      = ..
TEMPLATE     = app
RC_FILE      = initodb.rc
RESOURCES   += initodb.qrc
TRANSLATIONS = initodb_hu.ts

SOURCES     += ../language/language.cpp \
               main.cpp \
               mainwindow.cpp

HEADERS     += ../language/language.h \
               mainwindow.h

FORMS       += mainwindow.ui

