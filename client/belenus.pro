TEMPLATE = app
FORMS = ../framework/dlgcrud.ui \
    dlghardwaretest.ui \
    dlglogin.ui \
    dlglogs.ui \
    dlgpaneltypeedit.ui \
    dlgpreferences.ui \
    dlgproductedit.ui \
    dlgproducttypeedit.ui \
    dlgpwdconfirm.ui \
    dlgusercardedit.ui \
    dlgusercardtypeedit.ui \
    dlguseredit.ui \
    wndmain.ui \
    dlgpatientoriginedit.ui
HEADERS = ../framework/belenusmessages.h \
    ../framework/dbconnection.h \
    ../framework/dlgcrud.h \
    ../framework/qtframework.h \
    ../framework/qtlogger.h \
    ../framework/qtmysqlconnection.h \
    ../framework/qtmysqlquerymodel.h \
    ../framework/sevexception.h \
    ../framework/tracer.h \
    crud/dlgpaneltypes.h \
    crud/dlgproducts.h \
    crud/dlgproducttypes.h \
    crud/dlgusercards.h \
    crud/dlgusercardtypes.h \
    crud/dlgusers.h \
    belenus.h \
    bs_connection.h \
    communication.h \
    communication_demo.h \
    dbproduct.h \
    dbproducttype.h \
    dbuser.h \
    dbusercard.h \
    dbusercardtype.h \
    dlghardwaretest.h \
    dlglogin.h \
    dlglogs.h \
    dlgpaneltypeedit.h \
    dlgpreferences.h \
    dlgproductedit.h \
    dlgproducttypeedit.h \
    dlgpwdconfirm.h \
    dlgusercardedit.h \
    dlgusercardtypeedit.h \
    dlguseredit.h \
    frmpanel.h \
    mdipanels.h \
    preferences.h \
    wndmain.h \
    db/dbpatientorigin.h \
    crud/dlgpatientorigin.h \
    edit/dlgpatientoriginedit.h
SOURCES = ../framework/dbconnection.cpp \
    ../framework/dlgcrud.cpp \
    ../framework/qtlogger.cpp \
    ../framework/qtmysqlconnection.cpp \
    ../framework/qtmysqlquerymodel.cpp \
    ../framework/tracer.cpp \
    crud/dlgpaneltypes.cpp \
    crud/dlgproducts.cpp \
    crud/dlgproducttypes.cpp \
    crud/dlgusercards.cpp \
    crud/dlgusercardtypes.cpp \
    crud/dlgusers.cpp \
    bs_connection.cpp \
    communication_demo.cpp \
    dbproduct.cpp \
    dbproducttype.cpp \
    dbuser.cpp \
    dbusercard.cpp \
    dbusercardtype.cpp \
    dlghardwaretest.cpp \
    dlglogin.cpp \
    dlglogs.cpp \
    dlgpaneltypeedit.cpp \
    dlgpreferences.cpp \
    dlgproductedit.cpp \
    dlgproducttypeedit.cpp \
    dlgpwdconfirm.cpp \
    dlgusercardedit.cpp \
    dlgusercardtypeedit.cpp \
    dlguseredit.cpp \
    frmpanel.cpp \
    main.cpp \
    mdipanels.cpp \
    preferences.cpp \
    wndmain.cpp \
    db/dbpatientorigin.cpp \
    crud/dlgpatientorigin.cpp \
    edit/dlgpatientoriginedit.cpp
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
