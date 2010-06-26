#ifndef BELENUS_H
#define BELENUS_H

#include "../framework/qtframework.h"
#include "db/dbuser.h"
#include "preferences.h"
#include "communication.h"
#include "bs_connection.h"
#include "db/dbpatient.h"
#include "db/dbcassa.h"

extern cDBUser                   g_obUser;
extern cPreferences             *g_poPrefs;
extern CS_Communication         *g_poHardware;
extern BelenusServerConnection  *g_poServer;
extern cDBPatient                g_obPatient;
extern cDBCassa                  g_obCassa;

#endif
