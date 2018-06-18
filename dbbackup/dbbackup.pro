#-------------------------------------------------
#
# Project created by QtCreator 2014-06-08T13:59:12
#
#-------------------------------------------------

QT          += core gui xml
TARGET       = DBBackup
DESTDIR      = ..
TEMPLATE     = app
RC_FILE      = dbbackup.rc
RESOURCES   += dbbackup.qrc
TRANSLATIONS = dbbackup_hu.ts \
               dbbackup_de.ts \
               dbbackup_it.ts \
               dbbackup_ro.ts

SOURCES     += ../language/language.cpp \
               main.cpp \
               mainwindow.cpp

HEADERS     += ../language/language.h \
               mainwindow.h

FORMS       += mainwindow.ui

