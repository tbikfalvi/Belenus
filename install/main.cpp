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

//====================================================================================

#include "dlgMain.h"

#include "vregistry.h"
#include "vqtconvert.h"

//=======================================================================================

using namespace voidrealms::win32;

//====================================================================================
int main(int argc, char *argv[])
//====================================================================================
{
    QApplication apMainApp(argc, argv);

    QString     qsCurrentPath = QDir::currentPath();

    if( !QFile::exists( QString("%1\\setup.qm").arg(qsCurrentPath) ) )
    {
        VRegistry   obReg;
        QString     qsPathWindows;

        if( obReg.OpenKey( HKEY_LOCAL_MACHINE, QString("SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion") ) )
        {
            qsPathWindows = obReg.get_REG_SZ( "SystemRoot" );
            obReg.CloseKey();

            QDir::setCurrent( QString("%1\\Temp\\BelenusInstall").arg(qsPathWindows) );
        }
    }

    QTranslator *poTrans = new QTranslator();

    poTrans->load( QString("setup.qm") );
    apMainApp.installTranslator( poTrans );

    bool bUninstall = false;

    if( argc > 1 && strcmp(argv[1],"-uninstall") == 0 )
    {
        bUninstall = true;
    }

    dlgMain w( 0, bUninstall );

    w.show();

    return apMainApp.exec();
}
//====================================================================================
