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

//====================================================================================

cQTLogger            g_obLogger;
//FileWriter           g_obLogFileWriter("client_%1_%2.log");
FileWriter           g_obLogFileWriter("reportviewer_%1.log");

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

        QSettings   obPrefFile( "belenus.ini", QSettings::IniFormat );
        QString     qsLang = obPrefFile.value( QString::fromAscii( "Lang" ), "en" ).toString();
        QString     qsLangBl = QString("lang/brv_%1.qm").arg( qsLang );
        QString     qsLangQT = QString("lang/qt_%1.qm").arg( qsLang );

        g_obLogger(cSeverity::INFO) << "Language file: " << qsLangBl << EOM;

        QTranslator     obBlTr;
        QTranslator     obQtTr;

        obBlTr.load( qsLangBl );
        obQtTr.load( qsLangQT );

        apMainApp.installTranslator( &obBlTr );
        apMainApp.installTranslator( &obQtTr );

        cWndMain  obMainWindow;
        obMainWindow.show();
        if( argc > 2 )
        {
            QString qsName = QString( argv[1] );
            QString qsPass = QString( argv[2] );

            obMainWindow.setLoginData( qsName, qsPass );
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
