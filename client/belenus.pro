TEMPLATE = app
FORMS = ../framework/dlgcrud.ui \
    dlghardwaretest.ui \
    dlglogin.ui \
    dlglogs.ui \
    dlgpaneltypeedit.ui \
    dlgpreferences.ui \
    dlgpwdconfirm.ui \
    dlguseredit.ui \
    dlgpatientoriginedit.ui \
    dlgreasontovisitedit.ui \
    wndmain.ui \
    dlgpatientedit.ui \
    dlgattendanceedit.ui
HEADERS = belenus.h \
    ../framework/belenusmessages.h \
    ../framework/dbconnection.h \
    ../framework/dlgcrud.h \
    ../framework/qtframework.h \
    ../framework/qtlogger.h \
    ../framework/qtmysqlconnection.h \
    ../framework/qtmysqlquerymodel.h \
    ../framework/sevexception.h \
    ../framework/tracer.h \
    crud/dlgpaneltypes.h \
    crud/dlgusers.h \
    crud/dlgpatientorigin.h \
    crud/dlgreasontovisit.h \
    crud/dlgpatient.h \
    crud/dlgattendance.h \
    db/dbpatientorigin.h \
    db/dbreasontovisit.h \
    db/dbpatient.h \
    db/dbattendance.h \
    dlg/dlgpatientselect.h \
    edit/dlgpaneltypeedit.h \
    edit/dlguseredit.h \
    edit/dlgpatientedit.h \
    edit/dlgpatientoriginedit.h \
    edit/dlgreasontovisitedit.h \
    edit/dlgattendanceedit.h \
    bs_connection.h \
    communication.h \
    communication_demo.h \
    dbuser.h \
    dlghardwaretest.h \
    dlglogin.h \
    dlglogs.h \
    dlgpreferences.h \
    dlgpwdconfirm.h \
    frmpanel.h \
    mdipanels.h \
    preferences.h \
    wndmain.h
SOURCES = main.cpp \
    ../framework/dbconnection.cpp \
    ../framework/dlgcrud.cpp \
    ../framework/qtlogger.cpp \
    ../framework/qtmysqlconnection.cpp \
    ../framework/qtmysqlquerymodel.cpp \
    ../framework/tracer.cpp \
    crud/dlgpaneltypes.cpp \
    crud/dlgusers.cpp \
    crud/dlgpatientorigin.cpp \
    crud/dlgreasontovisit.cpp \
    crud/dlgpatient.cpp \
    crud/dlgattendance.cpp \
    db/dbpatientorigin.cpp \
    db/dbreasontovisit.cpp \
    db/dbpatient.cpp \
    db/dbattendance.cpp \
    dlg/dlgpatientselect.cpp \
    edit/dlgpaneltypeedit.cpp \
    edit/dlguseredit.cpp \
    edit/dlgpatientedit.cpp \
    edit/dlgpatientoriginedit.cpp \
    edit/dlgreasontovisitedit.cpp \
    edit/dlgattendanceedit.cpp \
    bs_connection.cpp \
    communication_demo.cpp \
    dbuser.cpp \
    dlghardwaretest.cpp \
    dlglogin.cpp \
    dlglogs.cpp \
    dlgpreferences.cpp \
    dlgpwdconfirm.cpp \
    frmpanel.cpp \
    mdipanels.cpp \
    preferences.cpp \
    wndmain.cpp
win32 { 
    HEADERS += communication_serial.h
    SOURCES += communication_serial.cpp
}
TRANSLATIONS = belenus_us.ts \
    belenus_hu.ts
DESTDIR = ..
QT += sql
QT += network
CONFIG += qt
CONFIG += console
win32:DEFINES -= UNICODE
