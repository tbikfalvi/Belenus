QT += widgets network sql
TEMPLATE = app
TARGET = webapp
CONFIG += release c++17 lrelease embed_translations
CONFIG -= debug debug_and_release debug_and_release_target

SOURCES += main.cpp \
           mainwindow.cpp \
           database.cpp \
           webcommunication.cpp \
           licencemanager.cpp \
           logger.cpp \
           filemanager.cpp \
           languagemanager.cpp

HEADERS += mainwindow.h \
           database.h \
           webcommunication.h \
           licencemanager.h \
           logger.h \
           filemanager.h \
           languagemanager.h \
           uimessage.h

TRANSLATIONS += translations/webapp_en.ts \
                translations/webapp_hu.ts

FORMS += mainwindow.ui
RESOURCES += webapp.qrc
