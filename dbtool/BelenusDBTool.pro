# -------------------------------------------------
# Project created by QtCreator 2010-09-12T14:55:28
# -------------------------------------------------

TEMPLATE = app

FORMS = cdlgmain.ui

HEADERS = belenusdbtool.h \
    ../framework/dbconnection.h \
    ../framework/qtframework.h \
    ../framework/qtlogger.h \
    ../framework/logger/DatabaseWriter.h \
    ../framework/logger/GUIWriter.h \
    ../framework/logger/ConsoleWriter.h \
    ../framework/qtmysqlconnection.h \
    ../framework/qtmysqlquerymodel.h \
    ../framework/sevexception.h \
    ../framework/tracer.h \
    cdlgmain.h

SOURCES += main.cpp \
    ../framework/dbconnection.cpp \
    ../framework/qtlogger.cpp \
    ../framework/logger/DatabaseWriter.cpp \
    ../framework/logger/GUIWriter.cpp \
    ../framework/logger/ConsoleWriter.cpp \
    ../framework/qtmysqlconnection.cpp \
    ../framework/qtmysqlquerymodel.cpp \
    ../framework/tracer.cpp \
    cdlgmain.cpp

DESTDIR = ..
QT += sql
QT += network
CONFIG += qt
CONFIG += console
win32:DEFINES -= UNICODE 
