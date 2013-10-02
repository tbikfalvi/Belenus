//====================================================================================
//
// Belenus Kliens alkalmazas (c) Pagony Multimedia Studio Bt - 2010
//
//====================================================================================
//
// Filename    : dsppanel.cpp
// AppVersion  : 1.0
// FileVersion : 1.0
// Author      : Bikfalvi Tamas
//
//====================================================================================
// Panelok kezeleset vegzo osztaly
//====================================================================================

#include <QPalette>
#include <QMessageBox>
#include <QPixmap>

//====================================================================================

#include "belenus.h"
#include "dsppanel.h"

#include <iostream>

//====================================================================================
cDspPanel::cDspPanel( const unsigned int p_uiPanelId ) : QFrame()
{
    cTracer obTrace( "cDspPanel::cDspPanel" );

    m_uiId          = p_uiPanelId;

    setFrameShape( QFrame::Panel );
    setFrameShadow( QFrame::Raised );
    setLineWidth( 3 );
    setAutoFillBackground( true );

    verticalLayout  = new QVBoxLayout( this );
    lblTitle        = new QLabel( this );
    spacer1         = new QSpacerItem( 20, 5, QSizePolicy::Minimum, QSizePolicy::Minimum );
    lblCurrStatus   = new QLabel( this );
    spacer2         = new QSpacerItem( 20, 2, QSizePolicy::Minimum, QSizePolicy::Minimum );
    lblCurrTimer    = new QLabel( this );
    spacer3         = new QSpacerItem( 20, 2, QSizePolicy::Minimum, QSizePolicy::Minimum );
    lblEstTimer     = new QLabel( this );
    spacer4         = new QSpacerItem( 20, 2, QSizePolicy::Minimum, QSizePolicy::Minimum );
    lblInfo         = new QLabel( this );
    spacer5         = new QSpacerItem( 20, 5, QSizePolicy::Minimum, QSizePolicy::Minimum );

    verticalLayout->setContentsMargins( 0, 0, 0, 0 );

    lblTitle->setAutoFillBackground( true );
    lblTitle->setContentsMargins( 0, 5, 0, 5 );
    lblTitle->setAlignment( Qt::AlignCenter );
    QPalette  obNewPalette = lblTitle->palette();
    obNewPalette.setBrush( QPalette::Window, QBrush( QColor( "#0000FF" ) ) );
    lblTitle->setPalette( obNewPalette );
    lblTitle->setStyleSheet( "QLabel {font: bold; color: white; font-size:14px;}" );

    lblCurrStatus->setWordWrap( true );
    lblInfo->setWordWrap( true );

    verticalLayout->addWidget( lblTitle );
    verticalLayout->addItem( spacer1 );
    verticalLayout->addWidget( lblCurrStatus );
    verticalLayout->addItem( spacer2 );
    verticalLayout->addWidget( lblCurrTimer );
    verticalLayout->addItem( spacer3 );
    verticalLayout->addWidget( lblEstTimer );
    verticalLayout->addItem( spacer4 );
    verticalLayout->addWidget( lblInfo );
    verticalLayout->addItem( spacer5 );

    m_obDBPanelStatusSettings.createNew();

    _load();
    _displayStatus();
}
//====================================================================================
cDspPanel::~cDspPanel()
{
    cTracer obTrace( "cDspPanel::~cDspPanel" );
/*
    for( unsigned int i = 0; i < m_obStatuses.size(); i ++ ) if( m_obStatuses.at( i ) ) delete m_obStatuses.at( i );*/
}
//====================================================================================
void cDspPanel::refreshTitle()
{
    _load();
}
//====================================================================================
void cDspPanel::setPanelStatus( const unsigned int p_uiPanelStatusId )
{
    cTracer obTrace( "cDspPanel::setPanelStatus" );

    try
    {
        m_obDBPanelStatusSettings.load( p_uiPanelStatusId );

        _displayStatus();
    }
    catch( cSevException &e )
    {
        g_obLogger(e.severity()) << e.what() << EOM;

        m_obDBPanelStatusSettings.createNew();

        m_obDBPanelStatusSettings.setStatusFontName( "Arial" );
        m_obDBPanelStatusSettings.setStatusFontSize( 18 );
        m_obDBPanelStatusSettings.setStatusFontColor( "#000000" );

        m_obDBPanelStatusSettings.setTimerFontName( "Arial" );
        m_obDBPanelStatusSettings.setTimerFontSize( 30 );
        m_obDBPanelStatusSettings.setTimerFontColor( "#000000" );

        m_obDBPanelStatusSettings.setNextFontName( "Arial" );
        m_obDBPanelStatusSettings.setNextFontSize( 18 );
        m_obDBPanelStatusSettings.setNextFontColor( "#000000" );

        m_obDBPanelStatusSettings.setInfoFontName( "Arial" );
        m_obDBPanelStatusSettings.setInfoFontSize( 10 );
        m_obDBPanelStatusSettings.setInfoFontColor( "#000000" );
    }
}
//====================================================================================
void cDspPanel::setPanelStatusText( const QString &p_qsStatus )
{
    cTracer obTrace( "cDspPanel::setPanelStatusText" );

    _formatStatusString( p_qsStatus );
}
//====================================================================================
void cDspPanel::setPanelInfoText( const QString &p_qsInfo )
{
    cTracer obTrace( "cDspPanel::setPanelInfoText" );

    _formatInfoString( p_qsInfo );
}
//====================================================================================
void cDspPanel::setCounterText( const QString &p_qsCounter )
{
    cTracer obTrace( "cDspPanel::setCounterText" );

    _formatTimerString( p_qsCounter );
}

//====================================================================================
void cDspPanel::setPanelWaitTime( const unsigned int p_uiWaitTime )
{
    cTracer obTrace( "cDspPanel::setPanelWaitTime" );

    m_uiWaitTime = p_uiWaitTime;

    if( m_uiWaitTime )
    {
        _formatWaitTimeString( QString( "%1:%2" ).arg( m_uiWaitTime / 60, 2, 10, QChar( '0' ) ).arg( m_uiWaitTime % 60, 2, 10, QChar( '0' ) ) );
    }
    else
    {
        _formatWaitTimeString( "" );
    }
}
//====================================================================================
void cDspPanel::_load()
{
    cTracer obTrace( "cDspPanel::_load" );

    QSqlQuery  *poQuery = NULL;
    try
    {
        poQuery = g_poDB->executeQTQuery( QString( "SELECT panelTypeId, title from panels WHERE panelId=%1" ).arg( m_uiId ) );
        if( poQuery->first() )
        {
            g_obLogger(cSeverity::DEBUG) << poQuery->value( 1 ).toString() << EOM;
            lblTitle->setText( poQuery->value( 1 ).toString() );
        }
        else
        {
            lblTitle->setText( tr("Panel Not Found in Database") );
        }

        delete poQuery;
        poQuery = NULL;
    }
    catch( cSevException &e )
    {
        g_obLogger(e.severity()) << e.what() << EOM;
        if( poQuery ) delete poQuery;
    }
}
//====================================================================================
void cDspPanel::_displayStatus()
{
    QPalette  obFramePalette = palette();
    obFramePalette.setBrush( QPalette::Window, QBrush( QColor(m_obDBPanelStatusSettings.backgroundColor()) ) );
    setPalette( obFramePalette );
}
//====================================================================================
void cDspPanel::_formatStatusString( QString p_qsStatusText )
{
    cTracer obTrace( QString("cDspPanel::_formatStatusString - %1").arg(p_qsStatusText) );

    QFont   obFont;

    obFont = lblCurrStatus->font();
    obFont.setFamily( m_obDBPanelStatusSettings.statusFontName() );
    obFont.setPixelSize( m_obDBPanelStatusSettings.statusFontSize() );
    obFont.setBold( true );
    obFont.setCapitalization( QFont::AllUppercase );

    lblCurrStatus->setAlignment( Qt::AlignCenter );
    lblCurrStatus->setFont( obFont );
    lblCurrStatus->setText( QString("<font color=%1>%2</font>").arg(QColor( m_obDBPanelStatusSettings.statusFontColor()).name()).arg(p_qsStatusText) );
}
//====================================================================================
void cDspPanel::_formatTimerString( QString p_qsTimerText )
{
    cTracer obTrace( QString("cDspPanel::_formatTimerString - %1").arg(p_qsTimerText) );

    QFont   obFont;

    obFont = lblCurrTimer->font();
    obFont.setFamily( m_obDBPanelStatusSettings.timerFontName() );
    obFont.setPixelSize( m_obDBPanelStatusSettings.timerFontSize() );
    obFont.setBold( true );

    lblCurrTimer->setAlignment( Qt::AlignCenter );
    lblCurrTimer->setFont( obFont );
    lblCurrTimer->setText( QString("<font color=%1>%2</font>").arg(QColor( m_obDBPanelStatusSettings.timerFontColor()).name()).arg(p_qsTimerText) );
}
//====================================================================================
void cDspPanel::_formatWaitTimeString( QString p_qsTimerText )
{
    cTracer obTrace( QString("cDspPanel::_formatWaitTimeString - %1").arg(p_qsTimerText) );

    QFont   obFont;

    obFont = lblEstTimer->font();
    obFont.setFamily( m_obDBPanelStatusSettings.nextFontName() );
    obFont.setPixelSize( m_obDBPanelStatusSettings.nextFontSize() );
    obFont.setBold( true );

    lblEstTimer->setAlignment( Qt::AlignCenter );
    lblEstTimer->setFont( obFont );
    lblEstTimer->setText( QString("<font color=%1>%2</font>").arg(QColor( m_obDBPanelStatusSettings.nextFontColor()).name()).arg(p_qsTimerText) );
}
//====================================================================================
void cDspPanel::_formatInfoString( QString p_qsInfoText )
{
    cTracer obTrace( QString("cDspPanel::_formatInfoString - %1").arg(p_qsInfoText) );

    QFont   obFont;

    obFont = lblInfo->font();
    obFont.setFamily( m_obDBPanelStatusSettings.infoFontName() );
    obFont.setPixelSize( m_obDBPanelStatusSettings.infoFontSize() );
    obFont.setBold( true );

    lblInfo->setAlignment( Qt::AlignCenter );
    lblInfo->setFont( obFont );
    lblInfo->setText( QString("<font color=%1>%2</font>").arg(QColor( m_obDBPanelStatusSettings.infoFontColor()).name()).arg(p_qsInfoText) );
}
//====================================================================================
