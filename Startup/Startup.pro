
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

QT          += core gui sql xml
CONFIG      += qt
RESOURCES   += startup.qrc
RC_FILE      = startup.rc
TEMPLATE     = app
TARGET       = Startup
DESTDIR      = ..
TRANSLATIONS = startup_hu.ts \
               startup_de.ts \
               startup_sk.ts \
               startup_it.ts

HEADERS     += mainwindow.h \
               cregistry.h

SOURCES     += main.cpp\
               mainwindow.cpp \
               cregistry.cpp

FORMS       += mainwindow.ui
