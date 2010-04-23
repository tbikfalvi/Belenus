#include "dlghardwaretest.h"

cDlgHardwareTest::cDlgHardwareTest( QWidget *p_poParent )
    : QDialog( p_poParent )
{
    cTracer obTrace( "cDlgHardwareTest::cDlgHardwareTest" );

    setupUi( this );

// __TO_BE_RESOLVED__
/*
    pbtComTest->setEnabled( false );

    char    stTemp[10];
    int     m_inSelectedComPort = 0;

    if( g_poHardware->SP_IsComPortOpened( &m_inSelectedComPort ) )
    {
        lblComTestResult->setText( tr( "Port opened" ) );
    }
    else
    {
        lblComTestResult->setText( tr( "Port unavailable" ) );
    }

    for( int i=0;i<g_poHardware->SP_GetCountAvailablePorts(); i++ )
    {
        memset( stTemp, 0, 10 );
        sprintf( stTemp, "COM%d", g_poHardware->SP_GetComPort( i ) );
        cmbComPort->addItem( stTemp );
        // megnyitott com port kivalasztasa a listabol __TO_BE_RESOLVED__
    }

    if( g_poHardware->SP_GetCountAvailablePorts() > 0 )
    {
        pbtComTest->setEnabled( true );
    }
*/
}

cDlgHardwareTest::~cDlgHardwareTest()
{
    cTracer obTrace( "cDlgHardwareTest::~cDlgHardwareTest" );
}

void cDlgHardwareTest::on_pbtTestHardwareConnection_clicked()
{
    cTracer obTrace( "cDlgHardwareTest::on_pbtTestHardwareConnection_clicked" );

// __TO_BE_RESOLVED__
/*
    int     m_inSelectedComPort = 0;

    if( g_poHardware->SP_IsComPortOpened( &m_inSelectedComPort ) )
    {
        QString stTemp = QString( tr( "Search hardware on port COM%1" ) ).arg( m_inSelectedComPort );

        lblHWConnectionResult->setText( stTemp );
    }
*/
}
