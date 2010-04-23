//====================================================================================
//
// Belenus Server alkalmazas © Pagony Multimedia Studio Bt - 2010
//
//====================================================================================
//
// Filename    : bs_main.cpp
// AppVersion  : 1.0
// FileVersion : 1.0
// Author      : Bikfalvi Tamas
//
//====================================================================================
// Alkalmazas fo forras allomanya. Kornyezeti valtozok beolvasasa, adatbazis
// kapcsolat kiepitese, fo dialogus ablak megnyitasa
//====================================================================================

#include <QApplication>
#include <QTranslator>

#include "belenusserver.h"
#include "bs_main_dialog.h"

//====================================================================================

cQTLogger             g_obLogger;
cQTMySQLConnection   *g_poDB;
cPreferences         *g_poPrefs;
int                   g_nConnectionCount;
QLabel               *g_msgLabel;

//====================================================================================
int main( int argc, char *argv[] )
{
   QApplication  apMainApp( argc, argv );

   //---------------------------------------------------------------------------------
   // Global cuccok beallitasa
   //---------------------------------------------------------------------------------
   g_poDB     = new cQTMySQLConnection;

   g_poPrefs  = new cPreferences( QString::fromAscii( "./belenusserver.ini" ) );
   g_poPrefs->setVersion( "0.0.1" );
   g_poPrefs->setLangFilePrefix( "bs_" );
   g_poPrefs->setDBAccess( "localhost", "belenus", "belenus", "belenus" );

   g_nConnectionCount = 0;

   int nRet = 1;
   try
   {
       //---------------------------------------------------------------------------------
       // Adatbazis megnyitasa
       //---------------------------------------------------------------------------------
       g_poDB->open();
       g_obLogger.setDBConnection( g_poDB );

       g_obLogger << cSeverity::INFO;
       g_obLogger << "Belenus Version " << g_poPrefs->getVersion().toStdString() << " started.";
       g_obLogger << cQTLogger::EOM;

       //---------------------------------------------------------------------------------
       // Beallitasok betoltese ini file-bol
       //---------------------------------------------------------------------------------
       if( g_poPrefs->getLang() != "uk" )
       {
           QTranslator obTrans;
           QString     qsTransFile = "lang/" + g_poPrefs->getLangFilePrefix() + g_poPrefs->getLang() + ".qm";
           g_obLogger << cSeverity::INFO;
           obTrans.load( qsTransFile );
           apMainApp.installTranslator( &obTrans );
       }

       int dlgWidth, dlgHeight;

       g_poPrefs->getMainWindowSize( &dlgWidth, &dlgHeight );

       //---------------------------------------------------------------------------------
       // Beallitasok betoltese adatbazisbol
       //---------------------------------------------------------------------------------
       g_poPrefs->loadDBSettings();

       //---------------------------------------------------------------------------------
       // Fo dialogus ablak megjelenitese
       //---------------------------------------------------------------------------------
       MainDialog  dlgMain;

       dlgMain.setModal( true );
       dlgMain.setMaximumSize( dlgWidth, dlgHeight );
       dlgMain.setMinimumSize( dlgWidth, dlgHeight );
       dlgMain.resize( dlgWidth, dlgHeight );
       dlgMain.show();

       //---------------------------------------------------------------------------------
       // Program inditasa
       //---------------------------------------------------------------------------------
       nRet = dlgMain.exec();
   }
   catch( cSevException &e )
   {
       g_obLogger << e.severity();
       g_obLogger << e.what();
       g_obLogger << cQTLogger::EOM;
   }

   g_obLogger << cSeverity::INFO;
   g_obLogger << "Belenus Version " << g_poPrefs->getVersion().toStdString() << " ended.";
   g_obLogger << cQTLogger::EOM;

   delete g_poPrefs;
   delete g_poDB;
   
   return nRet;
}
//====================================================================================
