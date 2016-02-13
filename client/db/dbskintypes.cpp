//====================================================================================
//
// Belenus Server alkalmazas (c) Pagony Multimedia Studio Bt - 2010
//
//====================================================================================
//
// Filename    : dbskintypes.cpp
// AppVersion  : 1.0
// FileVersion : 1.0
// Author      : Bikfalvi Tamas
//
//====================================================================================
//
//====================================================================================

#include "belenus.h"
#include "dbskintypes.h"

cDBSkinTypes::cDBSkinTypes()
{
    init();
}

cDBSkinTypes::~cDBSkinTypes()
{
}

void cDBSkinTypes::init( const unsigned int p_uiId,
                             const unsigned int p_uiLicenceId,
                             const QString &p_qsName,
                             const bool p_bActive,
                             const QString &p_qsArchive ) throw()
{
    m_uiId          = p_uiId;
    m_uiLicenceId   = p_uiLicenceId;
    m_qsName        = p_qsName;
    m_bActive       = p_bActive;
    m_qsArchive     = p_qsArchive;
}

void cDBSkinTypes::init( const QSqlRecord &p_obRecord ) throw()
{
    int inIdIdx         = p_obRecord.indexOf( "skinTypeId" );
    int inLicenceIdIdx  = p_obRecord.indexOf( "licenceId" );
    int inNameIdx       = p_obRecord.indexOf( "skinTypeName" );
    int inActiveIdx     = p_obRecord.indexOf( "active" );
    int inArchiveIdx    = p_obRecord.indexOf( "archive" );

    init( p_obRecord.value( inIdIdx ).toInt(),
          p_obRecord.value( inLicenceIdIdx ).toInt(),
          p_obRecord.value( inNameIdx ).toString(),
          p_obRecord.value( inActiveIdx ).toBool(),
          p_obRecord.value( inArchiveIdx ).toString() );
}

void cDBSkinTypes::load( const unsigned int p_uiId ) throw( cSevException )
{
    cTracer obTrace( "cDBSkinTypes::load", QString( "id: %1" ).arg( p_uiId ) );

    QSqlQuery *poQuery = g_poDB->executeQTQuery( QString( "SELECT * FROM skinTypes WHERE skinTypeId = %1" ).arg( p_uiId ) );

    if( poQuery->size() != 1 )
        throw cSevException( cSeverity::ERROR, "Skintype id not found" );

    poQuery->first();
    init( poQuery->record() );
}

void cDBSkinTypes::load( const QString &p_qsName ) throw( cSevException )
{
    cTracer obTrace( "cDBSkinTypes::load", QString("name: \"%1\"").arg(p_qsName) );

    QSqlQuery *poQuery = g_poDB->executeQTQuery( "SELECT * FROM skinTypes WHERE skinTypeName = \"" + p_qsName + "\"" );

    if( poQuery->size() != 1 )
        throw cSevException( cSeverity::ERROR, "Skintype name not found" );

    poQuery->first();
    init( poQuery->record() );
}

void cDBSkinTypes::save() throw( cSevException )
{
    cTracer obTrace( "cDBSkinTypes::save" );
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
    qsQuery += " skinTypes SET ";
    qsQuery += QString( "licenceId = \"%1\", " ).arg( m_uiLicenceId );
    qsQuery += QString( "skinTypeName = \"%1\", " ).arg( m_qsName );
    qsQuery += QString( "active = %1, " ).arg( m_bActive );
    qsQuery += QString( "archive = \"%1\" " ).arg( m_qsArchive );
    if( m_uiId )
    {
        qsQuery += QString( " WHERE skinTypeId = %1" ).arg( m_uiId );
    }

    QSqlQuery  *poQuery = g_poDB->executeQTQuery( qsQuery );
    if( !m_uiId && poQuery ) m_uiId = poQuery->lastInsertId().toUInt();
    if( poQuery ) delete poQuery;
/*
    if( m_uiId > 0 && m_uiLicenceId != 1 )
        g_obDBMirror.updateSynchronizationLevel( DB_PATIENT_ORIGIN );
    if( m_uiId > 0 && m_uiLicenceId == 0 )
        g_obDBMirror.updateGlobalSyncLevel( DB_PATIENT_ORIGIN );
*/
}

void cDBSkinTypes::remove() throw( cSevException )
{
    cTracer obTrace( "cDBSkinTypes::remove" );

    if( m_uiId )
    {
        QString  qsQuery;

//        if( m_qsArchive != "NEW" )
//        {
            qsQuery = "DELETE FROM skinTypes ";
//        }
//        else
//        {
//            qsQuery = "UPDATE skinTypes SET active=0, archive=\"MOD\" ";
//        }
        qsQuery += QString( " WHERE skinTypeId = %1" ).arg( m_uiId );

        QSqlQuery  *poQuery = g_poDB->executeQTQuery( qsQuery );
        if( poQuery ) delete poQuery;
    }
}

void cDBSkinTypes::createNew() throw()
{
    init();
}

unsigned int cDBSkinTypes::id() const throw()
{
    return m_uiId;
}

unsigned int cDBSkinTypes::licenceId() const throw()
{
    return m_uiLicenceId;
}

void cDBSkinTypes::setLicenceId( const unsigned int p_uiLicenceId ) throw()
{
    m_uiLicenceId = p_uiLicenceId;
}

QString cDBSkinTypes::name() const throw()
{
    return m_qsName;
}

void cDBSkinTypes::setName( const QString &p_qsName ) throw()
{
    m_qsName = p_qsName;
    m_qsName = m_qsName.replace( QString("\""), QString("\\\"") );
}

bool cDBSkinTypes::active() const throw()
{
    return m_bActive;
}

void cDBSkinTypes::setActive( const bool p_bActive ) throw()
{
    m_bActive = p_bActive;
}

QString cDBSkinTypes::archive() const throw()
{
    return m_qsArchive;
}

void cDBSkinTypes::setArchive( const QString &p_qsArchive ) throw()
{
    m_qsArchive = p_qsArchive;
}
