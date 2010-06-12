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
    dlgattendanceedit.ui \
    dlginputstart.ui \
    dlgpanelstatusesedit.ui \
    dlgpatientcardtypeedit.ui \
    dlgpatientcardedit.ui \
    dlgpatientcardadd.ui
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
    crud/dlgpatientselect.h \
    crud/dlgpanelstatuses.h \
    crud/dlgpatientcardtype.h \
    crud/dlgpatientcard.h \
    crud/dlgpostponedpatientselect.h \
    db/dbpatientorigin.h \
    db/dbreasontovisit.h \
    db/dbpatient.h \
    db/dbattendance.h \
    db/dbpatientcard.h \
    db/dbpatientcardtype.h \
    db/dbpanelstatuses.h \
    db/dbuser.h \
    db/dbpostponed.h \
    db/dbpaneltypes.h \
    db/dbpaneluses.h \
    dlg/dlghardwaretest.h \
    dlg/dlglogin.h \
    dlg/dlglogs.h \
    dlg/dlgpreferences.h \
    dlg/dlgpwdconfirm.h \
    dlg/dlginputstart.h \
    dlg/dlgpatientcardadd.h \
    edit/dlgpaneltypeedit.h \
    edit/dlguseredit.h \
    edit/dlgpatientedit.h \
    edit/dlgpatientoriginedit.h \
    edit/dlgreasontovisitedit.h \
    edit/dlgattendanceedit.h \
    edit/dlgpanelstatusesedit.h \
    edit/dlgpatientcardtypeedit.h \
    edit/dlgpatientcardedit.h \
    bs_connection.h \
    communication.h \
    communication_demo.h \
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
    crud/dlgpatientselect.cpp \
    crud/dlgpanelstatuses.cpp \
    crud/dlgpatientcardtype.cpp \
    crud/dlgpatientcard.cpp \
    crud/dlgpostponedpatientselect.cpp \
    db/dbpatientorigin.cpp \
    db/dbreasontovisit.cpp \
    db/dbpatient.cpp \
    db/dbattendance.cpp \
    db/dbpatientcard.cpp \
    db/dbpatientcardtype.cpp \
    db/dbpanelstatuses.cpp \
    db/dbuser.cpp \
    db/dbpostponed.cpp \
    db/dbpaneltypes.cpp \
    db/dbpaneluses.cpp \
    dlg/dlghardwaretest.cpp \
    dlg/dlglogin.cpp \
    dlg/dlglogs.cpp \
    dlg/dlgpreferences.cpp \
    dlg/dlgpwdconfirm.cpp \
    dlg/dlginputstart.cpp \
    dlg/dlgpatientcardadd.cpp \
    edit/dlgpaneltypeedit.cpp \
    edit/dlguseredit.cpp \
    edit/dlgpatientedit.cpp \
    edit/dlgpatientoriginedit.cpp \
    edit/dlgreasontovisitedit.cpp \
    edit/dlgattendanceedit.cpp \
    edit/dlgpanelstatusesedit.cpp \
    edit/dlgpatientcardtypeedit.cpp \
    edit/dlgpatientcardedit.cpp \
    bs_connection.cpp \
    communication_demo.cpp \
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
