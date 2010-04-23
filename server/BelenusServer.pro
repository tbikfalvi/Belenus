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
HEADERS += ../framework/belenusmessages.h \
    ../framework/dbconnection.h \
    ../framework/qtlogger.h \
    ../framework/qtmysqlconnection.h \
    ../framework/qtmysqlquerymodel.h \
    ../framework/sevexception.h \
    ../framework/tracer.h \
    belenusserver.h \
    bs_main_dialog.h \
    bs_server.h \
    bs_thread.h \
    preferences.h
SOURCES += ../framework/dbconnection.cpp \
    ../framework/qtlogger.cpp \
    ../framework/qtmysqlconnection.cpp \
    ../framework/qtmysqlquerymodel.cpp \
    ../framework/tracer.cpp \
    bs_main.cpp \
    bs_main_dialog.cpp \
    bs_server.cpp \
    bs_thread.cpp \
    preferences.cpp
QT += network
QT += sql
