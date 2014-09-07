
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

QT          += core gui sql
CONFIG      += qt
RESOURCES   += advertisement.qrc
RC_FILE      = advertisement.rc
TEMPLATE     = app
TARGET       = Advertisement
DESTDIR      = ..
TRANSLATIONS = advertisement_hu.ts

HEADERS     += ../framework/dbconnection.h \
               ../framework/qtmysqlconnection.h \
               ../framework/qtmysqlquerymodel.h \
               ../framework/qtlogger.h \
               ../framework/logger/FileWriter.h \
               ../framework/tracer.h \
               advertisementwindow.h \
               dbadvertisements.h \
               dlgadvertisementedit.h

SOURCES     += ../framework/dbconnection.cpp \
               ../framework/qtmysqlconnection.cpp \
               ../framework/qtmysqlquerymodel.cpp \
               ../framework/qtlogger.cpp \
               ../framework/logger/FileWriter.cpp \
               ../framework/tracer.cpp \
               main.cpp\
               advertisementwindow.cpp \
               dbadvertisements.cpp \
               dlgadvertisementedit.cpp

FORMS       += mainwindow.ui \
               dlgAdvertisement.ui \
               dlgAdvertisementedit.ui
