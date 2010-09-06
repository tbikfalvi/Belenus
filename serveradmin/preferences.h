#ifndef ADMIN_PREFERENCES_H
#define ADMIN_PREFERENCES_H

#include "../framework/preferences.h"



class AdminPreferences : public Preferences
{
public:
    AdminPreferences() : Preferences("BelenusServerAdmin.ini") { checkAndSetDefaults(); }

    void checkAndSetDefaults() {
        QString v;
        int t;


        setValue("version", "0.1");

        if ( value("server/host")=="" )
            setValue("server/host", "127.0.0.1");

        v = value("server/port");
        t = v.toInt();
        if ( v.isEmpty() || t<=0 || t>65535 )
            setValue("server/port", "4000");

        v = value("loglevel/console");
        t = v.toInt();
        if ( v.isEmpty() || t<cSeverity::MIN || t>cSeverity::MAX )
            setValue("loglevel/console", "3");

        v = value("loglevel/gui");
        t = v.toInt();
        if ( v.isEmpty() || t<cSeverity::MIN || t>cSeverity::MAX )
            setValue("loglevel/gui", "3");
    }
};

#endif
