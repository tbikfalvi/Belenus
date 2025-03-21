//====================================================================================
//
// Belenus Server alkalmazas (c) Pagony Multimedia Studio Bt - 2010
//
//====================================================================================
//
// Filename    : dbcassahistory.cpp
// AppVersion  : 1.0
// FileVersion : 1.0
// Author      : Bikfalvi Tamas
//
//====================================================================================
//
//====================================================================================

#include "belenus.h"
#include "dbcassahistory.h"
#include "dbledger.h"

cDBCassaHistory::cDBCassaHistory()
{
    init();
}

cDBCassaHistory::~cDBCassaHistory()
{
}

void cDBCassaHistory::init( const unsigned int p_uiId,
                             const unsigned int p_uiLicenceId,
                             const unsigned int p_uiParentId,
                             const unsigned int p_uiCassaId,
                             const unsigned int p_uiLedgerId,
                             const unsigned int p_uiUserId,
                             const unsigned int p_uiPatientId,
                             const int p_inActionValue,
                            const int p_inActionCard,
                            const int p_inActionCash,
                             const int p_inActionBalance,
                             const QString &p_qsActionTime,
                             const QString &p_qsComment,
                             const QString &p_qsModified,
                             const bool p_bActive,
                             const QString &p_qsArchive ) throw()
{
    m_uiId              = p_uiId;
    m_uiLicenceId       = p_uiLicenceId;
    m_uiParentId        = p_uiParentId;
    m_uiCassaId         = p_uiCassaId;
    m_uiLedgerId        = p_uiLedgerId;
    m_uiUserId          = p_uiUserId;
    m_uiPatientId       = p_uiPatientId;
    m_inActionValue     = p_inActionValue;
    m_inActionCard      = p_inActionCard;
    m_inActionCash      = p_inActionCash;
    m_inActionBalance   = p_inActionBalance;
    m_qsActionTime      = p_qsActionTime;
    m_qsComment         = p_qsComment;
    m_qsModified        = p_qsModified;
    m_bActive           = p_bActive;
    m_qsArchive         = p_qsArchive;
}

void cDBCassaHistory::init( const QSqlRecord &p_obRecord ) throw()
{
    int inIdIdx             = p_obRecord.indexOf( "cassaHistoryId" );
    int inLicenceIdIdx      = p_obRecord.indexOf( "licenceId" );
    int inParentIdIdx       = p_obRecord.indexOf( "parentId" );
    int inCassaIdIdx        = p_obRecord.indexOf( "cassaId" );
    int inLedgerIdIdx       = p_obRecord.indexOf( "ledgerId" );
    int inUserIdIdx         = p_obRecord.indexOf( "userId" );
    int inPatientIdIdx      = p_obRecord.indexOf( "patientId" );
    int inActionValueIdx    = p_obRecord.indexOf( "actionValue" );
    int inActionCardIdx     = p_obRecord.indexOf( "actionCard" );
    int inActionCashIdx     = p_obRecord.indexOf( "actionCash" );
    int inActionBalanceIdx  = p_obRecord.indexOf( "actionBalance" );
    int inActionTimeIdx     = p_obRecord.indexOf( "actionTime" );
    int inCommentIdx        = p_obRecord.indexOf( "comment" );
    int inModifiedIdx       = p_obRecord.indexOf( "modified" );
    int inActiveIdx         = p_obRecord.indexOf( "active" );
    int inArchiveIdx        = p_obRecord.indexOf( "archive" );

    init( p_obRecord.value( inIdIdx ).toUInt(),
          p_obRecord.value( inLicenceIdIdx ).toUInt(),
          p_obRecord.value( inParentIdIdx ).toUInt(),
          p_obRecord.value( inCassaIdIdx ).toUInt(),
          p_obRecord.value( inLedgerIdIdx ).toUInt(),
          p_obRecord.value( inUserIdIdx ).toUInt(),
          p_obRecord.value( inPatientIdIdx ).toUInt(),
          p_obRecord.value( inActionValueIdx ).toInt(),
          p_obRecord.value( inActionCardIdx ).toInt(),
          p_obRecord.value( inActionCashIdx ).toInt(),
          p_obRecord.value( inActionBalanceIdx ).toInt(),
          p_obRecord.value( inActionTimeIdx ).toString(),
          p_obRecord.value( inCommentIdx ).toString(),
          p_obRecord.value( inModifiedIdx ).toString(),
          p_obRecord.value( inActiveIdx ).toBool(),
          p_obRecord.value( inArchiveIdx ).toString() );
}

void cDBCassaHistory::load( const unsigned int p_uiId ) throw( cSevException )
{
    cTracer obTrace( "cDBCassaHistory::load", QString( "id: %1" ).arg( p_uiId ) );

    QSqlQuery *poQuery = g_poDB->executeQTQuery( QString( "SELECT * FROM cassaHistory WHERE cassaHistoryId = %1" ).arg( p_uiId ) );

    if( poQuery->size() != 1 )
        throw cSevException( cSeverity::ERROR, "Cassa history id not found" );

    poQuery->first();
    init( poQuery->record() );
}

void cDBCassaHistory::loadByLedger( const unsigned int p_uiId ) throw( cSevException )
{
    cTracer obTrace( "cDBCassaHistory::loadByLedger", QString( "ledgerId: %1" ).arg( p_uiId ) );

    QSqlQuery *poQuery = g_poDB->executeQTQuery( QString( "SELECT * FROM cassaHistory WHERE ledgerId = %1" ).arg( p_uiId ) );

    if( poQuery->size() != 1 )
        throw cSevException( cSeverity::ERROR, "Cassa history id not found" );

    poQuery->first();
    init( poQuery->record() );
}

void cDBCassaHistory::save() throw( cSevException )
{
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
    qsQuery += " cassaHistory SET ";
    qsQuery += QString( "licenceId = \"%1\", " ).arg( m_uiLicenceId );
    qsQuery += QString( "parentId = \"%1\", " ).arg( m_uiParentId );
    qsQuery += QString( "cassaId = \"%1\", " ).arg( m_uiCassaId );
    qsQuery += QString( "ledgerId = \"%1\", " ).arg( m_uiLedgerId );
    qsQuery += QString( "userId = \"%1\", " ).arg( m_uiUserId );
    qsQuery += QString( "patientId = \"%1\", " ).arg( m_uiPatientId );
    qsQuery += QString( "actionValue = \"%1\", " ).arg( m_inActionValue );
    qsQuery += QString( "actionCard = \"%1\", " ).arg( m_inActionCard );
    qsQuery += QString( "actionCash = \"%1\", " ).arg( m_inActionCash );
    qsQuery += QString( "actionBalance = \"%1\", " ).arg( m_inActionBalance );
    qsQuery += QString( "comment = \"%1\", " ).arg( m_qsComment.replace( QString("\""), QString("\\\"") ) );
    qsQuery += QString( "modified = \"%1\", " ).arg( QDateTime::currentDateTime().toString( QString("yyyy-MM-dd hh:mm:ss") ) );
    qsQuery += QString( "active = %1, " ).arg( m_bActive );
    qsQuery += QString( "archive = \"%1\" " ).arg( m_qsArchive );
    if( m_uiId )
    {
        qsQuery += QString( " WHERE cassaHistoryId = %1" ).arg( m_uiId );
    }

    QSqlQuery  *poQuery = g_poDB->executeQTQuery( qsQuery );
    if( !m_uiId && poQuery ) m_uiId = poQuery->lastInsertId().toUInt();
    if( poQuery ) delete poQuery;
/*
    if( m_uiId > 0 && m_uiLicenceId != 1 )
        g_obDBMirror.updateSynchronizationLevel( DB_CASSA_HISTORY );
    if( m_uiId > 0 && m_uiLicenceId == 0 )
        g_obDBMirror.updateGlobalSyncLevel( DB_CASSA_HISTORY );
*/
}

void cDBCassaHistory::remove() throw( cSevException )
{
    cTracer obTrace( "cDBCassaHistory::remove" );

    if( m_uiId )
    {
        QString  qsQuery;

        if( m_qsArchive == "NEW" )
        {
            qsQuery = "DELETE FROM cassaHistory ";
        }
        else
        {
            qsQuery = "UPDATE cassaHistory SET active=0, archive=\"MOD\" ";
        }
        qsQuery += QString( " WHERE cassaHistoryId = %1" ).arg( m_uiId );

        QSqlQuery  *poQuery = g_poDB->executeQTQuery( qsQuery );
        if( poQuery ) delete poQuery;
    }
}

void cDBCassaHistory::revoke(const unsigned int p_uiLedgerId) throw( cSevException )
{
    QString qsComment = QString( QObject::tr("Revoking cassa action: %1").arg(comment()) );

    g_obCassa.cassaAddMoneyAction( actionCash()*(-1), actionCard()*(-1), p_uiLedgerId, qsComment, m_uiId );
}

void cDBCassaHistory::createNew() throw()
{
    init();
}

bool cDBCassaHistory::isRevokeEnabled( const unsigned int p_uiId ) const throw()
{
    QSqlQuery *poQuery = g_poDB->executeQTQuery( QString( "SELECT * FROM cassaHistory WHERE cassaHistoryId = %1" ).arg( p_uiId ) );

    if( poQuery->size() != 1 )
        throw cSevException( cSeverity::ERROR, "Cassa history id not found" );

    // Ha sztorno tetel
    poQuery->first();
    if( poQuery->value(2).toUInt() > 0 )
    {
        return false;
    }

    poQuery = g_poDB->executeQTQuery( QString( "SELECT * FROM cassaHistory WHERE parentId = %1" ).arg( p_uiId ) );

    // Ha sztornozott tetel
    if( poQuery->size() > 0 )
    {
        return false;
    }

    poQuery = g_poDB->executeQTQuery( QString( "SELECT * FROM ledger, cassaHistory WHERE cassaHistoryId = %1 AND ledger.ledgerId=cassaHistory.ledgerId" ).arg( p_uiId ) );
    poQuery->first();

    // Ha gephasznalat, vagy kassza nyitas, zaras
    if( poQuery->size() > 0 && poQuery->value(3).toUInt() < 2 )
    {
        return false;
    }

    // Ha az eladott berlet hasznalatban van
    if( poQuery->value(9).toUInt() > 1 )
    {
        unsigned int uiPCId = poQuery->value(9).toUInt();

        poQuery = g_poDB->executeQTQuery( QString( "SELECT * FROM patientcardunits WHERE patientCardId = %1 AND prepared = 1 " ).arg( uiPCId ) );

        if( poQuery->size() > 0 )
        {
            return false;
        }
    }

    return true;
}

unsigned int cDBCassaHistory::id() const throw()
{
    return m_uiId;
}

unsigned int cDBCassaHistory::licenceId() const throw()
{
    return m_uiLicenceId;
}

void cDBCassaHistory::setLicenceId( const unsigned int p_uiLicenceId ) throw()
{
    m_uiLicenceId = p_uiLicenceId;
}

unsigned int cDBCassaHistory::parentId() const throw()
{
    return m_uiParentId;
}

void cDBCassaHistory::setParentId( const unsigned int p_uiParentId ) throw()
{
    m_uiParentId = p_uiParentId;
}

unsigned int cDBCassaHistory::cassaId() const throw()
{
    return m_uiCassaId;
}

void cDBCassaHistory::setCassaId( const unsigned int p_uiCassaId ) throw()
{
    m_uiCassaId = p_uiCassaId;
}

unsigned int cDBCassaHistory::ledgerId() const throw()
{
    return m_uiLedgerId;
}

void cDBCassaHistory::setLedgerId( const unsigned int p_uiLedgerId ) throw()
{
    m_uiLedgerId = p_uiLedgerId;
}

unsigned int cDBCassaHistory::userId() const throw()
{
    return m_uiUserId;
}

void cDBCassaHistory::setUserId( const unsigned int p_uiUserId ) throw()
{
    m_uiUserId = p_uiUserId;
}

unsigned int cDBCassaHistory::patientId() const throw()
{
    return m_uiPatientId;
}

void cDBCassaHistory::setPatientId( const unsigned int p_uiPatientId ) throw()
{
    m_uiPatientId = p_uiPatientId;
}

int cDBCassaHistory::actionValue() const throw()
{
    return m_inActionValue;
}

void cDBCassaHistory::setActionValue( const int p_inActionValue ) throw()
{
    m_inActionValue = p_inActionValue;
}

int cDBCassaHistory::actionCard() const throw()
{
    return m_inActionCard;
}

void cDBCassaHistory::setActionCard( const int p_inActionCard ) throw()
{
    m_inActionCard = p_inActionCard;
}

int cDBCassaHistory::actionCash() const throw()
{
    return m_inActionCash;
}

void cDBCassaHistory::setActionCash( const int p_inActionCash ) throw()
{
    m_inActionCash = p_inActionCash;
}

int cDBCassaHistory::actionBalance() const throw()
{
    return m_inActionBalance;
}

void cDBCassaHistory::setActionBalance( const int p_inActionBalance ) throw()
{
    m_inActionBalance = p_inActionBalance;
}

QString cDBCassaHistory::actionTime() const throw()
{
    return m_qsActionTime;
}

void cDBCassaHistory::setActionTime( const QString &p_qsActionTime ) throw()
{
    m_qsActionTime = p_qsActionTime;
}

QString cDBCassaHistory::comment() const throw()
{
    return m_qsComment;
}

void cDBCassaHistory::setComment( const QString &p_qsComment ) throw()
{
    m_qsComment = p_qsComment;
}

QString cDBCassaHistory::modified() const throw()
{
    return m_qsModified;
}

bool cDBCassaHistory::active() const throw()
{
    return m_bActive;
}

void cDBCassaHistory::setActive( const bool p_bActive ) throw()
{
    m_bActive = p_bActive;
}

QString cDBCassaHistory::archive() const throw()
{
    return m_qsArchive;
}

void cDBCassaHistory::setArchive( const QString &p_qsArchive ) throw()
{
    m_qsArchive = p_qsArchive;
}

