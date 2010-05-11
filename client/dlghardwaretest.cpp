
#include "dlghardwaretest.h"
#ifdef __WIN32__
    #include "windows.h"
#endif

cDlgHardwareTest::cDlgHardwareTest( QWidget *p_poParent )
    : QDialog( p_poParent )
{
    cTracer obTrace( "cDlgHardwareTest::cDlgHardwareTest" );

    setupUi( this );

    pbTestHardwareConnection->setIcon( QIcon("./resources/40x40_hardware_test.gif") );
    pbReconnect->setIcon( QIcon("./resources/40x40_hadware_reconnect.gif") );
    pbExit->setIcon( QIcon("./resources/40x40_exit.gif") );

    on_pbTestHardwareConnection_clicked();

    g_poHardware->setTestMode( true );
}

cDlgHardwareTest::~cDlgHardwareTest()
{
    cTracer obTrace( "cDlgHardwareTest::~cDlgHardwareTest" );

    g_poHardware->setTestMode( false );
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

void cDlgHardwareTest::on_pbRelayRunTest_clicked()
{
    for( int i=0; i<12+1; i++ )
    {
        g_poHardware->setRelayOn( i );
        g_poHardware->HW_Kezel();
#ifdef __WIN32__
        Sleep(500);
#else
        sleep( 1 );
#endif
    }
    for( int i=0; i<12+1; i++ )
    {
        g_poHardware->setRelayOff( i );
        g_poHardware->HW_Kezel();
#ifdef __WIN32__
        Sleep(500);
#else
        sleep( 1 );
#endif
    }
}

void cDlgHardwareTest::on_rbRelay01_On_toggled(bool checked)
{
    if( checked )
        g_poHardware->setRelayOn( 1 );
    else
        g_poHardware->setRelayOff( 1 );

    g_poHardware->HW_Kezel();
}

void cDlgHardwareTest::on_rbRelay02_On_toggled(bool checked)
{
    if( checked )
        g_poHardware->setRelayOn( 2 );
    else
        g_poHardware->setRelayOff( 2 );

    g_poHardware->HW_Kezel();

}

void cDlgHardwareTest::on_rbRelay03_On_toggled(bool checked)
{
    if( checked )
        g_poHardware->setRelayOn( 3 );
    else
        g_poHardware->setRelayOff( 3 );

    g_poHardware->HW_Kezel();
}

void cDlgHardwareTest::on_rbRelay04_On_toggled(bool checked)
{
    if( checked )
        g_poHardware->setRelayOn( 4 );
    else
        g_poHardware->setRelayOff( 4 );

    g_poHardware->HW_Kezel();
}

void cDlgHardwareTest::on_rbRelay05_On_toggled(bool checked)
{
    if( checked )
        g_poHardware->setRelayOn( 5 );
    else
        g_poHardware->setRelayOff( 5 );

    g_poHardware->HW_Kezel();
}

void cDlgHardwareTest::on_rbRelay06_On_toggled(bool checked)
{
    if( checked )
        g_poHardware->setRelayOn( 6 );
    else
        g_poHardware->setRelayOff( 6 );

    g_poHardware->HW_Kezel();
}

void cDlgHardwareTest::on_rbRelay07_On_toggled(bool checked)
{
    if( checked )
        g_poHardware->setRelayOn( 7 );
    else
        g_poHardware->setRelayOff( 7 );

    g_poHardware->HW_Kezel();
}

void cDlgHardwareTest::on_rbRelay08_on_toggled(bool checked)
{
    if( checked )
        g_poHardware->setRelayOn( 8 );
    else
        g_poHardware->setRelayOff( 8 );

    g_poHardware->HW_Kezel();
}

void cDlgHardwareTest::on_rbRelay09_on_toggled(bool checked)
{
    if( checked )
        g_poHardware->setRelayOn( 9 );
    else
        g_poHardware->setRelayOff( 9 );

    g_poHardware->HW_Kezel();
}

void cDlgHardwareTest::on_rbRelay10_On_toggled(bool checked)
{
    if( checked )
        g_poHardware->setRelayOn( 10 );
    else
        g_poHardware->setRelayOff( 10 );

    g_poHardware->HW_Kezel();
}

void cDlgHardwareTest::on_rbRelay11_on_toggled(bool checked)
{
    if( checked )
        g_poHardware->setRelayOn( 11 );
    else
        g_poHardware->setRelayOff( 11 );

    g_poHardware->HW_Kezel();
}

void cDlgHardwareTest::on_rbRelay12_on_toggled(bool checked)
{
    if( checked )
        g_poHardware->setRelayOn( 12 );
    else
        g_poHardware->setRelayOff( 12 );

    g_poHardware->HW_Kezel();
}
