//====================================================================================
//
// Belenus Server Admin alkalmazas © Pagony Multimedia Studio Bt - 2010
//
//====================================================================================

#include <QApplication>
#include <QTranslator>
#include "../framework/qtframework.h"
#include "mainwindow.h"
#include "preferences.h"


cQTLogger             g_obLogger;
cQTMySQLConnection   *g_poDB;
Preferences          *g_poPrefs;





int main( int argc, char *argv[] )
{
    QApplication  apMainApp( argc, argv );

    g_poDB     = new cQTMySQLConnection;
    g_poPrefs  = new AdminPreferences();
    g_obLogger.setMinSeverityLevels(cSeverity::MAX, cSeverity::MIN, cSeverity::MIN );

    int nRet = 1;
    try
    {
        //---------------------------------------------------------------------------------
        // Adatbazis megnyitasa
        //---------------------------------------------------------------------------------
//        g_poDB->open();
//        g_obLogger.setDBConnection( g_poDB );

        g_obLogger << cSeverity::INFO;
        g_obLogger << "Belenus Server Admin Version " << g_poPrefs->value("version").toStdString() << " started.";
        g_obLogger << cQTLogger::EOM;

        //---------------------------------------------------------------------------------
        // Beallitasok betoltese ini file-bol
        //---------------------------------------------------------------------------------
//        if( g_poPrefs->getLang() != "uk" )
//        {
//            QTranslator obTrans;
//            QString     qsTransFile = "lang/" + g_poPrefs->getLangFilePrefix() + g_poPrefs->getLang() + ".qm";
//            g_obLogger << cSeverity::INFO;
//            obTrans.load( qsTransFile );
//            apMainApp.installTranslator( &obTrans );
//        }

        //---------------------------------------------------------------------------------
        // Beallitasok betoltese adatbazisbol
        //---------------------------------------------------------------------------------
//        g_poPrefs->loadDBSettings();


        //---------------------------------------------------------------------------------
        // Fo dialogus ablak megjelenitese
        //---------------------------------------------------------------------------------
        MainWindow  wndMain;
        wndMain.show();
        nRet = wndMain.exec();
    }
    catch( cSevException &e )
    {
        g_obLogger << e.severity();
        g_obLogger << e.what();
        g_obLogger << cQTLogger::EOM;
    }

    g_obLogger << cSeverity::INFO;
    g_obLogger << "Belenus Server Admin Version " << g_poPrefs->value("version").toStdString() << " ended.";
    g_obLogger << cQTLogger::EOM;

    delete g_poPrefs;
    delete g_poDB;

    return nRet;
}
