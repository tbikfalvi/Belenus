
#include "dlghardwaretest.h"
#ifdef __WIN32__
    #include "windows.h"
#endif

cDlgHardwareTest::cDlgHardwareTest( QWidget *p_poParent )
    : QDialog( p_poParent )
{
    cTracer obTrace( "cDlgHardwareTest::cDlgHardwareTest" );

    setupUi( this );

    m_nRelayCount   = 0;
    m_bModuleOn     = true;
    m_nModelCount   = 0;

    setWindowTitle( tr( "Hardware Test" ) );
    setWindowIcon( QIcon("./resources/40x40_device_settings.png") );

    lblModuleButtonTestResult->setText( "" );

    pbTestHardwareConnection->setIcon( QIcon("./resources/40x40_hardware_test.png") );
    pbReconnect->setIcon( QIcon("./resources/40x40_hadware_reconnect.png") );

    qvLblStatus.push_back( lblStatus01 );
    qvLblStatus.push_back( lblStatus02 );
    qvLblStatus.push_back( lblStatus03 );
    qvLblStatus.push_back( lblStatus04 );
    qvLblStatus.push_back( lblStatus05 );
    qvLblStatus.push_back( lblStatus06 );
    qvLblStatus.push_back( lblStatus07 );
    qvLblStatus.push_back( lblStatus08 );
    qvLblStatus.push_back( lblStatus09 );
    qvLblStatus.push_back( lblStatus10 );
    qvLblStatus.push_back( lblStatus11 );
    qvLblStatus.push_back( lblStatus12 );
    qvLblStatus.push_back( lblStatus13 );
    qvLblStatus.push_back( lblStatus14 );

    qvLblOk.push_back( lblModulOk01 );
    qvLblOk.push_back( lblModulOk02 );
    qvLblOk.push_back( lblModulOk03 );
    qvLblOk.push_back( lblModulOk04 );
    qvLblOk.push_back( lblModulOk05 );
    qvLblOk.push_back( lblModulOk06 );
    qvLblOk.push_back( lblModulOk07 );
    qvLblOk.push_back( lblModulOk08 );
    qvLblOk.push_back( lblModulOk09 );
    qvLblOk.push_back( lblModulOk10 );
    qvLblOk.push_back( lblModulOk11 );
    qvLblOk.push_back( lblModulOk12 );
    qvLblOk.push_back( lblModulOk13 );
    qvLblOk.push_back( lblModulOk14 );

    qvLblFail.push_back( lblModulFail01 );
    qvLblFail.push_back( lblModulFail02 );
    qvLblFail.push_back( lblModulFail03 );
    qvLblFail.push_back( lblModulFail04 );
    qvLblFail.push_back( lblModulFail05 );
    qvLblFail.push_back( lblModulFail06 );
    qvLblFail.push_back( lblModulFail07 );
    qvLblFail.push_back( lblModulFail08 );
    qvLblFail.push_back( lblModulFail09 );
    qvLblFail.push_back( lblModulFail10 );
    qvLblFail.push_back( lblModulFail11 );
    qvLblFail.push_back( lblModulFail12 );
    qvLblFail.push_back( lblModulFail13 );
    qvLblFail.push_back( lblModulFail14 );

    for( int i=0; i<14; i++ )
    {
        qvLblOk.at(i)->setVisible( false );
        qvLblFail.at(i)->setVisible( false );
        nCounterOk[i] = 0;
        nCounterFail[i] = 0;
        qvLblStatus.at(i)->setText( QString( "%1 / %2" ).arg( nCounterOk[i] ).arg( nCounterFail[i] ) );
    }

    pbSwitchRelay01->setIcon( QIcon("./resources/77x40_off.png") );
    pbSwitchRelay02->setIcon( QIcon("./resources/77x40_off.png") );
    pbSwitchRelay03->setIcon( QIcon("./resources/77x40_off.png") );
    pbSwitchRelay04->setIcon( QIcon("./resources/77x40_off.png") );
    pbSwitchRelay05->setIcon( QIcon("./resources/77x40_off.png") );
    pbSwitchRelay06->setIcon( QIcon("./resources/77x40_off.png") );
    pbSwitchRelay07->setIcon( QIcon("./resources/77x40_off.png") );
    pbSwitchRelay08->setIcon( QIcon("./resources/77x40_off.png") );
    pbSwitchRelay09->setIcon( QIcon("./resources/77x40_off.png") );
    pbSwitchRelay10->setIcon( QIcon("./resources/77x40_off.png") );
    pbSwitchRelay11->setIcon( QIcon("./resources/77x40_off.png") );
    pbSwitchRelay12->setIcon( QIcon("./resources/77x40_off.png") );

    pbModulOnOff->setIcon( QIcon("./resources/77x40_on.png") );

    pbStopModuleTest->setIcon( QIcon("./resources/40x40_stop.png") );
    pbStopModuleButtonTest->setIcon( QIcon("./resources/40x40_stop.png") );

    pbTestModules->setEnabled( true );
    pbStopModuleTest->setEnabled( false );

    pbExit->setIcon( QIcon("./resources/40x40_exit.png") );

    timerHWTest = new QTimer(this);
    connect(timerHWTest, SIGNAL(timeout()), this, SLOT(updateHWStatus()));
    timerRelayTest = new QTimer(this);
    connect(timerRelayTest, SIGNAL(timeout()), this, SLOT(updateRelayStatus()));
    timerModulTest = new QTimer(this);
    connect(timerModulTest, SIGNAL(timeout()), this, SLOT(slotTestModules()) );
    timerModulButtonTest = new QTimer(this);
    connect(timerModulButtonTest, SIGNAL(timeout()), this, SLOT(slotTestModulButtons()) );

    m_nRelayCount = 0;

    g_poHardware->setTestMode( true );

    on_pbTestHardwareConnection_clicked();
    on_pbInitHardware_clicked();

    timerHWTest->start(250);
}

cDlgHardwareTest::~cDlgHardwareTest()
{
    cTracer obTrace( "cDlgHardwareTest::~cDlgHardwareTest" );

    timerHWTest->stop();
    timerRelayTest->stop();
    timerModulTest->stop();

    g_poHardware->setTestMode( false );
}

void cDlgHardwareTest::updateHWStatus()
{
    if( g_poHardware->getRelayStatus( 1 ) )
    {
        rbRelay01_On->setChecked(true);
        pbSwitchRelay01->setIcon( QIcon("./resources/77x40_on.png") );
    }
    else
    {
        rbRelay01_On->setChecked(false);
        pbSwitchRelay01->setIcon( QIcon("./resources/77x40_off.png") );
    }
    if( g_poHardware->getRelayStatus( 2 ) )
    {
        rbRelay02_On->setChecked(true);
        pbSwitchRelay02->setIcon( QIcon("./resources/77x40_on.png") );
    }
    else
    {
        rbRelay02_On->setChecked(false);
        pbSwitchRelay02->setIcon( QIcon("./resources/77x40_off.png") );
    }
    if( g_poHardware->getRelayStatus( 3 ) )
    {
        rbRelay03_On->setChecked(true);
        pbSwitchRelay03->setIcon( QIcon("./resources/77x40_on.png") );
    }
    else
    {
        rbRelay03_On->setChecked(false);
        pbSwitchRelay03->setIcon( QIcon("./resources/77x40_off.png") );
    }
    if( g_poHardware->getRelayStatus( 4 ) )
    {
        rbRelay04_On->setChecked(true);
        pbSwitchRelay04->setIcon( QIcon("./resources/77x40_on.png") );
    }
    else
    {
        rbRelay04_On->setChecked(false);
        pbSwitchRelay04->setIcon( QIcon("./resources/77x40_off.png") );
    }
    if( g_poHardware->getRelayStatus( 5 ) )
    {
        rbRelay05_On->setChecked(true);
        pbSwitchRelay05->setIcon( QIcon("./resources/77x40_on.png") );
    }
    else
    {
        rbRelay05_On->setChecked(false);
        pbSwitchRelay05->setIcon( QIcon("./resources/77x40_off.png") );
    }
    if( g_poHardware->getRelayStatus( 6 ) )
    {
        rbRelay06_On->setChecked(true);
        pbSwitchRelay06->setIcon( QIcon("./resources/77x40_on.png") );
    }
    else
    {
        rbRelay06_On->setChecked(false);
        pbSwitchRelay06->setIcon( QIcon("./resources/77x40_off.png") );
    }
    if( g_poHardware->getRelayStatus( 7 ) )
    {
        rbRelay07_On->setChecked(true);
        pbSwitchRelay07->setIcon( QIcon("./resources/77x40_on.png") );
    }
    else
    {
        rbRelay07_On->setChecked(false);
        pbSwitchRelay07->setIcon( QIcon("./resources/77x40_off.png") );
    }
    if( g_poHardware->getRelayStatus( 8 ) )
    {
        rbRelay08_On->setChecked(true);
        pbSwitchRelay08->setIcon( QIcon("./resources/77x40_on.png") );
    }
    else
    {
        rbRelay08_On->setChecked(false);
        pbSwitchRelay08->setIcon( QIcon("./resources/77x40_off.png") );
    }
    if( g_poHardware->getRelayStatus( 9 ) )
    {
        rbRelay09_On->setChecked(true);
        pbSwitchRelay09->setIcon( QIcon("./resources/77x40_on.png") );
    }
    else
    {
        rbRelay09_On->setChecked(false);
        pbSwitchRelay09->setIcon( QIcon("./resources/77x40_off.png") );
    }
    if( g_poHardware->getRelayStatus( 10 ) )
    {
        rbRelay10_On->setChecked(true);
        pbSwitchRelay10->setIcon( QIcon("./resources/77x40_on.png") );
    }
    else
    {
        rbRelay10_On->setChecked(false);
        pbSwitchRelay10->setIcon( QIcon("./resources/77x40_off.png") );
    }
    if( g_poHardware->getRelayStatus( 11 ) )
    {
        rbRelay11_On->setChecked(true);
        pbSwitchRelay11->setIcon( QIcon("./resources/77x40_on.png") );
    }
    else
    {
        rbRelay11_On->setChecked(false);
        pbSwitchRelay11->setIcon( QIcon("./resources/77x40_off.png") );
    }
    if( g_poHardware->getRelayStatus( 12 ) )
    {
        rbRelay12_On->setChecked(true);
        pbSwitchRelay12->setIcon( QIcon("./resources/77x40_on.png") );
    }
    else
    {
        rbRelay12_On->setChecked(false);
        pbSwitchRelay12->setIcon( QIcon("./resources/77x40_off.png") );
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

    lblHWConnectionResult->setStyleSheet( "QLabel {font: normal;}" );

    if( g_poHardware->isHardwareConnected() )
    {
        lblHWConnectionResult->setStyleSheet( "QLabel {font: bold; color: green;}" );
        lblHWConnectionResult->setText( tr("Hardware available") );
    }
    else
    {
        lblHWConnectionResult->setStyleSheet( "QLabel {font: bold; color: red;}" );
        lblHWConnectionResult->setText( tr("Hardware unavailable") );
    }
}

void cDlgHardwareTest::on_pbReconnect_clicked()
{
    g_obLogger(cSeverity::DEBUG) << "Closing HW connection ..." << EOM;
    lblHWConnectionResult->setStyleSheet( "QLabel {font: bold; color: red;}" );
    lblHWConnectionResult->setText( tr("Hardware unavailable") );
    Sleep( 2000 );
    g_poHardware->closeCommunication();
    g_obLogger(cSeverity::DEBUG) << "Restoring HW connection ..." << EOM;
    g_poHardware->init( g_poPrefs->getCommunicationPort() );
    on_pbTestHardwareConnection_clicked();
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
    if( !rbRelay01_On->isChecked() )
        g_poHardware->setRelayOn( 1 );
    else
        g_poHardware->setRelayOff( 1 );

    g_poHardware->HW_Kezel();
}

void cDlgHardwareTest::on_pbSwitchRelay02_clicked()
{
    if( !rbRelay02_On->isChecked() )
        g_poHardware->setRelayOn( 2 );
    else
        g_poHardware->setRelayOff( 2 );

    g_poHardware->HW_Kezel();
}

void cDlgHardwareTest::on_pbSwitchRelay03_clicked()
{
    if( !rbRelay03_On->isChecked() )
        g_poHardware->setRelayOn( 3 );
    else
        g_poHardware->setRelayOff( 3 );

    g_poHardware->HW_Kezel();
}

void cDlgHardwareTest::on_pbSwitchRelay04_clicked()
{
    if( !rbRelay04_On->isChecked() )
        g_poHardware->setRelayOn( 4 );
    else
        g_poHardware->setRelayOff( 4 );

    g_poHardware->HW_Kezel();
}

void cDlgHardwareTest::on_pbSwitchRelay05_clicked()
{
    if( !rbRelay05_On->isChecked() )
        g_poHardware->setRelayOn( 5 );
    else
        g_poHardware->setRelayOff( 5 );

    g_poHardware->HW_Kezel();
}

void cDlgHardwareTest::on_pbSwitchRelay06_clicked()
{
    if( !rbRelay06_On->isChecked() )
        g_poHardware->setRelayOn( 6 );
    else
        g_poHardware->setRelayOff( 6 );

    g_poHardware->HW_Kezel();
}

void cDlgHardwareTest::on_pbSwitchRelay07_clicked()
{
    if( !rbRelay07_On->isChecked() )
        g_poHardware->setRelayOn( 7 );
    else
        g_poHardware->setRelayOff( 7 );

    g_poHardware->HW_Kezel();
}

void cDlgHardwareTest::on_pbSwitchRelay08_clicked()
{
    if( !rbRelay08_On->isChecked() )
        g_poHardware->setRelayOn( 8 );
    else
        g_poHardware->setRelayOff( 8 );

    g_poHardware->HW_Kezel();
}

void cDlgHardwareTest::on_pbSwitchRelay09_clicked()
{
    if( !rbRelay09_On->isChecked() )
        g_poHardware->setRelayOn( 9 );
    else
        g_poHardware->setRelayOff( 9 );

    g_poHardware->HW_Kezel();
}

void cDlgHardwareTest::on_pbSwitchRelay10_clicked()
{
    if( !rbRelay10_On->isChecked() )
        g_poHardware->setRelayOn( 10 );
    else
        g_poHardware->setRelayOff( 10 );

    g_poHardware->HW_Kezel();
}

void cDlgHardwareTest::on_pbSwitchRelay11_clicked()
{
    if( !rbRelay11_On->isChecked() )
        g_poHardware->setRelayOn( 11 );
    else
        g_poHardware->setRelayOff( 11 );

    g_poHardware->HW_Kezel();
}

void cDlgHardwareTest::on_pbSwitchRelay12_clicked()
{
    if( !rbRelay12_On->isChecked() )
        g_poHardware->setRelayOn( 12 );
    else
        g_poHardware->setRelayOff( 12 );

    g_poHardware->HW_Kezel();
}

void cDlgHardwareTest::on_pbAddressModules_clicked()
{
    g_obLogger(cSeverity::DEBUG) << "Set address of moduls ..." << EOM;
    bool bRet = g_poHardware->HW_SetModuleAddress();
    g_obLogger(cSeverity::DEBUG) << "Set address of moduls " << (bRet?"SUCCEEDED":"FAILED") << EOM;
}

void cDlgHardwareTest::on_pbInitHardware_clicked()
{
    if( g_poHardware->isHardwareConnected() )
    {
        m_nModelCount = g_poHardware->getPanelCount();

        if( m_nModelCount > 14 )
            m_nModelCount = 14;

        for( int i=0; i<m_nModelCount; i++ )
        {
            if( g_poHardware->checkHardwarePanel( i ) )
            {
                qvLblOk.at(i)->setVisible( true );
                qvLblFail.at(i)->setVisible( false );
            }
            else
            {
                qvLblOk.at(i)->setVisible( false );
                qvLblFail.at(i)->setVisible( true );
            }
        }
    }
}

void cDlgHardwareTest::on_pbTestModules_clicked()
{
    for( int i=0; i<14; i++ )
    {
        nCounterOk[i] = 0;
        nCounterFail[i] = 0;
    }
    m_nTime = 0;
    timerModulTest->start( 500 );

    pbTestModules->setEnabled( false );
    pbStopModuleTest->setEnabled( true );
}

void cDlgHardwareTest::on_pbModulOnOff_clicked()
{
    if( m_bModuleOn )
    {
        m_bModuleOn = false;
        pbModulOnOff->setIcon( QIcon("./resources/77x40_off.png") );
        g_poHardware->ModuleTurnOff();
    }
    else
    {
        m_bModuleOn = true;
        pbModulOnOff->setIcon( QIcon("./resources/77x40_on.png") );
        g_poHardware->ModuleTurnOn();
    }
}

void cDlgHardwareTest::slotTestModules()
{
    m_nTime++;

    if( m_nTime > 99 ) m_nTime = 1;

    for( int i=0; i<m_nModelCount; i++ )
    {
        if( g_poHardware->setMainActionTime( i, m_nTime*60, true ) )
        {
            nCounterOk[i]++;
        }
        else
        {
            nCounterFail[i]++;
        }
        qvLblStatus.at(i)->setText( QString( "%1 / %2" ).arg( nCounterOk[i] ).arg( nCounterFail[i] ) );
    }
}

void cDlgHardwareTest::on_pbStopModuleTest_clicked()
{
    timerModulTest->stop();

    for( int i=0; i<g_poHardware->getPanelCount(); i++ )
    {
        g_poHardware->setMainActionTime( i, 0, true );
    }

    pbTestModules->setEnabled( true );
    pbStopModuleTest->setEnabled( false );
}

void cDlgHardwareTest::on_pbTestModuleButtons_clicked()
{
    pbTestModuleButtons->setEnabled( false );
    pbStopModuleButtonTest->setEnabled( true );
    timerModulButtonTest->start( 250 );
}

void cDlgHardwareTest::on_pbStopModuleButtonTest_clicked()
{
    timerModulButtonTest->stop();
    pbTestModuleButtons->setEnabled( true );
    pbStopModuleButtonTest->setEnabled( false );
}

void cDlgHardwareTest::slotTestModulButtons()
{
    //g_poHardware->EnableModulIRQ();
    lblModuleButtonTestResult->setText( tr("Not implemented yet") );
}
