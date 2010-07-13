# #====================================================================================
# #
# # Belenus Server alkalmazas (c) Pagony Multimedia Studio Bt - 2010
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
DEFINES += SERVER
CONFIG += qt
CONFIG += console
TRANSLATIONS = bs_us.ts \
    bs_hu.ts
HEADERS += ../framework/dbconnection.h \
    ../framework/qtmysqlconnection.h \
    ../framework/sevexception.h \
    preferences.h \
    ../framework/network/packet.h \
    ../framework/network/connection.h \
    serverthread.h \
    main.h \
    ../framework/preferences.h \
    ../framework/network/protocolException.h \
    ../framework/network/sqlResult.h \
    qtlogger.h
SOURCES += ../framework/dbconnection.cpp \
    ../framework/qtmysqlconnection.cpp \
    main.cpp \
    serverthread.cpp \
    ../framework/network/connection.cpp \
    ../framework/network/packet.cpp \
    ../framework/preferences.cpp \
    ../framework/network/sqlResult.cpp \
    qtlogger.cpp
QT += network
QT += sql
QT -= gui
