
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

QT          += core gui
CONFIG      += qt
RESOURCES   += startup.qrc
RC_FILE      = startup.rc
TEMPLATE     = app
TARGET       = Startup
DESTDIR      = ..
TRANSLATIONS = startup_hu.ts \
               startup_de.ts \
               startup_sk.ts \

HEADERS     += mainwindow.h

SOURCES     += main.cpp\
               mainwindow.cpp

FORMS       += mainwindow.ui
