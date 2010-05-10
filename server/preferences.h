#ifndef SERVER_PREFERENCES_H
#define SERVER_PREFERENCES_H

#include "../framework/preferences.h"


class ServerPreferences : public Preferences
{

public:
    ServerPreferences() : Preferences()
    {
        setValue("version", "0.0.1");

        if ( value("server/interface")=="" )
            setValue("server/interface", "127.0.0.1");

        int port = value("server/port").toInt();
        if ( port<=0 || port>65535 )
            setValue("server/port", "4000");

    }
};


#endif
