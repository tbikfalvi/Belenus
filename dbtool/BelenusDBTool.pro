# -------------------------------------------------
# Project created by QtCreator 2010-09-12T14:55:28
# -------------------------------------------------
QT += core \
    gui
TARGET = BelenusDBTool
TEMPLATE = app
SOURCES += main.cpp \
    cdlgmain.cpp \
    ../framework/tracer.cpp \
    ../framework/qtmysqlquerymodel.cpp \
    ../framework/qtmysqlconnection.cpp \
    ../framework/qtlogger.cpp \
    ../framework/dbconnection.cpp
HEADERS += cdlgmain.h \
    ../framework/tracer.h \
    ../framework/qtmysqlquerymodel.h \
    ../framework/qtmysqlconnection.h \
    ../framework/qtlogger.h \
    ../framework/dbconnection.h
FORMS += cdlgmain.ui
DESTDIR = ..

QT += network
QT += sql

