//====================================================================================
//
// Belenus Kliens alkalmazas (c) Pagony Multimedia Studio Bt - 2010
//
//====================================================================================
//
// Filename    : cassa.cpp
// AppVersion  : 1.0
// FileVersion : 1.0
// Author      : Bikfalvi Tamas
//
//====================================================================================
// Penztart es az azzal kapcsolodo reszeket kezelo osztaly.
//====================================================================================

#include <QMessageBox>
#include <QSqlQuery>
#include <QString>

#include "cassa.h"
#include "belenus.h"
#include "db/dbcassahistory.h"

//====================================================================================
cCassa::cCassa()
//====================================================================================
{
    m_pCassa                = new cDBCassa();
    m_pDenomination         = new cDBDenomination();
    m_pCassaDenomination    = new cDBCassaDenomination();

    m_bCassaEnabled         = false;
}
//====================================================================================
cCassa::~cCassa()
//====================================================================================
{
    if( m_pCassa ) delete m_pCassa;
    if( m_pDenomination ) delete m_pDenomination;
    if( m_pCassaDenomination ) delete m_pCassaDenomination;
}
//====================================================================================
bool cCassa::loadOpenCassa( unsigned int p_uiUserId )
//====================================================================================
{
    bool        bRet    = false;
    QSqlQuery  *poQuery = NULL;

    poQuery = g_poDB->executeQTQuery( QString( "SELECT * FROM cassa WHERE userId=%1 AND licenceId=%2 AND stopDateTime=\"0000-00-00 00:00:00\" ORDER BY cassaId DESC " ).arg(p_uiUserId).arg(g_poPrefs->getLicenceId()) );

    if( poQuery->first() )
    {
        m_pCassa->load( poQuery->value( 0 ).toUInt() );
        bRet = true;
    }
    if( poQuery ) delete poQuery;

    return bRet;
}
//====================================================================================
bool cCassa::loadOpenCassa()
//====================================================================================
{
    bool        bRet    = false;
    QSqlQuery  *poQuery = NULL;

    poQuery = g_poDB->executeQTQuery( QString( "SELECT * FROM cassa WHERE licenceId=%1 AND stopDateTime=\"0000-00-00 00:00:00\" ORDER BY cassaId DESC " ).arg(g_poPrefs->getLicenceId()) );

    if( poQuery->first() )
    {
        m_pCassa->load( poQuery->value( 0 ).toUInt() );
        bRet = true;
    }
    if( poQuery ) delete poQuery;

    return bRet;
}
//====================================================================================
bool cCassa::loadLatestCassa()
//====================================================================================
{
    bool        bRet    = false;
    QSqlQuery  *poQuery = NULL;

    poQuery = g_poDB->executeQTQuery( QString( "SELECT * FROM cassa WHERE licenceId=%1 ORDER BY cassaId DESC " ).arg(g_poPrefs->getLicenceId()) );

    if( poQuery->first() )
    {
        m_pCassa->load( poQuery->value( 0 ).toUInt() );
        bRet = true;
    }
    if( poQuery ) delete poQuery;

    return bRet;
}
//====================================================================================
void cCassa::createNew( unsigned int p_uiUserId, int p_inBalance )
//====================================================================================
{
    QSqlQuery *poQuery = NULL;

    try
    {
        m_pCassa->createNew();
        m_pCassa->setUserId( p_uiUserId );
        m_pCassa->setLicenceId( g_poPrefs->getLicenceId() );
        m_pCassa->setCurrentBalance( p_inBalance );
        m_pCassa->setStartDateTime( QDateTime::currentDateTime().toString( QString("yyyy-MM-dd hh:mm:ss") ) );
        m_pCassa->setActive( true );
        m_pCassa->save();

        poQuery = g_poDB->executeQTQuery( QString( "SELECT denominationId FROM denominations WHERE active=1" ) );
        while( poQuery->next() )
        {
            m_pCassaDenomination->createNew();
            m_pCassaDenomination->setDenominationId( poQuery->value( 0 ).toUInt() );
            m_pCassaDenomination->setCassaId( m_pCassa->id() );
            m_pCassaDenomination->setLicenceId( g_poPrefs->getLicenceId() );
            m_pCassaDenomination->save();
        }

        cDBCassaHistory obDBCassaHistory;

        obDBCassaHistory.setLicenceId( g_poPrefs->getLicenceId() );
        obDBCassaHistory.setCassaId( m_pCassa->id() );
        obDBCassaHistory.setUserId( p_uiUserId );
        obDBCassaHistory.setActionValue( 0 );
        obDBCassaHistory.setActionBalance( p_inBalance );
        obDBCassaHistory.setComment( QObject::tr("Open new cassa record.") );
        obDBCassaHistory.setActive( true );
        obDBCassaHistory.save();

        setEnabled();
    }
    catch( cSevException &e )
    {
            g_obLogger(e.severity()) << e.what() << EOM;
    }

    if( poQuery ) delete poQuery;
}
//====================================================================================
void cCassa::cassaContinue()
//====================================================================================
{
    cDBCassaHistory obDBCassaHistory;

    obDBCassaHistory.setLicenceId( m_pCassa->licenceId() );
    obDBCassaHistory.setCassaId( m_pCassa->id() );
    obDBCassaHistory.setUserId( g_obUser.id() );
    obDBCassaHistory.setActionValue( 0 );
    obDBCassaHistory.setActionBalance( m_pCassa->currentBalance() );
    obDBCassaHistory.setComment( QObject::tr("Continue cassa record.") );
    obDBCassaHistory.setActive( true );
    obDBCassaHistory.save();

    setEnabled();
}
//====================================================================================
void cCassa::cassaContinue( unsigned int p_uiUserId )
//====================================================================================
{
    int inCurrentBalance = m_pCassa->currentBalance();

    m_pCassa->setStopDateTime( QDateTime::currentDateTime().toString( QString("yyyy-MM-dd hh:mm:ss") ) );
    m_pCassa->save();

    cDBCassaHistory obDBCassaHistory;

    obDBCassaHistory.setLicenceId( m_pCassa->licenceId() );
    obDBCassaHistory.setCassaId( m_pCassa->id() );
    obDBCassaHistory.setUserId( m_pCassa->userId() );
    obDBCassaHistory.setActionValue( 0 );
    obDBCassaHistory.setActionBalance( m_pCassa->currentBalance() );
    obDBCassaHistory.setComment( QObject::tr("Close cassa record.") );
    obDBCassaHistory.setActive( true );
    obDBCassaHistory.save();

    createNew( p_uiUserId, inCurrentBalance );
}
//====================================================================================
void cCassa::cassaReOpen()
//====================================================================================
{
    m_pCassa->setStopDateTime( "" );
    m_pCassa->save();

    cDBCassaHistory obDBCassaHistory;

    obDBCassaHistory.setLicenceId( m_pCassa->licenceId() );
    obDBCassaHistory.setCassaId( m_pCassa->id() );
    obDBCassaHistory.setUserId( m_pCassa->userId() );
    obDBCassaHistory.setActionValue( 0 );
    obDBCassaHistory.setActionBalance( m_pCassa->currentBalance() );
    obDBCassaHistory.setComment( QObject::tr("Reopen cassa record.") );
    obDBCassaHistory.setActive( true );
    obDBCassaHistory.save();

    setEnabled();
}
//====================================================================================
void cCassa::cassaReOpen( unsigned int p_uiCassaId )
//====================================================================================
{
    m_pCassa->load( p_uiCassaId );
    cassaReOpen();
}
//====================================================================================
void cCassa::cassaClose()
//====================================================================================
{
    m_pCassa->setStopDateTime( QDateTime::currentDateTime().toString( QString("yyyy-MM-dd hh:mm:ss") ) );
    m_pCassa->save();

    cDBCassaHistory obDBCassaHistory;

    obDBCassaHistory.setLicenceId( m_pCassa->licenceId() );
    obDBCassaHistory.setCassaId( m_pCassa->id() );
    obDBCassaHistory.setUserId( g_obUser.id() );
    obDBCassaHistory.setActionValue( 0 );
    obDBCassaHistory.setActionBalance( m_pCassa->currentBalance() );
    obDBCassaHistory.setComment( QObject::tr("Close cassa record.") );
    obDBCassaHistory.setActive( true );
    obDBCassaHistory.save();
}
//====================================================================================
void cCassa::cassaIncreaseMoney( int p_nMoney, QString p_qsComment )
//====================================================================================
{
    QString m_qsComment = QObject::tr("Add money to cassa.");

    if( p_qsComment.length() > 0 )
    {
        m_qsComment += " - ";
        m_qsComment += p_qsComment;
    }

    m_pCassa->setCurrentBalance( m_pCassa->currentBalance()+p_nMoney );
    m_pCassa->save();

    cDBCassaHistory obDBCassaHistory;

    obDBCassaHistory.setLicenceId( g_poPrefs->getLicenceId() );
    obDBCassaHistory.setCassaId( m_pCassa->id() );
    obDBCassaHistory.setUserId( g_obUser.id() );
    obDBCassaHistory.setActionValue( p_nMoney );
    obDBCassaHistory.setActionBalance( m_pCassa->currentBalance() );
    obDBCassaHistory.setComment( m_qsComment );
    obDBCassaHistory.setActive( true );
    obDBCassaHistory.save();
}
//====================================================================================
void cCassa::cassaDecreaseMoney( int p_nMoney, QString p_qsComment )
//====================================================================================
{
    QString m_qsComment = QObject::tr("Remove money from cassa.");

    if( p_qsComment.length() > 0 )
    {
        m_qsComment += " - ";
        m_qsComment += p_qsComment;
    }

    m_pCassa->setCurrentBalance( m_pCassa->currentBalance()-p_nMoney );
    m_pCassa->save();

    cDBCassaHistory obDBCassaHistory;

    obDBCassaHistory.setLicenceId( g_poPrefs->getLicenceId() );
    obDBCassaHistory.setCassaId( m_pCassa->id() );
    obDBCassaHistory.setUserId( g_obUser.id() );
    obDBCassaHistory.setActionValue( -p_nMoney );
    obDBCassaHistory.setActionBalance( m_pCassa->currentBalance() );
    obDBCassaHistory.setComment( m_qsComment );
    obDBCassaHistory.setActive( true );
    obDBCassaHistory.save();
}
//====================================================================================
void cCassa::cassaAddMoneyAction( int p_nMoney, QString p_qsComment )
//====================================================================================
{
    m_pCassa->setCurrentBalance( m_pCassa->currentBalance()+p_nMoney );
    m_pCassa->save();

    cDBCassaHistory obDBCassaHistory;

    obDBCassaHistory.setLicenceId( g_poPrefs->getLicenceId() );
    obDBCassaHistory.setCassaId( m_pCassa->id() );
    obDBCassaHistory.setUserId( g_obUser.id() );
    obDBCassaHistory.setActionValue( p_nMoney );
    obDBCassaHistory.setActionBalance( m_pCassa->currentBalance() );
    obDBCassaHistory.setComment( p_qsComment );
    obDBCassaHistory.setActive( true );
    obDBCassaHistory.save();
}
//====================================================================================
void cCassa::setEnabled()
//====================================================================================
{
    m_bCassaEnabled = true;
}
//====================================================================================
void cCassa::setDisabled()
//====================================================================================
{
    m_bCassaEnabled = false;
}
//====================================================================================
bool cCassa::isCassaEnabled()
//====================================================================================
{
    return m_bCassaEnabled;
}
//====================================================================================
QString cCassa::cassaOwnerStr()
//====================================================================================
{
    cDBUser     obUser;

    obUser.load( m_pCassa->userId() );

    return obUser.realName();
}
//====================================================================================
int cCassa::cassaBalance()
//====================================================================================
{
    return m_pCassa->currentBalance();
}
//====================================================================================
unsigned int cCassa::cassaId()
//====================================================================================
{
    return m_pCassa->id();
}
//====================================================================================
