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

# CONFIG += console
TRANSLATIONS = brv_us.ts \
    brv_hu.ts
FORMS = wndmain.ui \
        dlgdemo.ui
HEADERS = belenusReportViewer.h \
    wndmain.h \
    dlgdemo.h
SOURCES = main.cpp \
    wndmain.cpp \
    dlgdemo.cpp
QT += network
QT += sql
