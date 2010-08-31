# #====================================================================================
# #
# # Belenus Report Viewer alkalmazas (c) Pagony Multimedia Studio Bt - 2010
# #
# #====================================================================================
TARGET = BelenusReportViewer
TEMPLATE = app
DESTDIR = ..
win32:DEFINES -= UNICODE
CONFIG += qt

# CONFIG += console
TRANSLATIONS = brv_us.ts \
    brv_hu.ts
FORMS = wndmain.ui \
    dlgdemo.ui \
    dlgpreview.ui
HEADERS = ../framework/dbconnection.h \
    ../framework/qtmysqlconnection.h \
    ../framework/qtmysqlquerymodel.h \
    ../framework/qtlogger.h \
    ../framework/tracer.h \
    wndmain.h \
    dlgdemo.h \
    dlgpreview.h
SOURCES = ../framework/dbconnection.cpp \
    ../framework/qtmysqlconnection.cpp \
    ../framework/qtmysqlquerymodel.cpp \
    ../framework/qtlogger.cpp \
    ../framework/tracer.cpp \
    main.cpp \
    wndmain.cpp \
    dlgdemo.cpp \
    dlgpreview.cpp
QT += network
QT += sql
