# #====================================================================================
# #
# # Belenus Report Viewer alkalmazas (c) Pagony Multimedia Studio Bt - 2010
# #
# #====================================================================================

TARGET = ReportViewer
TEMPLATE = app
DESTDIR = ..
win32:DEFINES -= UNICODE
CONFIG += qt
QT += network
QT += sql

RC_FILE         = reportviewer.rc
RESOURCES      += reportviewer.qrc
TRANSLATIONS = brv_us.ts \
    brv_hu.ts
FORMS = wndmain.ui

HEADERS = ../framework/dbconnection.h \
    ../framework/qtmysqlconnection.h \
    ../framework/qtmysqlquerymodel.h \
    ../framework/qtlogger.h \
    ../framework/tracer.h \
    wndmain.h \
    currency.h \
    creport.h \
    creportdaily.h \
    creportcardusage.h \
    creportcardinactive.h \
    creportpatientcardtype.h \
    creportledger.h

SOURCES = ../framework/dbconnection.cpp \
    ../framework/qtmysqlconnection.cpp \
    ../framework/qtmysqlquerymodel.cpp \
    ../framework/qtlogger.cpp \
    ../framework/tracer.cpp \
    main.cpp \
    wndmain.cpp \
    currency.cpp \
    creport.cpp \
    creportdaily.cpp \
    creportledger.cpp \
    creportcardusage.cpp \
    creportcardinactive.cpp \
    creportpatientcardtype.cpp
