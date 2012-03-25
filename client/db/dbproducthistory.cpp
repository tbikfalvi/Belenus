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
                             const unsigned int p_uiProductActionTypeId,
                             const unsigned int p_uiUserId,
                             const int p_inProductItemCount,
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
    m_uiPATypeId        = p_uiProductActionTypeId;
    m_uiUserId          = p_uiUserId;
    m_inItemCount       = p_inProductItemCount;
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
    int inProductIdIdx        = p_obRecord.indexOf( "productId" );
    int inUserIdIdx         = p_obRecord.indexOf( "userId" );
    int inPatientIdIdx      = p_obRecord.indexOf( "patientId" );
    int inActionValueIdx    = p_obRecord.indexOf( "actionValue" );
    int inActionBalanceIdx  = p_obRecord.indexOf( "actionBalance" );
    int inActionTimeIdx     = p_obRecord.indexOf( "actionTime" );
    int inCommentIdx        = p_obRecord.indexOf( "comment" );
    int inModifiedIdx       = p_obRecord.indexOf( "modified" );
    int inActiveIdx         = p_obRecord.indexOf( "active" );
    int inArchiveIdx        = p_obRecord.indexOf( "archive" );

    init( p_obRecord.value( inIdIdx ).toUInt(),
          p_obRecord.value( inLicenceIdIdx ).toUInt(),
          p_obRecord.value( inProductIdIdx ).toUInt(),
          p_obRecord.value( inUserIdIdx ).toUInt(),
          p_obRecord.value( inPatientIdIdx ).toUInt(),
          p_obRecord.value( inActionValueIdx ).toInt(),
          p_obRecord.value( inActionBalanceIdx ).toInt(),
          p_obRecord.value( inActionTimeIdx ).toString(),
          p_obRecord.value( inCommentIdx ).toString(),
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

        if( m_qsArchive != "NEW" )
        {
            m_qsArchive = "MOD";
        }
    }
    else
    {
        qsQuery = "INSERT INTO";
        m_qsArchive = "NEW";
    }
    qsQuery += " productHistory SET ";
    qsQuery += QString( "licenceId = \"%1\", " ).arg( m_uiLicenceId );
    qsQuery += QString( "productId = \"%1\", " ).arg( m_uiProductId );
    qsQuery += QString( "userId = \"%1\", " ).arg( m_uiUserId );
    qsQuery += QString( "patientId = \"%1\", " ).arg( m_uiPatientId );
    qsQuery += QString( "actionValue = \"%1\", " ).arg( m_inActionValue );
    qsQuery += QString( "actionBalance = \"%1\", " ).arg( m_inActionBalance );
    qsQuery += QString( "comment = \"%1\", " ).arg( m_qsComment );
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

unsigned int cDBProductHistory::userId() const throw()
{
    return m_uiUserId;
}

void cDBProductHistory::setUserId( const unsigned int p_uiUserId ) throw()
{
    m_uiUserId = p_uiUserId;
}

unsigned int cDBProductHistory::patientId() const throw()
{
    return m_uiPatientId;
}

void cDBProductHistory::setPatientId( const unsigned int p_uiPatientId ) throw()
{
    m_uiPatientId = p_uiPatientId;
}

int cDBProductHistory::actionValue() const throw()
{
    return m_inActionValue;
}

void cDBProductHistory::setActionValue( const int p_inActionValue ) throw()
{
    m_inActionValue = p_inActionValue;
}

int cDBProductHistory::actionBalance() const throw()
{
    return m_inActionBalance;
}

void cDBProductHistory::setActionBalance( const int p_inActionBalance ) throw()
{
    m_inActionBalance = p_inActionBalance;
}

QString cDBProductHistory::actionTime() const throw()
{
    return m_qsActionTime;
}

void cDBProductHistory::setActionTime( const QString &p_qsActionTime ) throw()
{
    m_qsActionTime = p_qsActionTime;
}

QString cDBProductHistory::comment() const throw()
{
    return m_qsComment;
}

void cDBProductHistory::setComment( const QString &p_qsComment ) throw()
{
    m_qsComment = p_qsComment;
    m_qsComment = m_qsComment.replace( QString("\""), QString("\\\"") );
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

