
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

QT          += core gui multimedia network sql xml
CONFIG      += qt
RESOURCES   += websync.qrc
RC_FILE      = websync.rc
TEMPLATE     = app
DESTDIR      = ..
TRANSLATIONS = websync_hu.ts
TARGET       = websync

HEADERS     += ../framework/dbconnection.h \
               ../framework/qtmysqlconnection.h \
               ../framework/qtmysqlquerymodel.h \
               ../framework/qtlogger.h \
               ../framework/logger/FileWriter.h \
               ../framework/tracer.h \
               http.h \
               dlgmain.h \
    dlglineedit.h

SOURCES     += ../framework/dbconnection.cpp \
               ../framework/qtmysqlconnection.cpp \
               ../framework/qtmysqlquerymodel.cpp \
               ../framework/qtlogger.cpp \
               ../framework/logger/FileWriter.cpp \
               ../framework/tracer.cpp \
               main.cpp \
               http.cpp \
               dlgmain.cpp \
    dlglineedit.cpp

FORMS       += dlgmain.ui \
    dlgLineEdit.ui \
    dlglineedit.ui
