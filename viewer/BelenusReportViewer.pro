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
FORMS = wndmain.ui \
    dlgprogress.ui

HEADERS = ../framework/dbconnection.h \
    ../framework/qtmysqlconnection.h \
    ../framework/qtmysqlquerymodel.h \
    ../framework/qtlogger.h \
    ../framework/logger/FileWriter.h \
    ../framework/tracer.h \
    wndmain.h \
    currency.h \
    creport.h \
    creporttemplate.h \
    creportdaily.h \
    creportcarddetails.h \
    creportcardinactive.h \
    creportpatientcardtype.h \
    creportledger.h \
    creportcassahistory.h \
    creportpatientcardsell.h \
    creportproducts.h \
    creportpatientcarddebts.h \
    creportproductstatus.h \
    creportproducthistory.h \
    dlgprogress.h

SOURCES = ../framework/dbconnection.cpp \
    ../framework/qtmysqlconnection.cpp \
    ../framework/qtmysqlquerymodel.cpp \
    ../framework/qtlogger.cpp \
    ../framework/logger/FileWriter.cpp \
    ../framework/tracer.cpp \
    main.cpp \
    wndmain.cpp \
    currency.cpp \
    creport.cpp \
    creporttemplate.cpp \
    creportdaily.cpp \
    creportledger.cpp \
    creportcarddetails.cpp \
    creportcardinactive.cpp \
    creportpatientcardtype.cpp \
    creportcassahistory.cpp \
    creportpatientcardsell.cpp \
    creportproducts.cpp \
    creportproductstatus.cpp \
    creportpatientcarddebts.cpp \
    creportproducthistory.cpp \
    dlgprogress.cpp

