//====================================================================================
//
// Belenus Report Viewer alkalmazas (v) Pagony Multimedia Studio Bt - 2010
//
//====================================================================================

#include <QApplication>
#include <QTranslator>
#include <iostream>
#include <QSettings>

#include "wndmain.h"

//====================================================================================

#include "../framework/qtlogger.h"
#include "../framework/qtframework.h"
#include "../framework/logger/FileWriter.h"
#include "../language/language.h"

//====================================================================================

#define APPLICATION_VERSION_NUMBER  "1.9.0.0"

//====================================================================================

cQTLogger                g_obLogger;
FileWriter               g_obLogFileWriter("reportviewer_%1.log");
cLanguage                g_obLanguage;

cQTMySQLConnection  *g_poDB;

using namespace std;

//====================================================================================
int main( int argc, char *argv[] )
{
    QApplication  apMainApp( argc, argv );

    int nRet = 1;

    g_obLogger.attachWriter("file", &g_obLogFileWriter);
    g_obLogger.setMinimumSeverity("file", cSeverity::DEBUG);

    g_obLogger(cSeverity::INFO) << "Belenus Report Viewer started." << EOM;

    try
    {
        g_poDB = new cQTMySQLConnection;
        g_poDB->setHostName( "localhost" );
        g_poDB->setDatabaseName( "belenus" );
        g_poDB->setUserName( "belenus" );
        g_poDB->setPassword( "belenus" );
        g_poDB->open();

        QString qsCurrentPath = QDir::currentPath().replace( "\\", "/" );

        g_obLanguage.getLanguages();
        g_obLanguage.init( &apMainApp, "brv", "_" );

        cWndMain  *obMainWindow = new cWndMain( 0, APPLICATION_VERSION_NUMBER );
        obMainWindow->show();
        if( argc > 2 )
        {
            QString qsName = QString( argv[1] );
            QString qsPass = QString( argv[2] );

            obMainWindow->setLoginData( qsName, qsPass );
        }
        nRet = apMainApp.exec();
    }
    catch( cSevException &e )
    {
        cerr << ">> " << e.what() << endl << flush;;

        g_obLogger(e.severity()) << e.what() << EOM;
    }

    g_obLogger(cSeverity::INFO) << "Belenus Report Viewer ended." << EOM;

    delete g_poDB;

    return nRet;
}
