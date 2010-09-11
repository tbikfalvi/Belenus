#include "wndmain.h"
#include "dlgdemo.h"

cWndMain::cWndMain( QWidget *parent )
    : QMainWindow( parent )
{
    setupUi( this );
}

cWndMain::~cWndMain()
{
}

void cWndMain::on_action_Demo_triggered()
{
    cDlgDemo  obDlgDemo( this );

    obDlgDemo.exec();
}
