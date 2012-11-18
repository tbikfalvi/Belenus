#-------------------------------------------------
#
# Project created by QtCreator 2010-12-08T21:54:04
#
#-------------------------------------------------

TEMPLATE     = app
DESTDIR      = ..
TARGET       = Setup
RC_FILE      = setup.rc

CONFIG      += qt
QT          += core gui sql
TRANSLATIONS = setup_us.ts \
               setup_hu.ts \
               qt_hu.ts

win32:DEFINES -= UNICODE

FORMS       += dlgMain.ui

SOURCES     += main.cpp \
               dlgMain.cpp \
               ../client/communication_serial.cpp \
               ../client/ClassSerialComm.cpp \
               ../client/ClassHardware.cpp \
               cregistry.cpp

HEADERS     += dlgMain.h \
               ../client/ClassSerialComm.h \
               ../client/ClassHardware.h \
               ../client/communication_serial.h \
               ../client/communication_defines.h \
               ../client/communication.h \
               cregistry.h

RESOURCES   += install.qrc
