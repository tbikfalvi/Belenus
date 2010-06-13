//====================================================================================
//
// Belenus Server alkalmazas © Pagony Multimedia Studio Bt - 2010
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

cDBCassaHistory::cDBCassaHistory()
{
    init();
}

cDBCassaHistory::~cDBCassaHistory()
{
}

void cDBCassaHistory::init( const unsigned int p_uiId,
                             const unsigned int p_uiLicenceId,
                             const unsigned int p_uiCassaId,
                             const unsigned int p_uiUserId,
                             const int p_inActionValue,
                             const int p_inActionBalance,
                             const string &p_stActionTime,
                             const string &p_stComment,
                             const bool p_bActive,
                             const string &p_stArchive ) throw()
{
    m_uiId              = p_uiId;
    m_uiLicenceId       = p_uiLicenceId;
    m_uiCassaId         = p_uiCassaId;
    m_uiUserId          = p_uiUserId;
    m_inActionValue     = p_inActionValue;
    m_inActionBalance   = p_inActionBalance;
    m_stActionTime      = p_stActionTime;
    m_stComment         = p_stComment;
    m_bActive           = p_bActive;
    m_stArchive         = p_stArchive;
}

void cDBCassaHistory::init( const QSqlRecord &p_obRecord ) throw()
{
    int inIdIdx             = p_obRecord.indexOf( "cassaHistoryId" );
    int inLicenceIdIdx      = p_obRecord.indexOf( "licenceId" );
    int inCassaIdIdx        = p_obRecord.indexOf( "cassaId" );
    int inUserIdIdx         = p_obRecord.indexOf( "userId" );
    int inActionValueIdx    = p_obRecord.indexOf( "actionValue" );
    int inActionBalanceIdx  = p_obRecord.indexOf( "actionBalance" );
    int inActionTimeIdx     = p_obRecord.indexOf( "actionTime" );
    int inCommentIdx        = p_obRecord.indexOf( "comment" );
    int inActiveIdx         = p_obRecord.indexOf( "active" );
    int inArchiveIdx        = p_obRecord.indexOf( "archive" );

    init( p_obRecord.value( inIdIdx ).toUInt(),
          p_obRecord.value( inLicenceIdIdx ).toUInt(),
          p_obRecord.value( inCassaIdIdx ).toUInt(),
          p_obRecord.value( inUserIdIdx ).toUInt(),
          p_obRecord.value( inActionValueIdx ).toInt(),
          p_obRecord.value( inActionBalanceIdx ).toInt(),
          p_obRecord.value( inActionTimeIdx ).toString().toStdString(),
          p_obRecord.value( inCommentIdx ).toString().toStdString(),
          p_obRecord.value( inActiveIdx ).toBool(),
          p_obRecord.value( inArchiveIdx ).toString().toStdString() );
}

void cDBCassaHistory::load( const unsigned int p_uiId ) throw( cSevException )
{
    cTracer obTrace( "cDBCassaHistory::load", QString( "id: %1" ).arg( p_uiId ).toStdString() );

    QSqlQuery *poQuery = g_poDB->executeQTQuery( QString( "SELECT * FROM cassaHistory WHERE cassaHistoryId = %1" ).arg( p_uiId ) );

    if( poQuery->size() != 1 )
        throw cSevException( cSeverity::ERROR, "Cassa history id not found" );

    poQuery->first();
    init( poQuery->record() );
}

void cDBCassaHistory::save() throw( cSevException )
{
    cTracer obTrace( "cDBCassaHistory::save" );
    QString  qsQuery;

    if( m_uiId )
    {
        qsQuery = "UPDATE";

        if( m_stArchive.compare("NEW") != 0 )
        {
            m_stArchive = "MOD";
        }
    }
    else
    {
        qsQuery = "INSERT INTO";
        m_stArchive = "NEW";
    }
    qsQuery += " cassaHistory SET ";
    qsQuery += QString( "licenceId = \"%1\", " ).arg( m_uiLicenceId );
    qsQuery += QString( "cassaId = \"%1\", " ).arg( m_uiCassaId );
    qsQuery += QString( "userId = \"%1\", " ).arg( m_uiUserId );
    qsQuery += QString( "actionValue = \"%1\", " ).arg( m_inActionValue );
    qsQuery += QString( "actionBalance = \"%1\", " ).arg( m_inActionBalance );
    qsQuery += QString( "actionTime = \"%1\", " ).arg( m_stActionTime );
    qsQuery += QString( "comment = \"%1\", " ).arg( m_stComment );
    qsQuery += QString( "active = %1, " ).arg( m_bActive );
    qsQuery += QString( "archive = \"%1\" " ).arg( QString::fromStdString( m_stArchive ) );
    if( m_uiId )
    {
        qsQuery += QString( " WHERE cassaHistoryId = %1" ).arg( m_uiId );
    }

    QSqlQuery  *poQuery = g_poDB->executeQTQuery( qsQuery );
    if( !m_uiId && poQuery ) m_uiId = poQuery->lastInsertId().toUInt();
    if( poQuery ) delete poQuery;
}

void cDBCassaHistory::remove() throw( cSevException )
{
    cTracer obTrace( "cDBCassaHistory::remove" );

    if( m_uiId )
    {
        QString  qsQuery;

        if( m_stArchive.compare( "NEW" ) == 0 )
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

void cDBCassaHistory::createNew() throw()
{
    init();
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

unsigned int cDBCassaHistory::cassaId() const throw()
{
    return m_uiCassaId;
}

void cDBCassaHistory::setCassaId( const unsigned int p_uiCassaId ) throw()
{
    m_uiCassaId = p_uiCassaId;
}

unsigned int cDBCassaHistory::userId() const throw()
{
    return m_uiUserId;
}

void cDBCassaHistory::setUserId( const unsigned int p_uiUserId ) throw()
{
    m_uiUserId = p_uiUserId;
}

int cDBCassaHistory::actionValue() const throw()
{
    return m_inActionValue;
}

void cDBCassaHistory::setActionValue( const int p_inActionValue ) throw()
{
    m_inActionValue = p_inActionValue;
}

int cDBCassaHistory::actionBalance() const throw()
{
    return m_inActionBalance;
}

void cDBCassaHistory::setActionBalance( const int p_inActionBalance ) throw()
{
    m_inActionBalance = p_inActionBalance;
}

string cDBCassaHistory::actionTime() const throw()
{
    return m_stActionTime;
}

void cDBCassaHistory::setActionTime( const string &p_stActionTime ) throw()
{
    m_stActionTime = p_stActionTime;
}

string cDBCassaHistory::comment() const throw()
{
    return m_stComment;
}

void cDBCassaHistory::setComment( const string &p_stComment ) throw()
{
    m_stComment = p_stComment;
}

bool cDBCassaHistory::active() const throw()
{
    return m_bActive;
}

void cDBCassaHistory::setActive( const bool p_bActive ) throw()
{
    m_bActive = p_bActive;
}

string cDBCassaHistory::archive() const throw()
{
    return m_stArchive;
}

void cDBCassaHistory::setArchive( const string &p_stArchive ) throw()
{
    m_stArchive = p_stArchive;
}

