#-------------------------------------------------
#
# Project created by QtCreator 2014-03-29T11:33:32
#
#-------------------------------------------------

QT          += core gui sql
DESTDIR      = ..
TARGET       = BelenusUpdater
TEMPLATE     = app
RC_FILE      = BelenusUpdater.rc
RESOURCES   += BelenusUpdater.qrc
TRANSLATIONS = updater_us.ts \
               updater_hu.ts

SOURCES += ../framework/dbconnection.cpp \
           ../framework/qtmysqlconnection.cpp \
           ../framework/qtmysqlquerymodel.cpp \
           ../framework/qtlogger.cpp \
           ../framework/logger/FileWriter.cpp \
           ../framework/tracer.cpp \
           main.cpp\
           mainwindow.cpp

HEADERS += ../framework/dbconnection.h \
           ../framework/qtmysqlconnection.h \
           ../framework/qtmysqlquerymodel.h \
           ../framework/qtlogger.h \
           ../framework/logger/FileWriter.h \
           ../framework/tracer.h \
           BelenusUpdater.h \
           mainwindow.h

FORMS   += mainwindow.ui
