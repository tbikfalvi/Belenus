//====================================================================================
//
// Belenus Server alkalmazas (c) Pagony Multimedia Studio Bt - 2010
//
//====================================================================================
//
// Filename    : dbcassa.cpp
// AppVersion  : 1.0
// FileVersion : 1.0
// Author      : Bikfalvi Tamas
//
//====================================================================================
//
//====================================================================================

#include "belenus.h"
#include "dbcassa.h"

cDBCassa::cDBCassa()
{
    init();
}

cDBCassa::~cDBCassa()
{
}

void cDBCassa::init( const unsigned int p_uiId,
                             const unsigned int p_uiLicenceId,
                             const unsigned int p_uiUserId,
                             const int p_nCurrentBalance,
                             const QString p_qsStartDateTime,
                             const QString p_qsStopDateTime,
                             const QString &p_qsModified,
                             const bool p_bActive,
                             const QString &p_qsArchive ) throw()
{
    m_uiId              = p_uiId;
    m_uiLicenceId       = p_uiLicenceId;
    m_uiUserId          = p_uiUserId;
    m_nCurrentBalance   = p_nCurrentBalance;
    m_qsStartDateTime   = p_qsStartDateTime;
    m_qsStopDateTime    = p_qsStopDateTime;
    m_qsModified        = p_qsModified;
    m_bActive           = p_bActive;
    m_qsArchive         = p_qsArchive;
}

void cDBCassa::init( const QSqlRecord &p_obRecord ) throw()
{
    int inIdIdx             = p_obRecord.indexOf( "cassaId" );
    int inLicenceIdIdx      = p_obRecord.indexOf( "licenceId" );
    int inUserIdIdx         = p_obRecord.indexOf( "userId" );
    int inCurrentBalanceIdx = p_obRecord.indexOf( "currentBalance" );
    int inStartDateTimeIdx  = p_obRecord.indexOf( "startDateTime" );
    int inStopDateTimeIdx   = p_obRecord.indexOf( "stopDateTime" );
    int inModifiedIdx       = p_obRecord.indexOf( "modified" );
    int inActiveIdx         = p_obRecord.indexOf( "active" );
    int inArchiveIdx        = p_obRecord.indexOf( "archive" );

    init( p_obRecord.value( inIdIdx ).toUInt(),
          p_obRecord.value( inLicenceIdIdx ).toUInt(),
          p_obRecord.value( inUserIdIdx ).toUInt(),
          p_obRecord.value( inCurrentBalanceIdx ).toInt(),
          p_obRecord.value( inStartDateTimeIdx ).toString(),
          p_obRecord.value( inStopDateTimeIdx ).toString(),
          p_obRecord.value( inModifiedIdx ).toString(),
          p_obRecord.value( inActiveIdx ).toBool(),
          p_obRecord.value( inArchiveIdx ).toString() );
}

void cDBCassa::load( const unsigned int p_uiId ) throw( cSevException )
{
    cTracer obTrace( "cDBCassa::load", QString( "id: %1" ).arg( p_uiId ) );

    QSqlQuery *poQuery = g_poDB->executeQTQuery( QString( "SELECT * FROM cassa WHERE cassaId = %1" ).arg( p_uiId ) );

    if( poQuery->size() != 1 )
        throw cSevException( cSeverity::ERROR, "Cassa id not found" );

    poQuery->first();
    init( poQuery->record() );
}

void cDBCassa::load() throw( cSevException )
{
    cTracer obTrace( "cDBCassa::load" );

    QSqlQuery *poQuery = g_poDB->executeQTQuery( QString( "SELECT * FROM cassa ORDER BY cassaId DESC" ) );

    if( poQuery->size() < 1 )
        throw cSevException( cSeverity::ERROR, "Cassa table empty" );

    poQuery->first();
    init( poQuery->record() );
}

void cDBCassa::save() throw( cSevException )
{
    cTracer obTrace( "cDBCassa::save" );
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
    qsQuery += " cassa SET ";
    qsQuery += QString( "licenceId = \"%1\", " ).arg( m_uiLicenceId );
    qsQuery += QString( "userId = \"%1\", " ).arg( m_uiUserId );
    qsQuery += QString( "currentBalance = \"%1\", " ).arg( m_nCurrentBalance );
    qsQuery += QString( "startDateTime = \"%1\", " ).arg( m_qsStartDateTime );
    qsQuery += QString( "stopDateTime = \"%1\", " ).arg( m_qsStopDateTime );
    qsQuery += QString( "modified = \"%1\", " ).arg( QDateTime::currentDateTime().toString( QString("yyyy-MM-dd hh:mm:ss") ) );
    qsQuery += QString( "active = %1, " ).arg( m_bActive );
    qsQuery += QString( "archive = \"%1\" " ).arg( m_qsArchive );
    if( m_uiId )
    {
        qsQuery += QString( " WHERE cassaId = %1" ).arg( m_uiId );
    }

    QSqlQuery  *poQuery = g_poDB->executeQTQuery( qsQuery );
    if( !m_uiId && poQuery ) m_uiId = poQuery->lastInsertId().toUInt();
    if( poQuery ) delete poQuery;
/*
    if( m_uiId > 0 && m_uiLicenceId != 1 )
        g_obDBMirror.updateSynchronizationLevel( DB_CASSA );
    if( m_uiId > 0 && m_uiLicenceId == 0 )
        g_obDBMirror.updateGlobalSyncLevel( DB_CASSA );
*/
}

void cDBCassa::remove() throw( cSevException )
{
    cTracer obTrace( "cDBCassa::remove" );

    if( m_uiId )
    {
        QString  qsQuery;

        if( m_qsArchive == "NEW" )
        {
            qsQuery = "DELETE FROM cassa ";
        }
        else
        {
            qsQuery = "UPDATE cassa SET active=0, archive=\"MOD\" ";
        }
        qsQuery += QString( " WHERE cassaId = %1" ).arg( m_uiId );

        QSqlQuery  *poQuery = g_poDB->executeQTQuery( qsQuery );
        if( poQuery ) delete poQuery;
    }
}

void cDBCassa::createNew() throw()
{
    init();
}

unsigned int cDBCassa::id() const throw()
{
    return m_uiId;
}

unsigned int cDBCassa::licenceId() const throw()
{
    return m_uiLicenceId;
}

void cDBCassa::setLicenceId( const unsigned int p_uiLicenceId ) throw()
{
    m_uiLicenceId = p_uiLicenceId;
}

unsigned int cDBCassa::userId() const throw()
{
    return m_uiUserId;
}

void cDBCassa::setUserId( const unsigned int p_nUserId ) throw()
{
    m_uiUserId = p_nUserId;
}

int cDBCassa::currentBalance() const throw()
{
    return m_nCurrentBalance;
}

void cDBCassa::setCurrentBalance( const int p_nCurrentBalance ) throw()
{
    m_nCurrentBalance = p_nCurrentBalance;
}

QString cDBCassa::startDateTime() const throw()
{
    return m_qsStartDateTime;
}

void cDBCassa::setStartDateTime( const QString &p_qsStartDateTime ) throw()
{
    m_qsStartDateTime = p_qsStartDateTime;
}

QString cDBCassa::stopDateTime() const throw()
{
    return m_qsStopDateTime;
}

void cDBCassa::setStopDateTime( const QString &p_qsStopDateTime ) throw()
{
    m_qsStopDateTime = p_qsStopDateTime;
}

bool cDBCassa::active() const throw()
{
    return m_bActive;
}

QString cDBCassa::modified() const throw()
{
    return m_qsModified;
}

void cDBCassa::setActive( const bool p_bActive ) throw()
{
    m_bActive = p_bActive;
}

QString cDBCassa::archive() const throw()
{
    return m_qsArchive;
}

void cDBCassa::setArchive( const QString &p_qsArchive ) throw()
{
    m_qsArchive = p_qsArchive;
}

