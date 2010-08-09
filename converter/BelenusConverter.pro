# #====================================================================================
# #
# # Belenus Converter alkalmazas (c) Pagony Multimedia Studio Bt - 2010
# #
# #====================================================================================
TARGET = BelenusConverter
TEMPLATE = app
DESTDIR = ..
win32:DEFINES -= UNICODE
CONFIG += qt
#CONFIG += console
HEADERS += BelenusConverter.h
SOURCES += main.cpp

