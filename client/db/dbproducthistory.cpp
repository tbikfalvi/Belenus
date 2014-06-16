//====================================================================================
//
// Belenus Server alkalmazas (c) Pagony Multimedia Studio Bt - 2010
//
//====================================================================================
//
// Filename    : dbproducthistory.cpp
// AppVersion  : 1.0
// FileVersion : 1.0
// Author      : Bikfalvi Tamas
//
//====================================================================================
//
//====================================================================================

#include "belenus.h"
#include "dbproducthistory.h"

cDBProductHistory::cDBProductHistory()
{
    init();
}

cDBProductHistory::~cDBProductHistory()
{
}

void cDBProductHistory::init( const unsigned int p_uiId,
                              const unsigned int p_uiLicenceId,
                              const unsigned int p_uiProductId,
                              const unsigned int p_uiLedgerId,
                              const unsigned int p_uiProductActionTypeId,
                              const unsigned int p_uiUserId,
                              const int p_inItemCount,
                              const int p_inNetPrice,
                              const int p_inVatPercent,
                              const QString &p_qsActionTime,
                              const QString &p_qsModified,
                              const bool p_bActive,
                              const QString &p_qsArchive ) throw()
{
    m_uiId              = p_uiId;
    m_uiLicenceId       = p_uiLicenceId;
    m_uiProductId       = p_uiProductId;
    m_uiLedgerId        = p_uiLedgerId;
    m_uiPATypeId        = p_uiProductActionTypeId;
    m_uiUserId          = p_uiUserId;
    m_inItemCount       = p_inItemCount;
    m_inNetPrice        = p_inNetPrice;
    m_inVatPercent      = p_inVatPercent;
    m_qsActionTime      = p_qsActionTime;
    m_qsModified        = p_qsModified;
    m_bActive           = p_bActive;
    m_qsArchive         = p_qsArchive;
}

void cDBProductHistory::init( const QSqlRecord &p_obRecord ) throw()
{
    int inIdIdx             = p_obRecord.indexOf( "productHistoryId" );
    int inLicenceIdIdx      = p_obRecord.indexOf( "licenceId" );
    int inProductIdIdx      = p_obRecord.indexOf( "productId" );
    int inLedgerIdIdx       = p_obRecord.indexOf( "ledgerId" );
    int inPATypeIdIdx       = p_obRecord.indexOf( "productActionTypeId" );
    int inUserIdIdx         = p_obRecord.indexOf( "userId" );
    int inItemCountIdx      = p_obRecord.indexOf( "productItemCount" );
    int inNetPriceIdx       = p_obRecord.indexOf( "netPrice" );
    int inVatPercentIdx     = p_obRecord.indexOf( "vatpercent" );
    int inActionTimeIdx     = p_obRecord.indexOf( "actionDateTime" );
    int inModifiedIdx       = p_obRecord.indexOf( "modified" );
    int inActiveIdx         = p_obRecord.indexOf( "active" );
    int inArchiveIdx        = p_obRecord.indexOf( "archive" );

    init( p_obRecord.value( inIdIdx ).toUInt(),
          p_obRecord.value( inLicenceIdIdx ).toUInt(),
          p_obRecord.value( inProductIdIdx ).toUInt(),
          p_obRecord.value( inLedgerIdIdx ).toUInt(),
          p_obRecord.value( inPATypeIdIdx ).toUInt(),
          p_obRecord.value( inUserIdIdx ).toUInt(),
          p_obRecord.value( inItemCountIdx ).toInt(),
          p_obRecord.value( inNetPriceIdx ).toInt(),
          p_obRecord.value( inVatPercentIdx ).toInt(),
          p_obRecord.value( inActionTimeIdx ).toString(),
          p_obRecord.value( inModifiedIdx ).toString(),
          p_obRecord.value( inActiveIdx ).toBool(),
          p_obRecord.value( inArchiveIdx ).toString() );
}

void cDBProductHistory::load( const unsigned int p_uiId ) throw( cSevException )
{
    cTracer obTrace( "cDBProductHistory::load", QString( "id: %1" ).arg( p_uiId ) );

    QSqlQuery *poQuery = g_poDB->executeQTQuery( QString( "SELECT * FROM productHistory WHERE productHistoryId = %1" ).arg( p_uiId ) );

    if( poQuery->size() != 1 )
        throw cSevException( cSeverity::ERROR, "Product history id not found" );

    poQuery->first();
    init( poQuery->record() );
}

void cDBProductHistory::save() throw( cSevException )
{
    cTracer obTrace( "cDBProductHistory::save" );
    QString  qsQuery;

    if( m_uiId )
    {
        qsQuery = "UPDATE";
        m_qsArchive = "MOD";
    }
    else
    {
        qsQuery = "INSERT INTO";
        m_qsArchive = "NEW";
    }
    qsQuery += " productHistory SET ";
    qsQuery += QString( "licenceId = \"%1\", " ).arg( m_uiLicenceId );
    qsQuery += QString( "productId = \"%1\", " ).arg( m_uiProductId );
    qsQuery += QString( "ledgerId = \"%1\", " ).arg( m_uiLedgerId );
    qsQuery += QString( "productActionTypeId = \"%1\", " ).arg( m_uiPATypeId );
    qsQuery += QString( "userId = \"%1\", " ).arg( m_uiUserId );
    qsQuery += QString( "productItemCount = \"%1\", " ).arg( m_inItemCount );
    qsQuery += QString( "netPrice = \"%1\", " ).arg( m_inNetPrice );
    qsQuery += QString( "vatpercent = \"%1\", " ).arg( m_inVatPercent );
    qsQuery += QString( "modified = \"%1\", " ).arg( QDateTime::currentDateTime().toString( QString("yyyy-MM-dd hh:mm:ss") ) );
    qsQuery += QString( "active = %1, " ).arg( m_bActive );
    qsQuery += QString( "archive = \"%1\" " ).arg( m_qsArchive );
    if( m_uiId )
    {
        qsQuery += QString( " WHERE productHistoryId = %1" ).arg( m_uiId );
    }

    QSqlQuery  *poQuery = g_poDB->executeQTQuery( qsQuery );
    if( !m_uiId && poQuery ) m_uiId = poQuery->lastInsertId().toUInt();
    if( poQuery ) delete poQuery;
/*
    if( m_uiId > 0 && m_uiLicenceId != 1 )
        g_obDBMirror.updateSynchronizationLevel( DB_PRODUCT_HISTORY );
    if( m_uiId > 0 && m_uiLicenceId == 0 )
        g_obDBMirror.updateGlobalSyncLevel( DB_PRODUCT_HISTORY );
*/
}

void cDBProductHistory::remove() throw( cSevException )
{
    cTracer obTrace( "cDBProductHistory::remove" );

    if( m_uiId )
    {
        QString  qsQuery;

        if( m_qsArchive == "NEW" )
        {
            qsQuery = "DELETE FROM productHistory ";
        }
        else
        {
            qsQuery = "UPDATE productHistory SET active=0, archive=\"MOD\" ";
        }
        qsQuery += QString( " WHERE productHistoryId = %1" ).arg( m_uiId );

        QSqlQuery  *poQuery = g_poDB->executeQTQuery( qsQuery );
        if( poQuery ) delete poQuery;
    }
}

void cDBProductHistory::createNew() throw()
{
    init();
}

unsigned int cDBProductHistory::id() const throw()
{
    return m_uiId;
}

unsigned int cDBProductHistory::licenceId() const throw()
{
    return m_uiLicenceId;
}

void cDBProductHistory::setLicenceId( const unsigned int p_uiLicenceId ) throw()
{
    m_uiLicenceId = p_uiLicenceId;
}

unsigned int cDBProductHistory::productId() const throw()
{
    return m_uiProductId;
}

void cDBProductHistory::setProductId( const unsigned int p_uiProductId ) throw()
{
    m_uiProductId = p_uiProductId;
}

unsigned int cDBProductHistory::ledgerId() const throw()
{
    return m_uiLedgerId;
}

void cDBProductHistory::setLedgerId( const unsigned int p_uiLedgerId ) throw()
{
    m_uiLedgerId = p_uiLedgerId;
}

unsigned int cDBProductHistory::productActionTypeId() const throw()
{
    return m_uiPATypeId;
}

void cDBProductHistory::setPATypeId( const unsigned int p_uiPATypeId ) throw()
{
    m_uiPATypeId = p_uiPATypeId;
}

unsigned int cDBProductHistory::userId() const throw()
{
    return m_uiUserId;
}

void cDBProductHistory::setUserId( const unsigned int p_uiUserId ) throw()
{
    m_uiUserId = p_uiUserId;
}

int cDBProductHistory::itemCount() const throw()
{
    return m_inItemCount;
}

void cDBProductHistory::setItemCount( const int p_inItemCount ) throw()
{
    m_inItemCount = p_inItemCount;
}

int cDBProductHistory::netPrice() const throw()
{
    return m_inNetPrice;
}

void cDBProductHistory::setNetPrice( const int p_inNetPrice ) throw()
{
    m_inNetPrice = p_inNetPrice;
}

int cDBProductHistory::vatPercent() const throw()
{
    return m_inVatPercent;
}

void cDBProductHistory::setVatPercent( const int p_inVatPercent ) throw()
{
    m_inVatPercent = p_inVatPercent;
}

QString cDBProductHistory::actionTime() const throw()
{
    return m_qsActionTime;
}

void cDBProductHistory::setActionTime( const QString &p_qsActionTime ) throw()
{
    m_qsActionTime = p_qsActionTime;
}

QString cDBProductHistory::modified() const throw()
{
    return m_qsModified;
}

bool cDBProductHistory::active() const throw()
{
    return m_bActive;
}

void cDBProductHistory::setActive( const bool p_bActive ) throw()
{
    m_bActive = p_bActive;
}

QString cDBProductHistory::archive() const throw()
{
    return m_qsArchive;
}

void cDBProductHistory::setArchive( const QString &p_qsArchive ) throw()
{
    m_qsArchive = p_qsArchive;
}

