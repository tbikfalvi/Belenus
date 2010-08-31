# #====================================================================================
# #
# # Belenus Report Viewer alkalmazas (c) Pagony Multimedia Studio Bt - 2010
# #
# #====================================================================================
TARGET = BelenusReportViewer
TEMPLATE = app
DESTDIR = ..
win32:DEFINES -= UNICODE
CONFIG += qt
#CONFIG += console
TRANSLATIONS = brv_us.ts \
    brv_hu.ts
HEADERS += belenusReportViewer.h
SOURCES += main.cpp
QT += network
QT += sql
#FORMS += mainwindow.ui
