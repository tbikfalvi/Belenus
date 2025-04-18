//====================================================================================
//
// Belenus Kliens alkalmazas (c) Pagony Multimedia Studio Bt - 2010
//
//====================================================================================
//
// Filename    : belenus.h
// AppVersion  : 1.0
// FileVersion : 1.0
// Author      : Ballok Peter, Bikfalvi Tamas, Kovacs Gabor
//
//====================================================================================
// Alkalmazas fo header allomanya.
//====================================================================================

#ifndef BELENUS_H
#define BELENUS_H

//====================================================================================

#include "../framework/qtframework.h"
#include "../language/language.h"
#include "db/dbuser.h"
#include "db/dbguest.h"
#include "preferences.h"
#include "communication.h"
#include "cassa.h"
#include "general.h"

//====================================================================================

#define CONST_COMPONENT_SENSOLITE           1
#define CONST_COMPONENT_KIWISUN             2
#define CONST_COMPONENT_DATABASE            4
#define CONST_COMPONENT_HARDWARE            8
#define CONST_COMPONENT_INTERNET            16
#define CONST_COMPONENT_CLIENT              32
#define CONST_COMPONENT_VIEWER              64

//====================================================================================

#define STATUS_ALAP                         0
#define STATUS_VETKOZES                     1
#define STATUS_SZAUNAZAS                    2
#define STATUS_BARNULAS                     3
#define STATUS_UTOHUTES                     4
#define STATUS_VARAKOZAS                    9

//====================================================================================

#define AUTO_MAIL_ON_PCSELL                 1
#define AUTO_MAIL_ON_PCUSE                  2
#define AUTO_MAIL_ON_EXPIRE                 3
#define AUTO_MAIL_ON_UNITCHANGE             4

#define AUTO_MAIL_DESTINATION_MAIL          1
#define AUTO_MAIL_DESTINATION_CARDY         2
#define AUTO_MAIL_DESTINATION_MAIL_CARDY    3

//====================================================================================

#define PATIENTHISTORY_SAVEDTODATABASE      1
#define PATIENTHISTORY_PURCHASEDCARD        2
#define PATIENTHISTORY_REFILLEDCARD         3
#define PATIENTHISTORY_PURCHASEDPRODUCT     4
#define PATIENTHISTORY_USEDDEVICEWITHCARD   5
#define PATIENTHISTORY_USEDDEVICEWITHCASH   6

//====================================================================================
extern QApplication            *apMainApp;
extern cDBUser                  g_obUser;
extern cPreferences            *g_poPrefs;
extern CS_Communication        *g_poHardware;
extern cCassa                   g_obCassa;
extern cGeneral                 g_obGen;
extern cDBGuest                 g_obGuest;
extern cLanguage                g_obLanguage;

//====================================================================================

#endif

//====================================================================================
