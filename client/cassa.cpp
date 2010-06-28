//====================================================================================
//
// Belenus Kliens alkalmazas © Pagony Multimedia Studio Bt - 2010
//
//====================================================================================
//
// Filename    : cassa.cpp
// AppVersion  : 1.0
// FileVersion : 1.0
// Author      : Bikfalvi Tamas
//
//====================================================================================
// Penztart es az azzal kapcsolodo reszeket kezelo allomany.
//====================================================================================

#include <QMessageBox>

#include "cassa.h"
#include "belenus.h"

//====================================================================================

cCassa::cCassa()
{
    m_pCassa                = new cDBCassa();
    m_pDenomination         = new cDBDenomination();
    m_pCassaDenomination    = new cDBCassaDenomination();

    m_bCassaEnabled         = false;
}

cCassa::~cCassa()
{
    if( m_pCassa ) delete m_pCassa;
    if( m_pDenomination ) delete m_pDenomination;
    if( m_pCassaDenomination ) delete m_pCassaDenomination;
}

void cCassa::init( QWidget *parent )
{
    try
    {
        m_pCassa->load();
    }
    catch( cSevException &e )
    {
        if( QString(e.what()).compare("Cassa table empty") != 0 )
        {
            g_obLogger << e.severity();
            g_obLogger << e.what() << cQTLogger::EOM;
        }
        else
        {
            m_pCassa->createNew();
        }
    }
}

bool cCassa::isCassaExists()
{
    return m_pCassa->id();
}

void cCassa::createNew( unsigned int p_uiUserId )
{
    m_pCassa->createNew();
    m_pCassa->setUserId( p_uiUserId );
    m_pCassa->setLicenceId( g_poPrefs->getLicenceId() );
    m_pCassa->setCurrentBalance( 0 );
    m_pCassa->setStartDateTime( QDateTime::currentDateTime().toString( QString("yyyy-MM-dd hh:mm:ss") ).toStdString() );
    m_pCassa->setActive( true );
    m_pCassa->save();

    QSqlQuery *poQuery;

    try
    {
        poQuery = g_poDB->executeQTQuery( QString( "SELECT denominationId FROM denominations WHERE active=1" ) );
        while( poQuery->next() )
        {
            m_pCassaDenomination->createNew();
            m_pCassaDenomination->setDenominationId( poQuery->value( 0 ).toUInt() );
            m_pCassaDenomination->setCassaId( m_pCassa->id() );
            m_pCassaDenomination->setLicenceId( g_poPrefs->getLicenceId() );
            m_pCassaDenomination->save();
        }
        cassaEnabled();
    }
    catch( cSevException &e )
    {
        g_obLogger << e.severity();
        g_obLogger << e.what() << cQTLogger::EOM;
    }

    if( poQuery ) delete poQuery;
}

bool cCassa::isCassaClosed()
{
    if( QString::fromStdString( m_pCassa->stopDateTime() ).length() == 0 )
        return false;
    else
        return true;
}

unsigned int cCassa::cassaOwner()
{
    return m_pCassa->userId();
}

void cCassa::cassaReOpen()
{
    m_pCassa->setStopDateTime( "" );
    m_pCassa->save();
    cassaEnabled();
}

void cCassa::cassaClose()
{
    m_pCassa->setStopDateTime( QDateTime::currentDateTime().toString( QString("yyyy-MM-dd hh:mm:ss") ).toStdString() );
    m_pCassa->save();
}

void cCassa::cassaEnabled()
{
    m_bCassaEnabled = true;
}

void cCassa::cassaDisabled()
{
    m_bCassaEnabled = false;
}

bool cCassa::isCassaEnabled()
{
    return m_bCassaEnabled;
}

unsigned int cCassa::cassaId()
{
    return m_pCassa->id();
}

void cCassa::cassaIncreaseMoney( int p_nMoney )
{
    m_pCassa->setCurrentBalance( m_pCassa->currentBalance()+p_nMoney );
    m_pCassa->save();
}

void cCassa::cassaDecreaseMoney( int p_nMoney )
{
    m_pCassa->setCurrentBalance( m_pCassa->currentBalance()-p_nMoney );
    m_pCassa->save();
}
