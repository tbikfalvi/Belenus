# #====================================================================================
# #
# # Belenus Server alkalmazas © Pagony Multimedia Studio Bt - 2010
# #
# #====================================================================================
# #
# # Filename : BelenusServer.pro
# # AppVersion : 1.0
# # FileVersion : 1.0
# # Author : Bikfalvi Tamas
# #
# #====================================================================================
# # Belenus szerver projekt allomany
# #====================================================================================
TARGET = BelenusServer
TEMPLATE = app
DESTDIR = ..
win32:DEFINES -= UNICODE
CONFIG += qt
CONFIG += console
TRANSLATIONS = bs_us.ts \
    bs_hu.ts
HEADERS += ../framework/dbconnection.h \
    ../framework/qtlogger.h \
    ../framework/qtmysqlconnection.h \
    ../framework/qtmysqlquerymodel.h \
    ../framework/sevexception.h \
    ../framework/tracer.h \
    preferences.h \
    ../framework/network/packet.h \
    ../framework/network/connection.h \
    serverthread.h \
    main.h \
    ../framework/preferences.h \
    ../framework/network/protocolException.h \
    ../framework/network/sqlResult.h
SOURCES += ../framework/dbconnection.cpp \
    ../framework/qtlogger.cpp \
    ../framework/qtmysqlconnection.cpp \
    ../framework/qtmysqlquerymodel.cpp \
    ../framework/tracer.cpp \
    main.cpp \
    serverthread.cpp \
    ../framework/network/connection.cpp \
    ../framework/network/packet.cpp \
    ../framework/preferences.cpp \
    ../framework/network/sqlResult.cpp
QT += network
QT += sql
