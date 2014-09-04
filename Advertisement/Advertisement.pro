
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

QT          += core gui sql
CONFIG      += qt
RESOURCES   += belenus.qrc
RC_FILE      = Belenus.rc
TEMPLATE     = app
TARGET       = Advertisement
DESTDIR      = ..
TRANSLATIONS = advertisement_hu.ts

HEADERS     += ../framework/dbconnection.h \
               ../framework/qtframework.h \
               ../framework/qtlogger.h \
               ../framework/logger/DatabaseWriter.h \
               ../framework/logger/GUIWriter.h \
               ../framework/logger/ConsoleWriter.h \
               ../framework/logger/FileWriter.h \
               ../framework/qtmysqlconnection.h \
               ../framework/qtmysqlquerymodel.h \
               ../framework/sevexception.h \
               ../framework/tracer.h \
               mainwindow.h \
               advertisementwindow.h \
               dbadvertisements.h \
               dlgadvertisementedit.h

SOURCES     += ../framework/dbconnection.cpp \
               ../framework/qtlogger.cpp \
               ../framework/logger/DatabaseWriter.cpp \
               ../framework/logger/GUIWriter.cpp \
               ../framework/logger/FileWriter.cpp \
               ../framework/logger/ConsoleWriter.cpp \
               ../framework/qtmysqlconnection.cpp \
               ../framework/qtmysqlquerymodel.cpp \
               ../framework/tracer.cpp \
               main.cpp\
               mainwindow.cpp \
               advertisementwindow.cpp \
               dbadvertisements.cpp \
               dlgadvertisementedit.cpp

FORMS       += mainwindow.ui \
               dlgAdvertisement.ui \
               dlgAdvertisementedit.ui
