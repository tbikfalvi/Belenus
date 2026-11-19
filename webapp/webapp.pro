QT += widgets network sql
TEMPLATE = app
TARGET = webapp
CONFIG += release c++17
CONFIG -= debug debug_and_release debug_and_release_target

SOURCES += main.cpp
FORMS += mainwindow.ui
RESOURCES += webapp.qrc
