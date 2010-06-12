//====================================================================================
//
// Belenus Server alkalmazas © Pagony Multimedia Studio Bt - 2010
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
                             const string &p_stName,
                             const unsigned int p_uiLength,
                             const unsigned int p_uiActivateCommand,
                             const bool p_bActive,
                             const string &p_stArchive ) throw()
{
    m_uiId              = p_uiId;
    m_uiLicenceId       = p_uiLicenceId;
    m_uiPanelTypeId     = p_uiPanelTypeId;
    m_uiSequenceNumber  = p_uiSequenceNumber;
    m_stName            = p_stName;
    m_uiLength          = p_uiLength;
    m_uiActivateCommand = p_uiActivateCommand;
    m_bActive           = p_bActive;
    m_stArchive         = p_stArchive;
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
    int inActiveIdx             = p_obRecord.indexOf( "active" );
    int inArchiveIdx            = p_obRecord.indexOf( "archive" );

    init( p_obRecord.value( inIdIdx ).toInt(),
          p_obRecord.value( inLicenceIdIdx ).toInt(),
          p_obRecord.value( inPanelTypeIdIdx ).toInt(),
          p_obRecord.value( inSequenceNumberIdx ).toInt(),
          p_obRecord.value( inNameIdx ).toString().toStdString(),
          p_obRecord.value( inLengthIdx ).toInt(),
          p_obRecord.value( inActivateCommandIdx ).toInt(),
          p_obRecord.value( inActiveIdx ).toBool(),
          p_obRecord.value( inArchiveIdx ).toString().toStdString() );
}

void cDBPanelStatuses::load( const unsigned int p_uiId ) throw( cSevException )
{
    cTracer obTrace( "cDBPanelStatuses::load", QString( "id: %1" ).arg( p_uiId ).toStdString() );

    QSqlQuery *poQuery = g_poDB->executeQTQuery( QString( "SELECT * FROM panelStatuses WHERE panelStatusId = %1" ).arg( p_uiId ) );

    if( poQuery->size() != 1 )
        throw cSevException( cSeverity::ERROR, "Panelstatus id not found" );

    poQuery->first();
    init( poQuery->record() );
}

void cDBPanelStatuses::load( const string &p_stName ) throw( cSevException )
{
    cTracer obTrace( "cDBPanelStatuses::load", "name: \""  + p_stName + "\"" );

    QSqlQuery *poQuery = g_poDB->executeQTQuery( "SELECT * FROM panelStatuses WHERE name = \"" + QString::fromStdString( p_stName ) + "\"" );

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
    qsQuery += " panelStatuses SET ";
    qsQuery += QString( "licenceId = %1, " ).arg( m_uiLicenceId );
    qsQuery += QString( "panelTypeId = %1, " ).arg( m_uiPanelTypeId );
    qsQuery += QString( "seqNumber = %1, " ).arg( m_uiSequenceNumber );
    qsQuery += QString( "name = \"%1\", " ).arg( QString::fromStdString(m_stName) );
    qsQuery += QString( "length = %1, " ).arg( m_uiLength );
    qsQuery += QString( "activateCmd = %1, " ).arg( m_uiActivateCommand );
    qsQuery += QString( "active = %1, " ).arg( m_bActive );
    qsQuery += QString( "archive = \"%1\" " ).arg( QString::fromStdString( m_stArchive ) );
    if( m_uiId )
    {
        qsQuery += QString( " WHERE panelStatusId = %1" ).arg( m_uiId );
    }

    QSqlQuery  *poQuery = g_poDB->executeQTQuery( qsQuery );
    if( !m_uiId && poQuery ) m_uiId = poQuery->lastInsertId().toUInt();
    if( poQuery ) delete poQuery;
}

void cDBPanelStatuses::remove() throw( cSevException )
{
    cTracer obTrace( "cDBPanelStatuses::remove" );

    if( m_uiId )
    {
        QString  qsQuery;

        if( m_stArchive.compare( "NEW" ) == 0 )
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

string cDBPanelStatuses::name() const throw()
{
    return m_stName;
}

void cDBPanelStatuses::setName( const string &p_stName ) throw()
{
    m_stName = p_stName;
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

bool cDBPanelStatuses::active() const throw()
{
    return m_bActive;
}

void cDBPanelStatuses::setActive( const bool p_bActive ) throw()
{
    m_bActive = p_bActive;
}

string cDBPanelStatuses::archive() const throw()
{
    return m_stArchive;
}

void cDBPanelStatuses::setArchive( const string &p_stArchive ) throw()
{
    m_stArchive = p_stArchive;
}

