#ifndef BELENUS_H
#define BELENUS_H

#include "../framework/qtframework.h"
#include "db/dbuser.h"
#include "db/dbpatient.h"
#include "db/dbmirror.h"
#include "preferences.h"
#include "communication.h"
#include "cassa.h"

extern cDBUser                   g_obUser;
extern cPreferences             *g_poPrefs;
extern CS_Communication         *g_poHardware;
extern cDBPatient                g_obPatient;
extern unsigned int              g_uiPatientAttendanceId;
extern cCassa                    g_obCassa;
extern cDBMirror                 g_obDBMirror;

#endif
