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
CONFIG += qt
CONFIG += console
TRANSLATIONS = bs_us.ts \
    bs_hu.ts
HEADERS += ../framework/dbconnection.h \
    ../framework/qtmysqlconnection.h \
    ../framework/sevexception.h \
    preferences.h \
    ../framework/network/packet.h \
    ../framework/network/CommunicationProtocol.h \
    serverthread.h \
    main.h \
    ../framework/preferences.h \
    ../framework/network/protocolException.h \
    ../framework/network/sqlResult.h \
    ../framework/qtlogger.h \
    ../framework/logger/ConsoleWriter.h \
    ../framework/logger/DatabaseWriter.h \
    sigc.h
SOURCES += ../framework/dbconnection.cpp \
    ../framework/qtmysqlconnection.cpp \
    main.cpp \
    serverthread.cpp \
    ../framework/network/CommunicationProtocol.cpp \
    ../framework/network/packet.cpp \
    ../framework/preferences.cpp \
    ../framework/network/sqlResult.cpp \
    ../framework/qtlogger.cpp \
    ../framework/logger/ConsoleWriter.cpp \
    ../framework/logger/DatabaseWriter.cpp \
    sigc.cpp
QT += network
QT += sql
QT -= gui
