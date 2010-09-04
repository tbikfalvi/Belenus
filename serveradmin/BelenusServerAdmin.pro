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
    ../framework/logger/ConsoleWriter.h \
    ../framework/logger/LogEmitter.h \
    mainwindow.h \
    ../framework/network/CommunicationProtocol.h \
    ../framework/network/packet.h \
    adminClientThread.h \
    preferences.h \
    ../framework/preferences.h \
    ../framework/network/protocolException.h \
    ../framework/network/sqlResult.h
SOURCES += ../framework/dbconnection.cpp \
    ../framework/qtlogger.cpp \
    ../framework/logger/ConsoleWriter.cpp \
    ../framework/qtmysqlconnection.cpp \
    ../framework/qtmysqlquerymodel.cpp \
    ../framework/tracer.cpp \
    main.cpp \
    mainwindow.cpp \
    adminClientThread.cpp \
    ../framework/network/CommunicationProtocol.cpp \
    ../framework/network/packet.cpp \
    ../framework/preferences.cpp \
    ../framework/network/sqlResult.cpp
QT += network
QT += sql
FORMS += mainwindow.ui
