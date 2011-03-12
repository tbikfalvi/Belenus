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

//====================================================================================

#include "dlgMain.h"

//====================================================================================
int main(int argc, char *argv[])
//====================================================================================
{
    QApplication apMainApp(argc, argv);

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
