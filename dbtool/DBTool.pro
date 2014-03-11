#-------------------------------------------------
#
# Project created by QtCreator 2012-05-28T14:01:59
#
#-------------------------------------------------

QT       += core gui sql

DESTDIR         = ..
TARGET          = DBTool
TEMPLATE        = app
RC_FILE         = DBTool.rc
SOURCES        += main.cpp \
                  mainwindow.cpp \
                  dlgprogress.cpp
HEADERS        += mainwindow.h \
                  dlgprogress.h
FORMS          += mainwindow.ui \
                  dlgprogress.ui
TRANSLATIONS    = dbtool_us.ts \
                  dbtool_hu.ts
RESOURCES      += DBTool.qrc
