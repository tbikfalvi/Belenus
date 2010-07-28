
#include "dlghardwaretest.h"
#ifdef __WIN32__
    #include "windows.h"
#endif

cDlgHardwareTest::cDlgHardwareTest( QWidget *p_poParent )
    : QDialog( p_poParent )
{
    cTracer obTrace( "cDlgHardwareTest::cDlgHardwareTest" );

    setupUi( this );

    pbTestHardwareConnection->setIcon( QIcon("./resources/40x40_hardware_test.png") );
    pbReconnect->setIcon( QIcon("./resources/40x40_hadware_reconnect.png") );
    pbExit->setIcon( QIcon("./resources/40x40_exit.png") );

    on_pbTestHardwareConnection_clicked();

    timerHWTest = new QTimer(this);
    connect(timerHWTest, SIGNAL(timeout()), this, SLOT(updateHWStatus()));
    timerRelayTest = new QTimer(this);
    connect(timerRelayTest, SIGNAL(timeout()), this, SLOT(updateRelayStatus()));

    m_nRelayCount = 0;

    g_poHardware->setTestMode( true );

    timerHWTest->start(200);
}

cDlgHardwareTest::~cDlgHardwareTest()
{
    cTracer obTrace( "cDlgHardwareTest::~cDlgHardwareTest" );

    timerHWTest->stop();

    g_poHardware->setTestMode( false );
}

void cDlgHardwareTest::updateHWStatus()
{
    if( g_poHardware->getRelayStatus( 1 ) )
    {
        rbRelay01_On->setChecked(true);
        rbRelay01_On->setText( "ON" );
        pbSwitchRelay01->setText( "OFF" );
    }
    else
    {
        rbRelay01_On->setChecked(false);
        rbRelay01_On->setText( "OFF" );
        pbSwitchRelay01->setText( "ON" );
    }
    if( g_poHardware->getRelayStatus( 2 ) )
    {
        rbRelay02_On->setChecked(true);
        rbRelay02_On->setText( "ON" );
        pbSwitchRelay02->setText( "OFF" );
    }
    else
    {
        rbRelay02_On->setChecked(false);
        rbRelay02_On->setText( "OFF" );
        pbSwitchRelay02->setText( "ON" );
    }
    if( g_poHardware->getRelayStatus( 3 ) )
    {
        rbRelay03_On->setChecked(true);
        rbRelay03_On->setText( "ON" );
        pbSwitchRelay03->setText( "OFF" );
    }
    else
    {
        rbRelay03_On->setChecked(false);
        rbRelay03_On->setText( "OFF" );
        pbSwitchRelay03->setText( "ON" );
    }
    if( g_poHardware->getRelayStatus( 4 ) )
    {
        rbRelay04_On->setChecked(true);
        rbRelay04_On->setText( "ON" );
        pbSwitchRelay04->setText( "OFF" );
    }
    else
    {
        rbRelay04_On->setChecked(false);
        rbRelay04_On->setText( "OFF" );
        pbSwitchRelay04->setText( "ON" );
    }
    if( g_poHardware->getRelayStatus( 5 ) )
    {
        rbRelay05_On->setChecked(true);
        rbRelay05_On->setText( "ON" );
        pbSwitchRelay05->setText( "OFF" );
    }
    else
    {
        rbRelay05_On->setChecked(false);
        rbRelay05_On->setText( "OFF" );
        pbSwitchRelay05->setText( "ON" );
    }
    if( g_poHardware->getRelayStatus( 6 ) )
    {
        rbRelay06_On->setChecked(true);
        rbRelay06_On->setText( "ON" );
        pbSwitchRelay06->setText( "OFF" );
    }
    else
    {
        rbRelay06_On->setChecked(false);
        rbRelay06_On->setText( "OFF" );
        pbSwitchRelay06->setText( "ON" );
    }
    if( g_poHardware->getRelayStatus( 7 ) )
    {
        rbRelay07_On->setChecked(true);
        rbRelay07_On->setText( "ON" );
        pbSwitchRelay07->setText( "OFF" );
    }
    else
    {
        rbRelay07_On->setChecked(false);
        rbRelay07_On->setText( "OFF" );
        pbSwitchRelay07->setText( "ON" );
    }
    if( g_poHardware->getRelayStatus( 8 ) )
    {
        rbRelay08_On->setChecked(true);
        rbRelay08_On->setText( "ON" );
        pbSwitchRelay08->setText( "OFF" );
    }
    else
    {
        rbRelay08_On->setChecked(false);
        rbRelay08_On->setText( "OFF" );
        pbSwitchRelay08->setText( "ON" );
    }
    if( g_poHardware->getRelayStatus( 9 ) )
    {
        rbRelay09_On->setChecked(true);
        rbRelay09_On->setText( "ON" );
        pbSwitchRelay09->setText( "OFF" );
    }
    else
    {
        rbRelay09_On->setChecked(false);
        rbRelay09_On->setText( "OFF" );
        pbSwitchRelay09->setText( "ON" );
    }
    if( g_poHardware->getRelayStatus( 10 ) )
    {
        rbRelay10_On->setChecked(true);
        rbRelay10_On->setText( "ON" );
        pbSwitchRelay10->setText( "OFF" );
    }
    else
    {
        rbRelay10_On->setChecked(false);
        rbRelay10_On->setText( "OFF" );
        pbSwitchRelay10->setText( "ON" );
    }
    if( g_poHardware->getRelayStatus( 11 ) )
    {
        rbRelay11_On->setChecked(true);
        rbRelay11_On->setText( "ON" );
        pbSwitchRelay11->setText( "OFF" );
    }
    else
    {
        rbRelay11_On->setChecked(false);
        rbRelay11_On->setText( "OFF" );
        pbSwitchRelay11->setText( "ON" );
    }
    if( g_poHardware->getRelayStatus( 12 ) )
    {
        rbRelay12_On->setChecked(true);
        rbRelay12_On->setText( "ON" );
        pbSwitchRelay12->setText( "OFF" );
    }
    else
    {
        rbRelay12_On->setChecked(false);
        rbRelay12_On->setText( "OFF" );
        pbSwitchRelay12->setText( "ON" );
    }
}

void cDlgHardwareTest::updateRelayStatus()
{
    if( m_nRelayCount > 0 && m_nRelayCount < 13 )
        g_poHardware->setRelayOn( m_nRelayCount );
    else if( m_nRelayCount > 12 && m_nRelayCount < 25 )
        g_poHardware->setRelayOff( m_nRelayCount-12 );

    g_poHardware->HW_Kezel();
    m_nRelayCount++;

    if( m_nRelayCount > 24 )
        timerRelayTest->stop();
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
    m_nRelayCount = 1;
    timerRelayTest->start(500);
}

void cDlgHardwareTest::on_pbSwitchRelay01_clicked()
{
    if( pbSwitchRelay01->text() == "ON" )
        g_poHardware->setRelayOn( 1 );
    else
        g_poHardware->setRelayOff( 1 );

    g_poHardware->HW_Kezel();
}

void cDlgHardwareTest::on_pbSwitchRelay02_clicked()
{
    if( pbSwitchRelay02->text() == "ON" )
        g_poHardware->setRelayOn( 2 );
    else
        g_poHardware->setRelayOff( 2 );

    g_poHardware->HW_Kezel();
}

void cDlgHardwareTest::on_pbSwitchRelay03_clicked()
{
    if( pbSwitchRelay03->text() == "ON" )
        g_poHardware->setRelayOn( 3 );
    else
        g_poHardware->setRelayOff( 3 );

    g_poHardware->HW_Kezel();
}

void cDlgHardwareTest::on_pbSwitchRelay04_clicked()
{
    if( pbSwitchRelay04->text() == "ON" )
        g_poHardware->setRelayOn( 4 );
    else
        g_poHardware->setRelayOff( 4 );

    g_poHardware->HW_Kezel();
}

void cDlgHardwareTest::on_pbSwitchRelay05_clicked()
{
    if( pbSwitchRelay05->text() == "ON" )
        g_poHardware->setRelayOn( 5 );
    else
        g_poHardware->setRelayOff( 5 );

    g_poHardware->HW_Kezel();
}

void cDlgHardwareTest::on_pbSwitchRelay06_clicked()
{
    if( pbSwitchRelay06->text() == "ON" )
        g_poHardware->setRelayOn( 6 );
    else
        g_poHardware->setRelayOff( 6 );

    g_poHardware->HW_Kezel();
}

void cDlgHardwareTest::on_pbSwitchRelay07_clicked()
{
    if( pbSwitchRelay07->text() == "ON" )
        g_poHardware->setRelayOn( 7 );
    else
        g_poHardware->setRelayOff( 7 );

    g_poHardware->HW_Kezel();
}

void cDlgHardwareTest::on_pbSwitchRelay08_clicked()
{
    if( pbSwitchRelay08->text() == "ON" )
        g_poHardware->setRelayOn( 8 );
    else
        g_poHardware->setRelayOff( 8 );

    g_poHardware->HW_Kezel();
}

void cDlgHardwareTest::on_pbSwitchRelay09_clicked()
{
    if( pbSwitchRelay09->text() == "ON" )
        g_poHardware->setRelayOn( 9 );
    else
        g_poHardware->setRelayOff( 9 );

    g_poHardware->HW_Kezel();
}

void cDlgHardwareTest::on_pbSwitchRelay10_clicked()
{
    if( pbSwitchRelay10->text() == "ON" )
        g_poHardware->setRelayOn( 10 );
    else
        g_poHardware->setRelayOff( 10 );

    g_poHardware->HW_Kezel();
}

void cDlgHardwareTest::on_pbSwitchRelay11_clicked()
{
    if( pbSwitchRelay11->text() == "ON" )
        g_poHardware->setRelayOn( 11 );
    else
        g_poHardware->setRelayOff( 11 );

    g_poHardware->HW_Kezel();
}

void cDlgHardwareTest::on_pbSwitchRelay12_clicked()
{
    if( pbSwitchRelay12->text() == "ON" )
        g_poHardware->setRelayOn( 12 );
    else
        g_poHardware->setRelayOff( 12 );

    g_poHardware->HW_Kezel();
}
