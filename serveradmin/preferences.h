#ifndef ADMIN_PREFERENCES_H
#define ADMIN_PREFERENCES_H

#include "../framework/preferences.h"

class AdminPreferences : public Preferences
{
public:
    AdminPreferences()
    {
        setValue("version", "0.0.1");

        if ( value("server/host")=="" )
            setValue("server/host", "127.0.0.1");

        int port = value("server/port").toInt();
        if ( port<=0 || port>65535 )
            setValue("server/port", "4000");

    }
};

#endif
