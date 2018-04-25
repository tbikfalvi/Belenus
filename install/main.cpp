//====================================================================================
//
// Belenus telepito alkalmazas (c) Pagony Multimedia Studio Bt - 2010
//
//====================================================================================
//
// Filename    : main.cpp
// AppVersion  : 1.0
// FileVersion : 1.0
// Author      : Bikfalvi Tamas
//
//====================================================================================
//
//====================================================================================

#include <QtGui/QApplication>
#include <QTranslator>
#include <QMessageBox>
#include <QSettings>

//====================================================================================

#include "../framework/qtlogger.h"
#include "dlgMain.h"

//====================================================================================

cRegistry        g_obReg;
QTranslator     *poTransSetup;
QTranslator     *poTransQT;
QApplication    *apMainApp;
QString          g_qsCurrentPath;
cQTLogger        g_obLogger;

//====================================================================================
int main(int argc, char *argv[])
//====================================================================================
{
    apMainApp = new QApplication(argc, argv);

    if( !QFile::exists( QString("%1\\lang\\setup_hu.qm").arg(QDir::currentPath()) ) )
    {
        QString     qsPathWindows = g_obReg.keyValueS( QString("HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion"), QString("SystemRoot"), "" );

        QDir::setCurrent( QString("%1\\Temp\\BelenusInstall").arg(qsPathWindows) );
    }

    g_qsCurrentPath = QDir::currentPath();

    poTransSetup = new QTranslator();
    poTransQT = new QTranslator();

    poTransSetup->load( QString("%1\\lang\\setup_hu.qm").arg(QDir::currentPath()) );
    poTransQT->load( QString("%1\\lang\\qt_hu.qm").arg(QDir::currentPath()) );

    apMainApp->installTranslator( poTransSetup );
    apMainApp->installTranslator( poTransQT );

    bool    bUninstall  = false;
    bool    bSilent     = false;
    QString qsDevice    = "3";
    QString qsLangInst  = "hu";
    QString qsLangApp   = "hu";
    QString qsComPort   = "";
    QString qsDir       = "c:/kiwisun/belenus";

    for( int i=1; i<argc; i++ )
    {
        if( strcmp(argv[i],"-uninstall") == 0 )
        {
            bUninstall = true;
        }
        else if( strcmp(argv[i],"-silent") == 0 )
        {
            bSilent = true;
        }
        else if( strncmp(argv[i],"-device:",8) == 0 )
        {
            qsDevice = QString( argv[i] ).replace("-device:","");
        }
        else if( strncmp(argv[i],"-com:",5) == 0 )
        {
            qsComPort = QString( argv[i] ).replace("-com:","");
        }
        else if( strncmp(argv[i],"-langi:",7) == 0 )
        {
            qsLangInst = QString( argv[i] ).replace("-langi:","");
        }
        else if( strncmp(argv[i],"-langa:",7) == 0 )
        {
            qsLangApp = QString( argv[i] ).replace("-langa:","");
        }
        else if( strncmp(argv[i],"-dir:",5) == 0 )
        {
            qsDir = QString( argv[i] ).replace("-dir:","");
        }
    }

    dlgMain w( 0, bUninstall, bSilent, qsDevice.toInt(), qsComPort.toInt(), qsLangInst, qsLangApp, qsDir );

    w.show();

    return apMainApp->exec();
}
//====================================================================================
