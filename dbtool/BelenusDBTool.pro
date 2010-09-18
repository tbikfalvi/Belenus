# -------------------------------------------------
# Project created by QtCreator 2010-09-12T14:55:28
# -------------------------------------------------
QT += core \
    gui
TARGET = BelenusDBTool
TEMPLATE = app
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
HEADERS += cdlgmain.h \
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
FORMS += cdlgmain.ui \
    belenusdbtool.h
DESTDIR = ..

QT += network
QT += sql
