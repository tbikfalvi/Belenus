//====================================================================================
//
// Belenus Server alkalmazas (c) Pagony Multimedia Studio Bt - 2010
//
//====================================================================================
//
// Filename    : dbcassadenomination.cpp
// AppVersion  : 1.0
// FileVersion : 1.0
// Author      : Bikfalvi Tamas
//
//====================================================================================
//
//====================================================================================

#include "belenus.h"
#include "dbcassadenomination.h"

cDBCassaDenomination::cDBCassaDenomination()
{
    init();
}

cDBCassaDenomination::~cDBCassaDenomination()
{
}

void cDBCassaDenomination::init( const unsigned int p_uiDenominationId,
                                 const unsigned int p_uiCassaId,
                                 const unsigned int p_uiLicenceId,
                                 const int p_inValue,
                                 const string &p_stArchive ) throw()
{
    m_uiDenominationId  = p_uiDenominationId;
    m_uiCassaId         = p_uiCassaId;
    m_uiLicenceId       = p_uiLicenceId;
    m_inValue           = p_inValue;
    m_stArchive         = p_stArchive;

    m_bNewRecord        = false;
}

void cDBCassaDenomination::init( const QSqlRecord &p_obRecord ) throw()
{
    int inDenominationIdIdx = p_obRecord.indexOf( "denominationId" );
    int inCassaIdIdx        = p_obRecord.indexOf( "cassaId" );
    int inLicenceIdIdx      = p_obRecord.indexOf( "licenceId" );
    int inValueIdx          = p_obRecord.indexOf( "value" );
    int inArchiveIdx        = p_obRecord.indexOf( "archive" );

    init( p_obRecord.value( inDenominationIdIdx ).toUInt(),
          p_obRecord.value( inCassaIdIdx ).toUInt(),
          p_obRecord.value( inLicenceIdIdx ).toUInt(),
          p_obRecord.value( inValueIdx ).toInt(),
          p_obRecord.value( inArchiveIdx ).toString().toStdString() );
}

void cDBCassaDenomination::load( const unsigned int p_uiDenominationId,
                                 const unsigned int p_uiCassaId,
                                 const unsigned int p_uiLicenceId ) throw( cSevException )
{
    cTracer obTrace( "cDBCassaDenomination::load", QString( "id1: %1 id2: %2 id3: %3 " ).arg( p_uiDenominationId ).arg( p_uiCassaId ).arg( p_uiLicenceId ) );

    QSqlQuery *poQuery = g_poDB->executeQTQuery( QString( "SELECT * FROM cassaDenominations WHERE denominationId = %1 AND cassaID = %2 AND licenceId = %3" ).arg( p_uiDenominationId ).arg( p_uiCassaId ).arg( p_uiLicenceId ) );

    if( poQuery->size() != 1 )
        throw cSevException( cSeverity::ERROR, "Cassa denomination not found" );

    poQuery->first();
    init( poQuery->record() );
}

void cDBCassaDenomination::save() throw( cSevException )
{
    cTracer obTrace( "cDBCassaDenomination::save" );
    QString  qsQuery;

    if( !m_bNewRecord )
    {
        qsQuery = "UPDATE";
        qsQuery += " cassaDenominations SET ";
        m_stArchive = "MOD";
    }
    else
    {
        qsQuery = "INSERT INTO";
        qsQuery += " cassaDenominations SET ";
        qsQuery += QString( "denominationId = \"%1\", " ).arg( m_uiDenominationId );
        qsQuery += QString( "cassaId = \"%1\", " ).arg( m_uiCassaId );
        qsQuery += QString( "licenceId = \"%1\", " ).arg( m_uiLicenceId );

        m_stArchive = "NEW";
    }
    qsQuery += QString( "value = %1, " ).arg( m_inValue );
    qsQuery += QString( "archive = \"%1\" " ).arg( QString::fromStdString( m_stArchive ) );
    if( !m_bNewRecord )
    {
        qsQuery += QString( " WHERE denominationId = %1 AND cassaID = %2 AND licenceId = %3" ).arg( m_uiDenominationId ).arg( m_uiCassaId ).arg( m_uiLicenceId );
    }

    QSqlQuery  *poQuery = g_poDB->executeQTQuery( qsQuery );
    if( poQuery ) delete poQuery;
    m_bNewRecord = false;

//    if( m_uiId > 0 && m_uiLicenceId != 1 )
//        g_obDBMirror.updateSynchronizationLevel( DB_CASSA_DENOMINATION );
//    if( m_uiId > 0 && m_uiLicenceId == 0 )
//        g_obDBMirror.updateGlobalSyncLevel( DB_CASSA_DENOMINATION );
}

void cDBCassaDenomination::remove() throw( cSevException )
{
    cTracer obTrace( "cDBCassaDenomination::remove" );

    if( !m_bNewRecord )
    {
        QString  qsQuery;

        if( m_stArchive.compare( "NEW" ) == 0 )
        {
            qsQuery = "DELETE FROM cassaDenominations ";
        }
        else
        {
            qsQuery = "UPDATE cassaDenominations SET archive=\"DEL\" ";
        }
        qsQuery += QString( " WHERE denominationId = %1 AND cassaID = %2 AND licenceId = %3" ).arg( m_uiDenominationId ).arg( m_uiCassaId ).arg( m_uiLicenceId );

        QSqlQuery  *poQuery = g_poDB->executeQTQuery( qsQuery );
        if( poQuery ) delete poQuery;
    }
}

void cDBCassaDenomination::createNew() throw()
{
    init();
    m_bNewRecord = true;
}

unsigned int cDBCassaDenomination::denominationId() const throw()
{
    return m_uiDenominationId;
}

void cDBCassaDenomination::setDenominationId( const unsigned int p_uiDenominationId ) throw()
{
    m_uiDenominationId = p_uiDenominationId;
}

unsigned int cDBCassaDenomination::cassaId() const throw()
{
    return m_uiCassaId;
}

void cDBCassaDenomination::setCassaId( const unsigned int p_uiCassaId ) throw()
{
    m_uiCassaId = p_uiCassaId;
}

unsigned int cDBCassaDenomination::licenceId() const throw()
{
    return m_uiLicenceId;
}

void cDBCassaDenomination::setLicenceId( const unsigned int p_uiLicenceId ) throw()
{
    m_uiLicenceId = p_uiLicenceId;
}

int cDBCassaDenomination::value() const throw()
{
    return m_inValue;
}

void cDBCassaDenomination::setValue( const int p_inValue ) throw()
{
    m_inValue = p_inValue;
}

string cDBCassaDenomination::archive() const throw()
{
    return m_stArchive;
}

void cDBCassaDenomination::setArchive( const string &p_stArchive ) throw()
{
    m_stArchive = p_stArchive;
}

