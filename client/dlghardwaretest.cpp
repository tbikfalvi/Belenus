#include "dlghardwaretest.h"

cDlgHardwareTest::cDlgHardwareTest( QWidget *p_poParent )
    : QDialog( p_poParent )
{
    cTracer obTrace( "cDlgHardwareTest::cDlgHardwareTest" );

    setupUi( this );

    pbTestHardwareConnection->setIcon( QIcon("./resources/40x40_hardware_test.gif") );
    pbReconnect->setIcon( QIcon("./resources/40x40_hadware_reconnect.gif") );
    pbExit->setIcon( QIcon("./resources/40x40_exit.gif") );

    on_pbTestHardwareConnection_clicked();
}

cDlgHardwareTest::~cDlgHardwareTest()
{
    cTracer obTrace( "cDlgHardwareTest::~cDlgHardwareTest" );
}

void cDlgHardwareTest::on_pbTestHardwareConnection_clicked()
{
    cTracer obTrace( "cDlgHardwareTest::on_pbtTestHardwareConnection_clicked" );

    if( g_poHardware->isHardwareConnected() )
    {
        lblHWConnectionResult->setText( tr("Hardware available") );
    }
    else
    {
        lblHWConnectionResult->setText( tr("Hardware unavailable") );
    }
}

void cDlgHardwareTest::on_pbReconnect_clicked()
{

}

void cDlgHardwareTest::on_pbExit_clicked()
{
    QDialog::accept();
}
