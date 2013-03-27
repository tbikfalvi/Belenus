//====================================================================================
//
// Belenus Server alkalmazas (c) Pagony Multimedia Studio Bt - 2010
//
//====================================================================================
//
// Filename    : dbpanelstatuses.cpp
// AppVersion  : 1.0
// FileVersion : 1.0
// Author      : Bikfalvi Tamas
//
//====================================================================================
//
//====================================================================================

#include "belenus.h"
#include "dbpanelstatuses.h"

cDBPanelStatuses::cDBPanelStatuses()
{
    init();
}

cDBPanelStatuses::~cDBPanelStatuses()
{
}

void cDBPanelStatuses::init( const unsigned int p_uiId,
                             const unsigned int p_uiLicenceId,
                             const unsigned int p_uiPanelTypeId,
                             const unsigned int p_uiSequenceNumber,
                             const QString &p_qsName,
                             const unsigned int p_uiLength,
                             const unsigned int p_uiActivateCommand,
                             const QString &p_qsModified,
                             const bool p_bActive,
                             const QString &p_qsArchive ) throw()
{
    m_uiId              = p_uiId;
    m_uiLicenceId       = p_uiLicenceId;
    m_uiPanelTypeId     = p_uiPanelTypeId;
    m_uiSequenceNumber  = p_uiSequenceNumber;
    m_qsName            = p_qsName;
    m_uiLength          = p_uiLength;
    m_uiActivateCommand = p_uiActivateCommand;
    m_qsModified        = p_qsModified;
    m_bActive           = p_bActive;
    m_qsArchive         = p_qsArchive;
}

void cDBPanelStatuses::init( const QSqlRecord &p_obRecord ) throw()
{
    int inIdIdx                 = p_obRecord.indexOf( "panelStatusId" );
    int inLicenceIdIdx          = p_obRecord.indexOf( "licenceId" );
    int inPanelTypeIdIdx        = p_obRecord.indexOf( "panelTypeId" );
    int inSequenceNumberIdx     = p_obRecord.indexOf( "seqNumber" );
    int inNameIdx               = p_obRecord.indexOf( "name" );
    int inLengthIdx             = p_obRecord.indexOf( "length" );
    int inActivateCommandIdx    = p_obRecord.indexOf( "activateCmd" );
    int inModifiedIdx           = p_obRecord.indexOf( "modified" );
    int inActiveIdx             = p_obRecord.indexOf( "active" );
    int inArchiveIdx            = p_obRecord.indexOf( "archive" );

    init( p_obRecord.value( inIdIdx ).toInt(),
          p_obRecord.value( inLicenceIdIdx ).toInt(),
          p_obRecord.value( inPanelTypeIdIdx ).toInt(),
          p_obRecord.value( inSequenceNumberIdx ).toInt(),
          p_obRecord.value( inNameIdx ).toString(),
          p_obRecord.value( inLengthIdx ).toInt(),
          p_obRecord.value( inActivateCommandIdx ).toInt(),
          p_obRecord.value( inModifiedIdx ).toString(),
          p_obRecord.value( inActiveIdx ).toBool(),
          p_obRecord.value( inArchiveIdx ).toString() );
}

void cDBPanelStatuses::load( const unsigned int p_uiId ) throw( cSevException )
{
    cTracer obTrace( "cDBPanelStatuses::load", QString( "id: %1" ).arg( p_uiId ) );

    QSqlQuery *poQuery = g_poDB->executeQTQuery( QString( "SELECT * FROM panelStatuses WHERE panelStatusId = %1" ).arg( p_uiId ) );

    if( poQuery->size() != 1 )
        throw cSevException( cSeverity::ERROR, "Panelstatus id not found" );

    poQuery->first();
    init( poQuery->record() );
}

void cDBPanelStatuses::load( const QString &p_qsName ) throw( cSevException )
{
    cTracer obTrace( "cDBPanelStatuses::load", QString("name: \"%1\"").arg(p_qsName) );

    QSqlQuery *poQuery = g_poDB->executeQTQuery( "SELECT * FROM panelStatuses WHERE name = \"" + p_qsName + "\"" );

    if( poQuery->size() != 1 )
        throw cSevException( cSeverity::ERROR, "Panelstatus name not found" );

    poQuery->first();
    init( poQuery->record() );
}

void cDBPanelStatuses::save() throw( cSevException )
{
    cTracer obTrace( "cDBPanelStatuses::save" );
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
    qsQuery += " panelStatuses SET ";
    qsQuery += QString( "licenceId = %1, " ).arg( m_uiLicenceId );
    qsQuery += QString( "panelTypeId = %1, " ).arg( m_uiPanelTypeId );
    qsQuery += QString( "seqNumber = %1, " ).arg( m_uiSequenceNumber );
    qsQuery += QString( "name = \"%1\", " ).arg( m_qsName );
    qsQuery += QString( "length = %1, " ).arg( m_uiLength );
    qsQuery += QString( "activateCmd = %1, " ).arg( m_uiActivateCommand );
    qsQuery += QString( "modified = \"%1\", " ).arg( QDateTime::currentDateTime().toString( QString("yyyy-MM-dd hh:mm:ss") ) );
    qsQuery += QString( "active = %1, " ).arg( m_bActive );
    qsQuery += QString( "archive = \"%1\" " ).arg( m_qsArchive );
    if( m_uiId )
    {
        qsQuery += QString( " WHERE panelStatusId = %1" ).arg( m_uiId );
    }

    QSqlQuery  *poQuery = g_poDB->executeQTQuery( qsQuery );
    if( !m_uiId && poQuery ) m_uiId = poQuery->lastInsertId().toUInt();
    if( poQuery ) delete poQuery;
/*
    if( m_uiId > 0 && m_uiLicenceId != 1 )
        g_obDBMirror.updateSynchronizationLevel( DB_PANEL_STATUS );
    if( m_uiId > 0 && m_uiLicenceId == 0 )
        g_obDBMirror.updateGlobalSyncLevel( DB_PANEL_STATUS );
*/
}

void cDBPanelStatuses::remove() throw( cSevException )
{
    cTracer obTrace( "cDBPanelStatuses::remove" );

    if( m_uiId )
    {
        QString  qsQuery;

        if( m_qsArchive != "NEW" )
        {
            qsQuery = "DELETE FROM panelStatuses ";
        }
        else
        {
            qsQuery = "UPDATE panelStatuses SET active=0, archive=\"MOD\" ";
        }
        qsQuery += QString( " WHERE panelStatusId = %1" ).arg( m_uiId );

        QSqlQuery  *poQuery = g_poDB->executeQTQuery( qsQuery );
        if( poQuery ) delete poQuery;
    }
}

void cDBPanelStatuses::createNew() throw()
{
    init();
}

unsigned int cDBPanelStatuses::id() const throw()
{
    return m_uiId;
}

unsigned int cDBPanelStatuses::licenceId() const throw()
{
    return m_uiLicenceId;
}

void cDBPanelStatuses::setLicenceId( const unsigned int p_uiLicenceId ) throw()
{
    m_uiLicenceId = p_uiLicenceId;
}

unsigned int cDBPanelStatuses::panelTypeId() const throw()
{
    return m_uiPanelTypeId;
}

void cDBPanelStatuses::setPanelTypeId( const unsigned int p_uiPanelTypeId ) throw()
{
    m_uiPanelTypeId = p_uiPanelTypeId;
}

unsigned int cDBPanelStatuses::sequenceNumber() const throw()
{
    return m_uiSequenceNumber;
}

void cDBPanelStatuses::setSequenceNumber( const unsigned int p_uiSequenceNumber ) throw()
{
    m_uiSequenceNumber = p_uiSequenceNumber;
}

QString cDBPanelStatuses::name() const throw()
{
    return m_qsName;
}

void cDBPanelStatuses::setName( const QString &p_qsName ) throw()
{
    m_qsName = p_qsName;
    m_qsName = m_qsName.replace( QString("\""), QString("\\\"") );
}

unsigned int cDBPanelStatuses::length() const throw()
{
    return m_uiLength;
}

void cDBPanelStatuses::setLength( const unsigned int p_uiLength ) throw()
{
    m_uiLength = p_uiLength;
}

unsigned int cDBPanelStatuses::activateCommand() const throw()
{
    return m_uiActivateCommand;
}

void cDBPanelStatuses::setActivateCommand( const unsigned int p_uiActivateCommand ) throw()
{
    m_uiActivateCommand = p_uiActivateCommand;
}

QString cDBPanelStatuses::modified() const throw()
{
    return m_qsModified;
}

bool cDBPanelStatuses::active() const throw()
{
    return m_bActive;
}

void cDBPanelStatuses::setActive( const bool p_bActive ) throw()
{
    m_bActive = p_bActive;
}

QString cDBPanelStatuses::archive() const throw()
{
    return m_qsArchive;
}

void cDBPanelStatuses::setArchive( const QString &p_qsArchive ) throw()
{
    m_qsArchive = p_qsArchive;
}

