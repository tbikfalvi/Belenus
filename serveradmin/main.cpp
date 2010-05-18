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

    g_obLogger.setMinSeverityLevels(cSeverity::MAX, cSeverity::MIN, cSeverity::MIN );

    int nRet = 1;
    try
    {
        g_obLogger(cSeverity::INFO) << "Belenus Server Admin Version " << g_prefs.value("version").toStdString() << " started." << cQTLogger::EOM;

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

    g_obLogger(cSeverity::INFO) << "Belenus Server Admin Version " << g_prefs.value("version").toStdString() << " ended." << cQTLogger::EOM;

    return nRet;
}
