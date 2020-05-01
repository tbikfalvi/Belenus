//====================================================================================
//
// Belenus Kliens alkalmazas (c) Pagony Multimedia Studio Bt - 2010
//
//====================================================================================
//
// Filename    : dlginformation.cpp
// AppVersion  : 1.0
// FileVersion : 1.0
// Author      : Bikfalvi Tamas
//
//====================================================================================

//====================================================================================

#include "dlginformation.h"

//====================================================================================
cDlgInformation::cDlgInformation( QWidget *p_poParent ) : QDialog( p_poParent )
//------------------------------------------------------------------------------------
{
    setupUi( this );

    setWindowIcon( QIcon("./resources/40x40_information.png") );
    setWindowFlags( Qt::Dialog | Qt::FramelessWindowHint );

    m_poParent          = p_poParent;
    m_nTimerCountdown   = 0;
}
//====================================================================================
cDlgInformation::~cDlgInformation()
//------------------------------------------------------------------------------------
{
}
//====================================================================================
void cDlgInformation::timerEvent(QTimerEvent *)
//------------------------------------------------------------------------------------
{
    m_nTimerCountdown--;

    pbOk->setText( tr( "Ok (%1)" ).arg( m_nTimerCountdown ) );

    if( m_nTimerCountdown < 1)
    {
        killTimer( m_nTimer );
        m_nTimer = 0;

        close();
    }
}
//====================================================================================
void cDlgInformation::setInformationTitle(QString p_qsTitle)
//------------------------------------------------------------------------------------
{
    lblTitle->setVisible( false );

    lblTitle->setText( p_qsTitle );

    if( p_qsTitle.length() > 0 )
        lblTitle->setVisible( true );
}
//====================================================================================
void cDlgInformation::setInformationText(QString p_qsInformation)
//------------------------------------------------------------------------------------
{
    lblInformation->setText( p_qsInformation );
}
//====================================================================================
void cDlgInformation::setTimer(int p_nSeconds)
//------------------------------------------------------------------------------------
{
    if( p_nSeconds == 0 )
        return;

    m_nTimerCountdown = p_nSeconds;

    if( m_nTimerCountdown > 0)
    {
        pbOk->setText( tr( "Ok (%1)" ).arg( m_nTimerCountdown ) );
        m_nTimer = startTimer( 1000 );
    }
}
//====================================================================================
void cDlgInformation::on_pbOk_clicked()
//------------------------------------------------------------------------------------
{
    close();
}
//====================================================================================
