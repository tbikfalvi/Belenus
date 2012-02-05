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
#include "db/dbuser.h"
#include "db/dbguest.h"
#include "preferences.h"
#include "communication.h"
#include "cassa.h"
#include "general.h"

//====================================================================================

#define CONST_COMPONENT_SENSOLITE   1
#define CONST_COMPONENT_KIWISUN     2
#define CONST_COMPONENT_DATABASE    4
#define CONST_COMPONENT_HARDWARE    8
#define CONST_COMPONENT_INTERNET    16
#define CONST_COMPONENT_CLIENT      32
#define CONST_COMPONENT_VIEWER      64

//====================================================================================

extern cDBUser                  g_obUser;
extern cPreferences            *g_poPrefs;
extern CS_Communication        *g_poHardware;
extern cCassa                   g_obCassa;
extern cGeneral                 g_obGen;
extern cDBGuest                 g_obGuest;

//====================================================================================

#endif

//====================================================================================
