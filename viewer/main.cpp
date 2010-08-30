//====================================================================================
//
// Belenus Report Viewer alkalmazas (v) Pagony Multimedia Studio Bt - 2010
//
//====================================================================================

#include <QApplication>
#include <QTranslator>

#include "wndmain.h"
//====================================================================================

#include "../framework/qtframework.h"

//====================================================================================

//cQTLogger            g_obLogger;

//====================================================================================
int main( int argc, char *argv[] )
{
    QApplication  apMainApp( argc, argv );

    int nRet = 1;


//    try
//    {
//        g_obLogger(cSeverity::INFO) << "Belenus Report Viewer Version " << g_prefs.value("version") << " started." << cQTLogger::EOM;

        cWndMain  obMainWindow;
        obMainWindow.show();
        nRet = apMainApp.exec();
//    }
//    catch( cSevException &e )
//    {
//        g_obLogger(e.severity()) << e.what() << cQTLogger::EOM;
//    }

//    g_obLogger(cSeverity::INFO) << "Belenus Report Viewer Version " << g_prefs.value("version") << " ended." << cQTLogger::EOM;

    return nRet;
}
