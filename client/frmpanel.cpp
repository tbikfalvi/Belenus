//====================================================================================
//
// Belenus Kliens alkalmazas (c) Pagony Multimedia Studio Bt - 2010
//
//====================================================================================
//
// Filename    : frmpanel.cpp
// AppVersion  : 1.0
// FileVersion : 1.0
// Author      : Ballok Peter, Bikfalvi Tamas
//
//====================================================================================
// Panelok kezeleset vegzo osztaly
//====================================================================================

#include <QPalette>

//====================================================================================

#include "belenus.h"
#include "frmpanel.h"
#include "db/dbpatientcard.h"

//====================================================================================
cFrmPanel::cFrmPanel( const unsigned int p_uiPanelId )
    : QFrame()
{
    cTracer obTrace( "cFrmPanel::cFrmPanel" );

    verticalLayout   = new QVBoxLayout( this );
    lblTitle         = new QLabel( this );
    lblCurrStatus    = new QLabel( this );
    lblCurrTimer     = new QLabel( this );
    lblNextStatusLen = new QLabel( this );
    lblInfo          = new QLabel( this );
    spacer1          = new QSpacerItem( 20, 15, QSizePolicy::Minimum, QSizePolicy::Expanding );
    spacer2          = new QSpacerItem( 20, 50, QSizePolicy::Minimum, QSizePolicy::Expanding );
    spacer3          = new QSpacerItem( 20, 20, QSizePolicy::Minimum, QSizePolicy::Expanding );
    spacer4          = new QSpacerItem( 20, 120, QSizePolicy::Minimum, QSizePolicy::Expanding );

    verticalLayout->setContentsMargins( 0, 0, 0, 0 );
    verticalLayout->addWidget( lblTitle );
    verticalLayout->addItem( spacer1 );
    verticalLayout->addWidget( lblCurrStatus );
    verticalLayout->addItem( spacer2 );
    verticalLayout->addWidget( lblCurrTimer );
    verticalLayout->addWidget( lblNextStatusLen );
    verticalLayout->addItem( spacer3 );
    verticalLayout->addWidget( lblInfo );
    verticalLayout->addItem( spacer4 );

    setAutoFillBackground( true );

    lblTitle->setAutoFillBackground( true );
    lblTitle->setContentsMargins( 0, 5, 0, 5 );
    lblTitle->setAlignment( Qt::AlignCenter );

    m_uiId                  = 0;
    m_uiType                = 0;
    m_uiStatus              = 0;
    m_uiCounter             = 0;
    m_inTimerId             = 0;

    m_inMainProcessLength   = 0;
    m_inCashToPay           = 0;
    m_bHasToPay             = false;

    m_vrPatientCard.clear();

    m_pDBLedgerDevice       = new cDBLedgerDevice();

    m_pDBLedgerDevice->createNew();

    load( p_uiPanelId );

    inactivate();
    displayStatus();
}
//====================================================================================
cFrmPanel::~cFrmPanel()
{
    cTracer obTrace( "cFrmPanel::~cFrmPanel" );

    if( m_pDBLedgerDevice ) delete m_pDBLedgerDevice;

    for( unsigned int i = 0; i < m_obStatuses.size(); i ++ ) if( m_obStatuses.at( i ) ) delete m_obStatuses.at( i );
}
//====================================================================================
bool cFrmPanel::isWorking() const
{
    return (m_uiStatus > 0);
}
//====================================================================================
bool cFrmPanel::isStatusCanBeSkipped()
{
    bool bRet = false;

    if( m_obStatuses.at(m_uiStatus)->activateCommand() != 3 && m_uiStatus > 0 )
        bRet = true;

    return bRet;
}
//====================================================================================
void cFrmPanel::start()
{
    if( m_inMainProcessLength == 0 )
        return;

//    stringstream ssTrace;
//    ssTrace << "Id: " << m_uiId;
//    cTracer obTrace( "cFrmPanel::start", ssTrace.str() );

    for( unsigned int i = 0; i < m_obStatuses.size(); i ++ )
    {
        if( m_obStatuses.at( i )->activateCommand() == 3 )
        {
            m_obStatuses.at( i )->setLength( m_inMainProcessLength );
        }
    }

    m_pDBLedgerDevice->setLicenceId( g_poPrefs->getLicenceId() );
    m_pDBLedgerDevice->setUserId( g_obUser.id() );
    m_pDBLedgerDevice->setPanelId( m_uiId );
    m_pDBLedgerDevice->setPatientId( g_obPatient.id() );
    m_pDBLedgerDevice->setActive( true );

    activateNextStatus();
    m_inTimerId = startTimer( 1000 );
}
//====================================================================================
void cFrmPanel::reset()
{
//    stringstream ssTrace;
//    ssTrace << "Id: " << m_uiId;
//    cTracer obTrace( "cFrmPanel::reset", ssTrace.str() );

    if( isMainProcess() )
    {
        closeAttendance();
    }

    m_pDBLedgerDevice->createNew();
    m_vrPatientCard.clear();

    m_inMainProcessLength = 0;
    if( !m_bHasToPay )
    {
        m_inCashToPay = 0;
    }

    m_uiStatus = m_obStatuses.size() - 1;
    activateNextStatus();
}
//====================================================================================
void cFrmPanel::next()
{
    activateNextStatus();
}
//====================================================================================
void cFrmPanel::inactivate()
{
    setFrameShadow( QFrame::Sunken );

    QPalette  obNewPalette = lblTitle->palette();
    obNewPalette.setBrush( QPalette::Window, QBrush( QColor( "#b9b9b9") ) );
    lblTitle->setPalette( obNewPalette );
}
//====================================================================================
void cFrmPanel::activate()
{
    setFrameShadow( QFrame::Raised );

    QPalette  obNewPalette = lblTitle->palette();
    obNewPalette.setBrush( QPalette::Window, QBrush( QColor( "#4387cb" ) ) );
    lblTitle->setPalette( obNewPalette );
}
//====================================================================================
int cFrmPanel::mainProcessTime()
{
    return m_inMainProcessLength;
}
//====================================================================================
void cFrmPanel::setMainProcessTime( const int p_inLength )
{
    m_inMainProcessLength += p_inLength;
    m_pDBLedgerDevice->setTimeReal( m_pDBLedgerDevice->timeReal()+p_inLength );

    displayStatus();
}
//====================================================================================
void cFrmPanel::setMainProcessTime( const int p_inLength, const int p_inPrice )
{
    m_inCashToPay += p_inPrice;

    m_pDBLedgerDevice->setCash( m_inCashToPay );
    m_pDBLedgerDevice->setTimeCash( m_pDBLedgerDevice->timeCash()+p_inLength );

    setMainProcessTime( p_inLength );
}
//====================================================================================
void cFrmPanel::setMainProcessTime( const unsigned int p_uiPatientCardId, const int p_inCountUnits, const int p_inLength )
{
    stUsedPatientCard   obTemp;

    obTemp.uiPatientCardId  = p_uiPatientCardId;
    obTemp.inCountUnits     = p_inCountUnits;
    obTemp.inUnitTime       = p_inLength;

    m_vrPatientCard.push_back( obTemp );

    m_pDBLedgerDevice->setUnits( m_pDBLedgerDevice->units()+p_inCountUnits );
    m_pDBLedgerDevice->setTimeCard( m_pDBLedgerDevice->timeCard()+p_inLength );

    setMainProcessTime( p_inLength );
}
//====================================================================================
bool cFrmPanel::isTimeIntervallValid( const int p_inLength, int *p_inPrice )
{
    QSqlQuery   *poQuery;
    bool         bRet = false;

    *p_inPrice = 0;

    poQuery = g_poDB->executeQTQuery( QString( "SELECT usePrice FROM panelUses WHERE panelId=%1 AND useTime=%2" ).arg(m_uiId).arg(p_inLength) );
    if( poQuery->first() )
    {
        *p_inPrice = poQuery->value( 0 ).toInt();
        bRet = true;
    }
    if( poQuery ) delete poQuery;

    return bRet;
}
//====================================================================================
void cFrmPanel::mousePressEvent ( QMouseEvent * p_poEvent )
{
    emit panelClicked( m_uiId - 1 );
    p_poEvent->ignore();
}
//====================================================================================
void cFrmPanel::timerEvent ( QTimerEvent * )
{
    if( g_poHardware->isHardwareMovedNextStatus( m_uiId-1 ) )
    {
        g_poHardware->setHardwareMovedNextStatus( m_uiId-1 );
        m_uiCounter = 0;
    }
    if( g_poHardware->isHardwareStopped( m_uiId-1 ) )
    {
        lblCurrStatus->setText( m_obStatuses.at( m_uiStatus )->name() + tr("\n<< STOPPED >>") );
    }
    else
    {
        lblCurrStatus->setText( m_obStatuses.at( m_uiStatus )->name() );
    }

    if( m_uiCounter )
    {
        m_uiCounter--;

        if( isMainProcess() )
            m_inMainProcessLength--;

        lblCurrTimer->setText( QString( "%1:%2" ).arg( m_uiCounter / 60, 2, 10, QChar( '0' ) ).arg( m_uiCounter % 60, 2, 10, QChar( '0' ) ) );
    }
    else
    {
        activateNextStatus();
    }
    g_poHardware->setCounter( m_uiId-1, (int)m_uiCounter );
}
//====================================================================================
void cFrmPanel::load( const unsigned int p_uiPanelId )
{
    m_uiId = p_uiPanelId;

    QSqlQuery  *poQuery = NULL;
    try
    {
        poQuery = g_poDB->executeQTQuery( QString( "SELECT panelTypeId, title from panels WHERE panelId=%1" ).arg( m_uiId ) );
        if( poQuery->size() )
        {
            poQuery->first();
            m_uiType = poQuery->value( 0 ).toInt();
            lblTitle->setText( poQuery->value( 1 ).toString() );
        }
        else
        {
            lblTitle->setText( tr("Panel Not Found in Database") );
        }

        delete poQuery;
        poQuery = NULL;

        poQuery = g_poDB->executeQTQuery( QString( "SELECT panelStatusId, panelTypeId, seqNumber from panelStatuses WHERE panelTypeId=%1 ORDER BY seqNumber" ).arg( m_uiType ) );
        while( poQuery->next() )
        {
            unsigned int uiStatusId = poQuery->value( 0 ).toInt();

            cDBPanelStatuses  *poStatus = new cDBPanelStatuses();
            poStatus->load( uiStatusId );
            m_obStatuses.push_back( poStatus );
        }

        delete poQuery;
    }
    catch( cSevException &e )
    {
        g_obLogger(e.severity()) << e.what() << cQTLogger::EOM;
        if( poQuery ) delete poQuery;
    }
}
//====================================================================================
void cFrmPanel::reload()
{
    QSqlQuery  *poQuery = NULL;
    try
    {
        poQuery = g_poDB->executeQTQuery( QString( "SELECT panelTypeId, title from panels WHERE panelId=%1" ).arg( m_uiId ) );
        if( poQuery->size() )
        {
            poQuery->first();
            lblTitle->setText( poQuery->value( 1 ).toString() );
        }
        delete poQuery;
    }
    catch( cSevException &e )
    {
        g_obLogger(e.severity()) << e.what() << cQTLogger::EOM;
        if( poQuery ) delete poQuery;
    }
}
//====================================================================================
void cFrmPanel::displayStatus()
{
    if( m_uiStatus )
    {
        lblCurrStatus->setText( m_obStatuses.at( m_uiStatus )->name() );

        lblCurrTimer->setText( QString( "%1:%2" ).arg( m_uiCounter / 60, 2, 10, QChar( '0' ) ).arg( m_uiCounter % 60, 2, 10, QChar( '0' ) ) );
        unsigned int uiNextLen = 0;
        if( m_uiStatus != m_obStatuses.size() - 1 )
        {
            uiNextLen = m_obStatuses.at( m_uiStatus + 1 )->length();
            lblNextStatusLen->setText( QString( "%1:%2" ).arg( uiNextLen / 60, 2, 10, QChar( '0' ) ).arg( uiNextLen % 60, 2, 10, QChar( '0' ) ) );
        }
        else
        {
            lblNextStatusLen->setText( "" );
        }
    }
    else
    {
        lblCurrStatus->setText( "" );
        if( m_inMainProcessLength > 0 )
            lblCurrTimer->setText( QString( "%1:%2" ).arg( m_inMainProcessLength / 60, 2, 10, QChar( '0' ) ).arg( m_inMainProcessLength % 60, 2, 10, QChar( '0' ) ) );
        else
            lblCurrTimer->setText( "" );
        lblNextStatusLen->setText( "" );
    }

    QString qsInfo = "";

    if( m_inCashToPay > 0 )
    {
        qsInfo += tr("Cash to pay: ") + convertCurrency( m_inCashToPay, g_poPrefs->getCurrencyShort() );
    }

    lblInfo->setText( qsInfo );

    // A kovetkezo reszt at kell irni, ha keszen lesz a dinamikus
    // stilus valtas statuszonkent
    QPalette  obFramePalette = palette();
    switch( m_uiStatus )
    {
        case 0:
            obFramePalette.setBrush( QPalette::Window, QBrush( Qt::green ) );
            break;
        case 1:
            obFramePalette.setBrush( QPalette::Window, QBrush( Qt::yellow ) );
            break;
        case 2:
            obFramePalette.setBrush( QPalette::Window, QBrush( Qt::cyan ) );
            break;
        case 3:
            obFramePalette.setBrush( QPalette::Window, QBrush( Qt::red ) );
            break;
        case 4:
            obFramePalette.setBrush( QPalette::Window, QBrush( Qt::yellow ) );
            break;
    }
    setPalette( obFramePalette );

    QFont   obFont;

    lblCurrStatus->setAlignment( Qt::AlignCenter );
    obFont = lblCurrStatus->font();
    obFont.setBold( true );
    obFont.setCapitalization( QFont::AllUppercase );
    obFont.setPixelSize( 18 );
    lblCurrStatus->setFont( obFont );

    lblCurrTimer->setAlignment( Qt::AlignCenter );
    obFont = lblCurrTimer->font();
    obFont.setBold( true );
    obFont.setPixelSize( 30 );
    lblCurrTimer->setFont( obFont );

    lblNextStatusLen->setAlignment( Qt::AlignCenter );

    lblInfo->setAlignment( Qt::AlignCenter );
    obFont = lblInfo->font();
    obFont.setBold( true );
    obFont.setPixelSize( 15 );
    lblInfo->setFont( obFont );
}
//====================================================================================
QString cFrmPanel::convertCurrency( int p_nCurrencyValue, QString p_qsCurrency )
{
    QString qsValue = QString::number( p_nCurrencyValue );
    QString qsRet = "";

    if( qsValue.length() > 3 )
    {
        while( qsValue.length() > 3 )
        {
            qsRet.insert( 0, qsValue.right(3) );
            qsRet.insert( 0, g_poPrefs->getCurrencySeparator() );
            qsValue.truncate( qsValue.length()-3 );
        }
    }
    qsRet.insert( 0, qsValue );
    qsRet += " " + p_qsCurrency;

    return qsRet;
}
//====================================================================================
void cFrmPanel::activateNextStatus()
{
    if( isMainProcess() )
    {
        // Kezeles vege
        closeAttendance();
    }

    m_uiStatus++;

    if( m_uiStatus == m_obStatuses.size() )
    {
        m_uiStatus  = 0;
        m_uiCounter = 0;
        killTimer( m_inTimerId );
        g_poHardware->setCurrentCommand( m_uiId-1, 0 );
    }
    else
    {
        m_uiCounter = m_obStatuses.at( m_uiStatus )->length();
        g_poHardware->setCurrentCommand( m_uiId-1, m_obStatuses.at( m_uiStatus )->activateCommand() );
    }

    displayStatus();
}
//====================================================================================
void cFrmPanel::cashPayed()
{
    m_inCashToPay = 0;
    m_bHasToPay = false;

    displayStatus();
}
//====================================================================================
bool cFrmPanel::isMainProcess()
{
    return ( m_obStatuses.at(m_uiStatus)->activateCommand()==3 ? true : false );
}
//====================================================================================
void cFrmPanel::closeAttendance()
{
    if( m_inCashToPay > 0 )
    {
        m_bHasToPay = true;
    }
    m_pDBLedgerDevice->setTimeLeft( m_inMainProcessLength );
    m_pDBLedgerDevice->setTimeReal( m_pDBLedgerDevice->timeReal()-m_inMainProcessLength );
    if( m_inMainProcessLength > 0 )
    {
        m_pDBLedgerDevice->setComment( tr("Device usage stopped after %1 minutes. Unused time: %2 minutes.").arg(m_pDBLedgerDevice->timeReal()).arg(m_pDBLedgerDevice->timeLeft()) );
    }
    m_pDBLedgerDevice->save();

    for( unsigned int i=0; i < m_vrPatientCard.size(); i++ )
    {
        cDBPatientCard  obDBPatientCard;

        obDBPatientCard.load( m_vrPatientCard.at(i).uiPatientCardId );

        obDBPatientCard.setUnits( obDBPatientCard.units()-m_vrPatientCard.at(i).inCountUnits );

        QTime m_qtTemp = QTime::fromString( obDBPatientCard.timeLeft(), "hh:mm:ss" );
        m_qtTemp = m_qtTemp.addSecs( -m_vrPatientCard.at(i).inUnitTime );
        obDBPatientCard.setTimeLeft( m_qtTemp.toString("hh:mm:ss") );

        obDBPatientCard.save();
    }

    m_pDBLedgerDevice->createNew();
}
//====================================================================================
