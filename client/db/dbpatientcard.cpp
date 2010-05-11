//====================================================================================
//
// Belenus Server alkalmazas © Pagony Multimedia Studio Bt - 2010
//
//====================================================================================
//
// Filename    : dbpatientcard.cpp
// AppVersion  : 1.0
// FileVersion : 1.0
// Author      : Bikfalvi Tamas
//
//====================================================================================
//
//====================================================================================

#include "belenus.h"
#include "dbpatientcard.h"

cDBPatientCard::cDBPatientCard()
{
    init();
}

cDBPatientCard::~cDBPatientCard()
{
}

void cDBPatientCard::init( const unsigned int p_uiId,
                             const unsigned int p_uiLicenceId,
                             const string &p_stName,
                             const string &p_stArchive ) throw()
{
    m_uiId          = p_uiId;
    m_uiLicenceId   = p_uiLicenceId;
    m_stName        = p_stName;
    m_stArchive     = p_stArchive;
}

void cDBPatientCard::init( const QSqlRecord &p_obRecord ) throw()
{
    int inIdIdx         = p_obRecord.indexOf( "patientOriginId" );
    int inLicenceIdIdx  = p_obRecord.indexOf( "licenceId" );
    int inNameIdx       = p_obRecord.indexOf( "name" );
    int inArchiveIdx    = p_obRecord.indexOf( "archive" );

    init( p_obRecord.value( inIdIdx ).toInt(),
          p_obRecord.value( inLicenceIdIdx ).toInt(),
          p_obRecord.value( inNameIdx ).toString().toStdString(),
          p_obRecord.value( inArchiveIdx ).toString().toStdString() );
}

void cDBPatientCard::load( const unsigned int p_uiId ) throw( cSevException )
{
    cTracer obTrace( "cDBPatientCard::load", QString( "id: %1" ).arg( p_uiId ).toStdString() );

    QSqlQuery *poQuery = g_poDB->executeQTQuery( QString( "SELECT * FROM patientOrigin WHERE patientOriginId = %1" ).arg( p_uiId ) );

    if( poQuery->size() != 1 )
        throw cSevException( cSeverity::ERROR, "Patientorigin id not found" );

    poQuery->first();
    init( poQuery->record() );
}

void cDBPatientCard::load( const string &p_stName ) throw( cSevException )
{
    cTracer obTrace( "cDBPatientCard::load", "name: \""  + p_stName + "\"" );

    QSqlQuery *poQuery = g_poDB->executeQTQuery( "SELECT * FROM patientOrigin WHERE name = \"" + QString::fromStdString( p_stName ) + "\"" );

    if( poQuery->size() != 1 )
        throw cSevException( cSeverity::ERROR, "Patientorigin name not found" );

    poQuery->first();
    init( poQuery->record() );
}

void cDBPatientCard::save() throw( cSevException )
{
    cTracer obTrace( "cDBPatientCard::save" );
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
    qsQuery += " patientOrigin SET ";
    qsQuery += QString( "licenceId = \"%1\", " ).arg( m_uiLicenceId );
    qsQuery += QString( "name = \"%1\", " ).arg( QString::fromStdString( m_stName ) );
    qsQuery += QString( "archive = \"%1\" " ).arg( QString::fromStdString( m_stArchive ) );
    if( m_uiId )
    {
        qsQuery += QString( " WHERE patientOriginId = %1" ).arg( m_uiId );
    }

    QSqlQuery  *poQuery = g_poDB->executeQTQuery( qsQuery );
    if( !m_uiId && poQuery ) m_uiId = poQuery->lastInsertId().toUInt();
    if( poQuery ) delete poQuery;
}

void cDBPatientCard::createNew() throw()
{
    init( 0, 0, "", "NEW" );
}

unsigned int cDBPatientCard::id() const throw()
{
    return m_uiId;
}

unsigned int cDBPatientCard::licenceId() const throw()
{
    return m_uiLicenceId;
}

void cDBPatientCard::setLicenceId( const unsigned int p_uiLicenceId ) throw()
{
    m_uiLicenceId = p_uiLicenceId;
}

string cDBPatientCard::name() const throw()
{
    return m_stName;
}

void cDBPatientCard::setName( const string &p_stName ) throw()
{
    m_stName = p_stName;
}

string cDBPatientCard::archive() const throw()
{
    return m_stArchive;
}

void cDBPatientCard::setArchive( const string &p_stArchive ) throw()
{
    m_stArchive = p_stArchive;
}

