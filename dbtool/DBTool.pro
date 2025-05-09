#-------------------------------------------------
#
# Project created by QtCreator 2012-05-28T14:01:59
#
#-------------------------------------------------

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets


QT          += core gui sql
CONFIG      += qt
TARGET       = DBTool
TEMPLATE     = app
DESTDIR      = ..
RESOURCES   += DBTool.qrc
TRANSLATIONS = dbtool_hu.ts
RC_FILE      = DBTool.rc

SOURCES     += ../framework/dbconnection.cpp \
               ../framework/qtmysqlconnection.cpp \
               ../framework/qtmysqlquerymodel.cpp \
               ../framework/qtlogger.cpp \
               ../framework/logger/FileWriter.cpp \
               ../framework/tracer.cpp \
               main.cpp \
               mainwindow.cpp \
               dlgprogress.cpp

HEADERS      = ../framework/dbconnection.h \
               ../framework/qtmysqlconnection.h \
               ../framework/qtmysqlquerymodel.h \
               ../framework/qtlogger.h \
               ../framework/logger/FileWriter.h \
               ../framework/tracer.h \
               mainwindow.h \
               dlgprogress.h

FORMS       += mainwindow.ui \
               dlgprogress.ui
