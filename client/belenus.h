#ifndef BELENUS_H
#define BELENUS_H

#include "../framework/qtframework.h"
#include "db/dbuser.h"
#include "preferences.h"
#include "communication.h"
#include "db/dbpatient.h"
#include "cassa.h"
#include "dlg/dlgprogress.h"

extern cDBUser                   g_obUser;
extern cPreferences             *g_poPrefs;
extern CS_Communication         *g_poHardware;
extern cDBPatient                g_obPatient;
extern unsigned int              g_uiPatientAttendanceId;
extern cCassa                    g_obCassa;
extern cDlgProgress             *g_dlgProgress;

#endif
