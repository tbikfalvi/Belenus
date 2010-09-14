//====================================================================================
//
// Belenus Report Viewer alkalmazas (v) Pagony Multimedia Studio Bt - 2010
//
//====================================================================================

#include <QApplication>
#include <QTranslator>
#include <iostream>

#include "wndmain.h"
//====================================================================================

#include "../framework/qtframework.h"

//====================================================================================

cQTLogger            g_obLogger;
cQTMySQLConnection  *g_poDB;

using namespace std;

//====================================================================================
int main( int argc, char *argv[] )
{
    QApplication  apMainApp( argc, argv );

    int nRet = 1;

    g_obLogger(cSeverity::INFO) << "Belenus Report Viewer started." << EOM;

    try
    {
        g_poDB = new cQTMySQLConnection;
        g_poDB->setHostName( "localhost" );
        g_poDB->setDatabaseName( "belenus" );
        g_poDB->setUserName( "belenus" );
        g_poDB->setPassword( "belenus" );
        g_poDB->open();

        cWndMain  obMainWindow;
        obMainWindow.show();
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
