//====================================================================================
//
// Belenus Server alkalmazas (c) Pagony Multimedia Studio Bt - 2010
//
//====================================================================================
//
// Filename    : dbapplicationaction.cpp
// AppVersion  : 1.0
// FileVersion : 1.0
// Author      : Bikfalvi Tamas
//
//====================================================================================
//
//====================================================================================

#include "belenus.h"
#include "dbapplicationaction.h"

cDBApplicationAction::cDBApplicationAction()
{
    init();
}

cDBApplicationAction::~cDBApplicationAction()
{
}

void cDBApplicationAction::init( const unsigned int p_uiId,
                                 const unsigned int p_uiLicenceId,
                                 const QString &p_stName,
                                 const QString &p_qsModified,
                                 const bool p_bActive,
                                 const QString &p_stArchive ) throw()
{
    m_uiId              = p_uiId;
    m_uiLicenceId       = p_uiLicenceId;
    m_stName            = p_stName;
    m_qsModified        = p_qsModified;
    m_bActive           = p_bActive;
    m_stArchive         = p_stArchive;
}

void cDBApplicationAction::init( const QSqlRecord &p_obRecord ) throw()
{
    int inIdIdx             = p_obRecord.indexOf( "applicationActionId" );
    int inLicenceIdIdx      = p_obRecord.indexOf( "licenceId" );
    int inNameIdx           = p_obRecord.indexOf( "name" );
    int inModifiedIdx       = p_obRecord.indexOf( "modified" );
    int inActiveIdx         = p_obRecord.indexOf( "active" );
    int inArchiveIdx        = p_obRecord.indexOf( "archive" );

    init( p_obRecord.value( inIdIdx ).toInt(),
          p_obRecord.value( inLicenceIdIdx ).toInt(),
          p_obRecord.value( inNameIdx ).toString(),
          p_obRecord.value( inModifiedIdx ).toString(),
          p_obRecord.value( inActiveIdx ).toBool(),
          p_obRecord.value( inArchiveIdx ).toString() );
}

void cDBApplicationAction::load( const unsigned int p_uiId ) throw( cSevException )
{
    cTracer obTrace( "cDBApplicationAction::load", QString( "id: %1" ).arg( p_uiId ) );

    QSqlQuery *poQuery = g_poDB->executeQTQuery( QString( "SELECT * FROM applicationAction WHERE applicationActionId = %1" ).arg( p_uiId ) );

    if( poQuery->size() != 1 )
        throw cSevException( cSeverity::ERROR, "Application action id not found" );

    poQuery->first();
    init( poQuery->record() );
}

void cDBApplicationAction::load( const QString &p_stName ) throw( cSevException )
{
    cTracer obTrace( "cDBApplicationAction::load", QString("name: \"%1\"").arg(p_stName) );

    QSqlQuery *poQuery = g_poDB->executeQTQuery( "SELECT * FROM applicationAction WHERE name = \"" + p_stName + "\"" );

    if( poQuery->size() != 1 )
        throw cSevException( cSeverity::ERROR, "Application action name not found" );

    poQuery->first();
    init( poQuery->record() );
}

void cDBApplicationAction::save() throw( cSevException )
{
    cTracer obTrace( "cDBApplicationAction::save" );
    QString  qsQuery;

    if( m_uiId )
    {
        qsQuery = "UPDATE";
        m_stArchive = "MOD";
    }
    else
    {
        qsQuery = "INSERT INTO";
        m_stArchive = "NEW";
    }
    qsQuery += " applicationAction SET ";
    qsQuery += QString( "licenceId = \"%1\", " ).arg( m_uiLicenceId );
    qsQuery += QString( "name = \"%1\", " ).arg( m_stName );
    qsQuery += QString( "modified = \"%1\", " ).arg( QDateTime::currentDateTime().toString( QString("yyyy-MM-dd hh:mm:ss") ) );
    qsQuery += QString( "active = %1, " ).arg( m_bActive );
    qsQuery += QString( "archive = \"%1\" " ).arg( m_stArchive );
    if( m_uiId )
    {
        qsQuery += QString( " WHERE applicationActionId = %1" ).arg( m_uiId );
    }

    QSqlQuery  *poQuery = g_poDB->executeQTQuery( qsQuery );
    if( !m_uiId && poQuery ) m_uiId = poQuery->lastInsertId().toUInt();
    if( poQuery ) delete poQuery;
/*
    if( m_uiId > 0 && m_uiLicenceId != 1 )
        g_obDBMirror.updateSynchronizationLevel( DB_REASON_TO_VISIT );
    if( m_uiId > 0 && m_uiLicenceId == 0 )
        g_obDBMirror.updateGlobalSyncLevel( DB_REASON_TO_VISIT );
*/
}

void cDBApplicationAction::remove() throw( cSevException )
{
    cTracer obTrace( "cDBApplicationAction::remove" );

    if( m_uiId )
    {
        QString  qsQuery;

        if( m_stArchive.compare( "NEW" ) == 0 )
        {
            qsQuery = "DELETE FROM applicationAction ";
        }
        else
        {
            qsQuery = "UPDATE applicationAction SET active=0, archive=\"MOD\" ";
        }
        qsQuery += QString( " WHERE applicationActionId = %1" ).arg( m_uiId );

        QSqlQuery  *poQuery = g_poDB->executeQTQuery( qsQuery );
        if( poQuery ) delete poQuery;
    }
}

void cDBApplicationAction::createNew() throw()
{
    init();
}

unsigned int cDBApplicationAction::id() const throw()
{
    return m_uiId;
}

unsigned int cDBApplicationAction::licenceId() const throw()
{
    return m_uiLicenceId;
}

void cDBApplicationAction::setLicenceId( const unsigned int p_uiLicenceId ) throw()
{
    m_uiLicenceId = p_uiLicenceId;
}

QString cDBApplicationAction::name() const throw()
{
    return m_stName;
}

void cDBApplicationAction::setName( const QString &p_stName ) throw()
{
    m_stName = p_stName;
}

QString cDBApplicationAction::modified() const throw()
{
    return m_qsModified;
}

bool cDBApplicationAction::active() const throw()
{
    return m_bActive;
}

void cDBApplicationAction::setActive( const bool p_bActive ) throw()
{
    m_bActive = p_bActive;
}

QString cDBApplicationAction::archive() const throw()
{
    return m_stArchive;
}

void cDBApplicationAction::setArchive( const QString &p_stArchive ) throw()
{
    m_stArchive = p_stArchive;
}

