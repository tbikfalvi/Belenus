//====================================================================================
//
// Belenus Kliens alkalmazas © Pagony Multimedia Studio Bt - 2010
//
//====================================================================================
//
// Filename    : ledger.cpp
// AppVersion  : 1.0
// FileVersion : 1.0
// Author      : Bikfalvi Tamas
//
//====================================================================================
// Konyvelest es az azzal kapcsolodo reszeket kezelo osztaly.
//====================================================================================

#include "ledger.h"
#include "belenus.h"
#include "db/dbpatientcard.h"
#include "db/dbpatientcardtype.h"

//====================================================================================
cLedger::cLedger()
{
    m_pLedger       = new cDBLedger();
    m_pLedgerDevice = new cDBLedgerDevice();
}

cLedger::~cLedger()
{
    if( m_pLedger ) delete m_pLedger;
    if( m_pLedgerDevice ) delete m_pLedgerDevice;
}

void cLedger::createNewLedger()
{
    m_pLedger->createNew();
}

void cLedger::sellingDeviceUsage( unsigned int p_uiPanelId )
{
    m_pLedger->setLedgerTypeId( 1 );
    m_pLedger->setPanelId( p_uiPanelId );

    QSqlQuery  *poQuery = NULL;
    try
    {
        poQuery = g_poDB->executeQTQuery( QString( "SELECT title from panels WHERE panelId=%1" ).arg( p_uiPanelId ) );
        if( poQuery->size() )
        {
            poQuery->first();
            m_pLedger->setName( poQuery->value( 0 ).toString().toStdString() );
        }
        delete poQuery;
    }
    catch( cSevException &e )
    {
        g_obLogger << e.severity() << e.what() << cQTLogger::EOM;

        if( poQuery ) delete poQuery;
    }
}

void cLedger::sellingPatientCard( unsigned int p_uiPatientCardId )
{
    cDBPatientCard      obPatientCard;
    cDBPatientCardType  obPatientCardType;

    obPatientCard.load( p_uiPatientCardId );
    obPatientCardType.load( obPatientCard.patientCardTypeId() );

    m_pLedger->setLedgerTypeId( 2 );
    m_pLedger->setPatientCardTypeId( obPatientCard.patientCardTypeId() );
    m_pLedger->setName( obPatientCard.barcode() );
    m_pLedger->setNetPrice( obPatientCardType.price() );
    m_pLedger->setVatpercent( obPatientCardType.vatpercent() );
}

void cLedger::refillPatientCard( unsigned int p_uiPatientCardId )
{
    sellingPatientCard( p_uiPatientCardId );
    m_pLedger->setLedgerTypeId( 3 );
}

void cLedger::sellingProduct( unsigned int p_uiProductId )
{
    m_pLedger->setLedgerTypeId( 4 );
}

void cLedger::prepareLedger( int p_inNetPrice, int p_inVatpercent, QString p_qsComment )
{
    m_pLedger->setNetPrice( p_inNetPrice );
    m_pLedger->setVatpercent( p_inVatpercent );
    m_pLedger->setComment( p_qsComment.toStdString() );
}

void cLedger::prepareLedger( QString p_qsComment )
{
    m_pLedger->setComment( p_qsComment.toStdString() );
}

void cLedger::saveLedger()
{
    m_pLedger->setLicenceId( g_poPrefs->getLicenceId() );
    m_pLedger->setUserId( g_obUser.id() );

    m_pLedger->save();
}

void cLedger::createNewLedgerDevice(  unsigned int p_uiPanelId )
{
    m_pLedgerDevice->createNew();

    m_pLedgerDevice->setPanelId( p_uiPanelId );
}

void cLedger::prepareLedgerDevice( unsigned int p_uiPatientCardId,
                                   int p_inUnits,
                                   int p_inCash,
                                   int p_inTimeLength )
{
    if( p_uiPatientCardId > 0 )
    {
        if( m_pLedgerDevice->patientCardId() == 0 ) m_pLedgerDevice->setPatientCardId( p_uiPatientCardId );

        m_pLedgerDevice->setUnits( m_pLedgerDevice->units()+p_inUnits );
        m_pLedgerDevice->setTimeCard( m_pLedgerDevice->timeCard()+p_inTimeLength );
    }
    else if( p_inCash > 0 )
    {
        m_pLedgerDevice->setTimeCash( m_pLedgerDevice->timeCash()+p_inTimeLength );
    }
    m_pLedgerDevice->setTimeReal( m_pLedgerDevice->timeReal()+p_inTimeLength );
}

void cLedger::addLedgerDevice( int p_inUnits,
                               int p_inCash,
                               int p_inTimeLength )
{
    if( p_inUnits > 0 )
    {
        m_pLedgerDevice->setUnits( m_pLedgerDevice->units()+p_inUnits );
        m_pLedgerDevice->setTimeCard( m_pLedgerDevice->timeCard()+p_inTimeLength );
    }
    else if( p_inCash > 0 )
    {
        m_pLedgerDevice->setTimeCash( m_pLedgerDevice->timeCash()+p_inTimeLength );
    }
    m_pLedgerDevice->setTimeReal( m_pLedgerDevice->timeReal()+p_inTimeLength );
}

void cLedger::finishLedgerDevice( int p_inTimeLength,
                                  int p_inTimeLeft )
{
    m_pLedgerDevice->setTimeReal( p_inTimeLength );

    if( p_inTimeLeft > 0 )
    {
        m_pLedgerDevice->setTimeLeft( p_inTimeLeft );

        if( p_inTimeLeft > m_pLedgerDevice->timeCash() )
        {
            p_inTimeLeft -= m_pLedgerDevice->timeCash();
            m_pLedgerDevice->setTimeCash( 0 );
            m_pLedgerDevice->setTimeCard( m_pLedgerDevice->timeCard()-p_inTimeLeft );
        }
    }
}

void cLedger::setLedgerDeviceComment( QString p_qsComment )
{
    m_pLedgerDevice->setComment( p_qsComment.toStdString() );
}

void cLedger::saveLedgerDevice()
{
    m_pLedgerDevice->setLicenceId( g_poPrefs->getLicenceId() );
    m_pLedgerDevice->setUserId( g_obUser.id() );

    m_pLedgerDevice->save();
}

