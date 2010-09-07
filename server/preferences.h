#ifndef SERVER_PREFERENCES_H
#define SERVER_PREFERENCES_H

#include "../framework/preferences.h"


class ServerPreferences : public Preferences
{

public:
    ServerPreferences() : Preferences("BelenusServer.ini") { checkAndSetDefaults(); }

    void checkAndSetDefaults()
    {
        int t;
        QString v;
        setValue("version", "0.1");

        if ( value("server/interface")=="" )
            setValue("server/interface", "127.0.0.1");

        v = value("server/port");
        t = v.toInt();
        if ( v.isEmpty() || t<=0 || t>65535 )
            setValue("server/port", "4000");

        if ( value("database/host")=="" )
            setValue("database/host", "localhost");

        if ( value("database/schema")=="" )
            setValue("database/schema", "BelenusDB");

        if ( value("database/username")=="" )
            setValue("database/username", "belenus_user");

        if ( value("database/password")=="" )
            setValue("database/password", "Sync205BW");

        v = value("loglevel/console");
        t = v.toInt();
        if ( v.isEmpty() || t<cSeverity::MIN || t>cSeverity::MAX )
            setValue("loglevel/console", "3");

        v = value("loglevel/db");
        t = v.toInt();
        if ( v.isEmpty() || t<cSeverity::MIN || t>cSeverity::MAX )
            setValue("loglevel/db", "3");
    }

};


#endif
