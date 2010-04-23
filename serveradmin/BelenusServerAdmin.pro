# #====================================================================================
# #
# # Belenus Server Admin alkalmazas © Pagony Multimedia Studio Bt - 2010
# #
# #====================================================================================
# #
# # Filename : BelenusServerAdmin.pro
# # AppVersion : 1.0
# # FileVersion : 1.0
# # Author : Bikfalvi Tamas
# #
# #====================================================================================
# # Belenus szerver projekt allomany
# #====================================================================================
TARGET = BelenusServerAdmin
TEMPLATE = app
DESTDIR = ..
win32:DEFINES -= UNICODE
CONFIG += qt
CONFIG += console
TRANSLATIONS = bsa_us.ts \
    bsa_hu.ts
HEADERS += ../framework/belenusmessages.h \
    ../framework/dbconnection.h \
    ../framework/qtlogger.h \
    ../framework/qtmysqlconnection.h \
    ../framework/qtmysqlquerymodel.h \
    ../framework/sevexception.h \
    ../framework/tracer.h \
    belenusserveradmin.h \
    dlg_mainwindow.h \
    preferences.h
SOURCES += ../framework/dbconnection.cpp \
    ../framework/qtlogger.cpp \
    ../framework/qtmysqlconnection.cpp \
    ../framework/qtmysqlquerymodel.cpp \
    ../framework/tracer.cpp \
    bsa_main.cpp \
    dlg_mainwindow.cpp \
    preferences.cpp
QT += network
QT += sql
FORMS += bsa_mainwindow.ui
