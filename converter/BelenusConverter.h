#ifndef BELENUS_CONVERTER_H
#define BELENUS_CONVERTER_H

//====================================================================================
// BERLETTIPUS structure
//====================================================================================
typedef struct _typ_berlettipus
{
   int   nID;
   int   nAr;
   int   nEgyseg;
   char  strNev[50];
   int   nErvTolEv;
   int   nErvTolHo;
   int   nErvTolNap;
   int   nErvIgEv;
   int   nErvIgHo;
   int   nErvIgNap;
   int   nErvNapok;
   bool  bSzolariumHaszn;
   int   nEgysegIdo;
} typ_berlettipus;

//====================================================================================
// BERLET structure
//====================================================================================
typedef struct _typ_berlet
{
   char  strVonalkod[20];
   char  strMegjegyzes[50];
   int   nBerletTipus;
   int   nEgyseg;
   int   nErvEv;
   int   nErvHo;
   int   nErvNap;
   int   nPin;
} typ_berlet;
//====================================================================================
// USER structure
//====================================================================================
typedef struct _typ_user
{
   DWORD    dID;
   char     strAzonosito[20];
   char     strLoginNev[20];
   char     strNevCsalad[100];
   char     strJelszo[20];
   char     strMegjegyzes[1000];
   int      nUserLevel;
} typ_user;

#endif
