#-------------------------------------------------
#
# Project created by QtCreator 2010-12-08T21:54:04
#
#-------------------------------------------------

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

QT          += core gui sql
TARGET       = Setup
TEMPLATE     = app
DESTDIR      = ..
RC_FILE      = setup.rc
RESOURCES   += install.qrc
CONFIG      += qt
TRANSLATIONS = setup_hu.ts \
               qt_hu.ts

win32:DEFINES -= UNICODE

FORMS       += dlgMain.ui

SOURCES     += ../framework/dbconnection.cpp \
               ../framework/qtmysqlconnection.cpp \
               ../framework/qtmysqlquerymodel.cpp \
               ../framework/qtlogger.cpp \
               ../framework/logger/FileWriter.cpp \
               ../framework/tracer.cpp \
               main.cpp \
               dlgMain.cpp \
               ../client/communication_serial.cpp \
               ../client/ClassSerialComm.cpp \
               ../client/ClassHardware.cpp \
               cregistry.cpp

HEADERS     += ../framework/dbconnection.h \
               ../framework/qtmysqlconnection.h \
               ../framework/qtmysqlquerymodel.h \
               ../framework/qtlogger.h \
               ../framework/logger/FileWriter.h \
               ../framework/tracer.h \
               dlgMain.h \
               ../client/ClassSerialComm.h \
               ../client/ClassHardware.h \
               ../client/communication_serial.h \
               ../client/communication_defines.h \
               ../client/communication.h \
               cregistry.h

