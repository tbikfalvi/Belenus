#-------------------------------------------------
#
# Project created by QtCreator 2014-06-08T13:59:12
#
#-------------------------------------------------

QT          += core gui sql xml
TARGET       = DBBackup
DESTDIR      = ..
TEMPLATE     = app
RC_FILE      = dbbackup.rc
RESOURCES   += dbbackup.qrc
TRANSLATIONS = dbbackup_hu.ts \
               dbbackup_de.ts \
               dbbackup_it.ts \
               dbbackup_ro.ts

SOURCES     += ../framework/dbconnection.cpp \
               ../framework/qtmysqlconnection.cpp \
               ../framework/qtmysqlquerymodel.cpp \
               ../framework/qtlogger.cpp \
               ../framework/logger/FileWriter.cpp \
               ../framework/tracer.cpp \
               ../language/language.cpp \
               main.cpp \
               mainwindow.cpp

HEADERS     += ../framework/dbconnection.h \
               ../framework/qtmysqlconnection.h \
               ../framework/qtmysqlquerymodel.h \
               ../framework/qtlogger.h \
               ../framework/logger/FileWriter.h \
               ../framework/tracer.h \
               ../language/language.h \
               mainwindow.h

FORMS       += mainwindow.ui

