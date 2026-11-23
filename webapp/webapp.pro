QT += widgets network sql
TEMPLATE = app
TARGET = webapp
CONFIG += release c++17
CONFIG -= debug debug_and_release debug_and_release_target

SOURCES += main.cpp \
           mainwindow.cpp \
           database.cpp \
           webcommunication.cpp \
           licencemanager.cpp \
           logger.cpp \
           filemanager.cpp

HEADERS += mainwindow.h \
           database.h \
           webcommunication.h \
           licencemanager.h \
           logger.h \
           filemanager.h

FORMS += mainwindow.ui
RESOURCES += webapp.qrc
