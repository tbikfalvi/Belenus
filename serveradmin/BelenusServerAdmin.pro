# #====================================================================================
# #
# # Belenus Server Admin alkalmazas © Pagony Multimedia Studio Bt - 2010
# #
# #====================================================================================
TARGET = BelenusServerAdmin
TEMPLATE = app
DESTDIR = ..
win32:DEFINES -= UNICODE
CONFIG += qt
CONFIG += console
TRANSLATIONS = bsa_us.ts \
    bsa_hu.ts
HEADERS += ../framework/dbconnection.h \
    ../framework/qtlogger.h \
    ../framework/qtmysqlconnection.h \
    ../framework/qtmysqlquerymodel.h \
    ../framework/sevexception.h \
    ../framework/tracer.h \
    mainwindow.h \
    ../framework/network/connection.h \
    ../framework/network/packet.h \
    adminClientThread.h \
    preferences.h \
    ../framework/preferences.h \
    ../framework/network/protocolException.h
SOURCES += ../framework/dbconnection.cpp \
    ../framework/qtlogger.cpp \
    ../framework/qtmysqlconnection.cpp \
    ../framework/qtmysqlquerymodel.cpp \
    ../framework/tracer.cpp \
    main.cpp \
    mainwindow.cpp \
    adminClientThread.cpp \
    ../framework/network/connection.cpp \
    ../framework/network/packet.cpp \
    ../framework/preferences.cpp
QT += network
QT += sql
FORMS += mainwindow.ui
