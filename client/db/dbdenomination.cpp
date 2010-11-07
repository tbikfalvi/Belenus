//====================================================================================
//
// Belenus Server alkalmazas (c) Pagony Multimedia Studio Bt - 2010
//
//====================================================================================
//
// Filename    : dbdenomination.cpp
// AppVersion  : 1.0
// FileVersion : 1.0
// Author      : Bikfalvi Tamas
//
//====================================================================================
//
//====================================================================================

#include "belenus.h"
#include "dbdenomination.h"

cDBDenomination::cDBDenomination()
{
    init();
}

cDBDenomination::~cDBDenomination()
{
}

void cDBDenomination::init( const unsigned int p_uiId,
                            const unsigned int p_uiLicenceId,
                            const unsigned int p_uiDenomination,
                            const bool p_bActive,
                            const string &p_stArchive ) throw()
{
    m_uiId              = p_uiId;
    m_uiLicenceId       = p_uiLicenceId;
    m_uiDenomination    = p_uiDenomination;
    m_bActive           = p_bActive;
    m_stArchive         = p_stArchive;
}

void cDBDenomination::init( const QSqlRecord &p_obRecord ) throw()
{
    int inIdIdx             = p_obRecord.indexOf( "denominationId" );
    int inLicenceIdIdx      = p_obRecord.indexOf( "licenceId" );
    int inDenominationIdx   = p_obRecord.indexOf( "denomination" );
    int inActiveIdx         = p_obRecord.indexOf( "active" );
    int inArchiveIdx        = p_obRecord.indexOf( "archive" );

    init( p_obRecord.value( inIdIdx ).toInt(),
          p_obRecord.value( inLicenceIdIdx ).toInt(),
          p_obRecord.value( inDenominationIdx ).toInt(),
          p_obRecord.value( inActiveIdx ).toBool(),
          p_obRecord.value( inArchiveIdx ).toString().toStdString() );
}

void cDBDenomination::load( const unsigned int p_uiId ) throw( cSevException )
{
    cTracer obTrace( "cDBDenomination::load", QString( "id: %1" ).arg( p_uiId ) );

    QSqlQuery *poQuery = g_poDB->executeQTQuery( QString( "SELECT * FROM denomination WHERE denominationId = %1" ).arg( p_uiId ) );

    if( poQuery->size() != 1 )
        throw cSevException( cSeverity::ERROR, "Denomination id not found" );

    poQuery->first();
    init( poQuery->record() );
}

void cDBDenomination::save() throw( cSevException )
{
    cTracer obTrace( "cDBDenomination::save" );
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
    qsQuery += " denomination SET ";
    qsQuery += QString( "licenceId = \"%1\", " ).arg( m_uiLicenceId );
    qsQuery += QString( "denomination = \"%1\", " ).arg( m_uiDenomination );
    qsQuery += QString( "active = %1, " ).arg( m_bActive );
    qsQuery += QString( "archive = \"%1\" " ).arg( QString::fromStdString( m_stArchive ) );
    if( m_uiId )
    {
        qsQuery += QString( " WHERE denominationId = %1" ).arg( m_uiId );
    }

    QSqlQuery  *poQuery = g_poDB->executeQTQuery( qsQuery );
    if( !m_uiId && poQuery ) m_uiId = poQuery->lastInsertId().toUInt();
    if( poQuery ) delete poQuery;

    if( m_uiId > 0 && m_uiLicenceId != 1 )
        g_obDBMirror.updateSynchronizationLevel( DB_DENOMINATION );
    if( m_uiId > 0 && m_uiLicenceId == 0 )
        g_obDBMirror.updateGlobalSyncLevel( DB_DENOMINATION );
}

void cDBDenomination::remove() throw( cSevException )
{
    cTracer obTrace( "cDBDenomination::remove" );

    if( m_uiId )
    {
        QString  qsQuery;

        if( m_stArchive.compare( "NEW" ) == 0 )
        {
            qsQuery = "DELETE FROM denomination ";
        }
        else
        {
            qsQuery = "UPDATE denomination SET active=0, archive=\"MOD\" ";
        }
        qsQuery += QString( " WHERE denominationId = %1" ).arg( m_uiId );

        QSqlQuery  *poQuery = g_poDB->executeQTQuery( qsQuery );
        if( poQuery ) delete poQuery;
    }
}

void cDBDenomination::createNew() throw()
{
    init();
}

unsigned int cDBDenomination::id() const throw()
{
    return m_uiId;
}

unsigned int cDBDenomination::licenceId() const throw()
{
    return m_uiLicenceId;
}

void cDBDenomination::setLicenceId( const unsigned int p_uiLicenceId ) throw()
{
    m_uiLicenceId = p_uiLicenceId;
}

unsigned int cDBDenomination::denomination() const throw()
{
    return m_uiDenomination;
}

void cDBDenomination::setDenomination( const unsigned int p_uiDenomination ) throw()
{
    m_uiDenomination = p_uiDenomination;
}

bool cDBDenomination::active() const throw()
{
    return m_bActive;
}

void cDBDenomination::setActive( const bool p_bActive ) throw()
{
    m_bActive = p_bActive;
}

string cDBDenomination::archive() const throw()
{
    return m_stArchive;
}

void cDBDenomination::setArchive( const string &p_stArchive ) throw()
{
    m_stArchive = p_stArchive;
}
