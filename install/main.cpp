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
cQTLogger                g_obLogger;

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

    bool bUninstall = false;

    if( argc > 1 && strcmp(argv[1],"-uninstall") == 0 )
    {
        bUninstall = true;
    }

    dlgMain w( 0, bUninstall );

    w.show();

    return apMainApp->exec();
}
//====================================================================================
