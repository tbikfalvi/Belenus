#ifndef SERVER_PREFERENCES_H
#define SERVER_PREFERENCES_H

#include "../framework/preferences.h"


class ServerPreferences : public Preferences
{

public:
    ServerPreferences() : Preferences("BelenusServer.ini") { checkAndSetDefaults(); }

    void checkAndSetDefaults()
    {
        setValue("version", "0.1");

        if ( value("server/interface")=="" )
            setValue("server/interface", "127.0.0.1");

        int port = value("server/port").toInt();
        if ( port<=0 || port>65535 )
            setValue("server/port", "4000");

        if ( value("database/host")=="" )
            setValue("database/host", "localhost");

        if ( value("database/schema")=="" )
            setValue("database/schema", "BelenusDB");

        if ( value("database/username")=="" )
            setValue("database/username", "belenus_user");

        if ( value("database/password")=="" )
            setValue("database/password", "Sync205BW");
    }

};


#endif
