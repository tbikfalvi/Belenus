# #====================================================================================
# #
# # Belenus Report Viewer alkalmazas (c) Pagony Multimedia Studio Bt - 2010
# #
# #====================================================================================

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
win32:DEFINES -= UNICODE

QT          += core gui network sql
CONFIG      += qt
TARGET       = ReportViewer
TEMPLATE     = app
DESTDIR      = ..
RESOURCES   += reportviewer.qrc
TRANSLATIONS = brv_us.ts \
               brv_hu.ts
RC_FILE      = reportviewer.rc

SOURCES      = ../framework/dbconnection.cpp \
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
               creportpatientcardusages.cpp \
               creportmonthclose.cpp \
               creportguests.cpp \
               creportdeviceusages.cpp \
               dlgprogress.cpp

HEADERS      = ../framework/dbconnection.h \
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
               creportpatientcardusages.h \
               creportmonthclose.h \
               creportguests.h \
               creportdeviceusages.h \
               dlgprogress.h

FORMS        = wndmain.ui \
               dlgprogress.ui
