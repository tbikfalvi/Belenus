#-------------------------------------------------
#
# Project created by QtCreator 2012-05-28T14:01:59
#
#-------------------------------------------------

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets


QT          += core gui sql
CONFIG      += qt
TARGET       = DBTool
TEMPLATE     = app
DESTDIR      = ..
RESOURCES   += DBTool.qrc
TRANSLATIONS = dbtool_us.ts \
               dbtool_hu.ts
RC_FILE      = DBTool.rc

SOURCES     += main.cpp \
               mainwindow.cpp \
               dlgprogress.cpp

HEADERS     += mainwindow.h \
               dlgprogress.h

FORMS       += mainwindow.ui \
               dlgprogress.ui
