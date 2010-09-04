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


cQTLogger            g_obLogger;
AdminPreferences     g_prefs;





int main( int argc, char *argv[] )
{
    QApplication  apMainApp( argc, argv );

    int nRet = 1;
    try
    {
        g_obLogger(cSeverity::INFO) << "Belenus Server Admin Version " << g_prefs.value("version") << " started." << EOM;

        MainWindow  wndMain;
        wndMain.show();
        nRet = wndMain.exec();
    }
    catch( cSevException &e )
    {
        g_obLogger(e.severity()) << e.what() << EOM;
    }

    g_obLogger(cSeverity::INFO) << "Belenus Server Admin Version " << g_prefs.value("version") << " ended." << EOM;

    return nRet;
}
