//====================================================================================
//
// Belenus Server alkalmazas (c) Pagony Multimedia Studio Bt - 2010
//
//====================================================================================
//
// Filename    : dbhealthinsurance.cpp
// AppVersion  : 1.0
// FileVersion : 1.0
// Author      : Bikfalvi Tamas
//
//====================================================================================
//
//====================================================================================

#include "belenus.h"
#include "dbhealthinsurance.h"

cDBHealthInsurance::cDBHealthInsurance()
{
    init();
}

cDBHealthInsurance::~cDBHealthInsurance()
{
}

void cDBHealthInsurance::init( const unsigned int p_uiId,
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

void cDBHealthInsurance::init( const QSqlRecord &p_obRecord ) throw()
{
    int inIdIdx         = p_obRecord.indexOf( "healthInsuranceId" );
    int inLicenceIdIdx  = p_obRecord.indexOf( "licenceId" );
    int inNameIdx       = p_obRecord.indexOf( "name" );
    int inActiveIdx     = p_obRecord.indexOf( "active" );
    int inArchiveIdx    = p_obRecord.indexOf( "archive" );

    init( p_obRecord.value( inIdIdx ).toInt(),
          p_obRecord.value( inLicenceIdIdx ).toInt(),
          p_obRecord.value( inNameIdx ).toString(),
          p_obRecord.value( inActiveIdx ).toBool(),
          p_obRecord.value( inArchiveIdx ).toString() );
}

void cDBHealthInsurance::load( const unsigned int p_uiId ) throw( cSevException )
{
    cTracer obTrace( "cDBHealthInsurance::load", QString( "id: %1" ).arg( p_uiId ) );

    QSqlQuery *poQuery = g_poDB->executeQTQuery( QString( "SELECT * FROM healthInsurances WHERE healthInsuranceId = %1" ).arg( p_uiId ) );

    if( poQuery->size() != 1 )
        throw cSevException( cSeverity::ERROR, "HealthInsurance id not found" );

    poQuery->first();
    init( poQuery->record() );
}

void cDBHealthInsurance::load( const QString &p_qsName ) throw( cSevException )
{
    cTracer obTrace( "cDBHealthInsurance::load", QString("name: \"%1\"").arg(p_qsName) );

    QSqlQuery *poQuery = g_poDB->executeQTQuery( "SELECT * FROM healthInsurances WHERE name = \"" + p_qsName + "\"" );

    if( poQuery->size() != 1 )
        throw cSevException( cSeverity::ERROR, "HealthInsurance name not found" );

    poQuery->first();
    init( poQuery->record() );
}

void cDBHealthInsurance::save() throw( cSevException )
{
    cTracer obTrace( "cDBHealthInsurance::save" );
    QString  qsQuery;

    if( m_uiId )
    {
        qsQuery = "UPDATE";

        if( m_qsArchive.compare("NEW") != 0 )
        {
            m_qsArchive = "MOD";
        }
    }
    else
    {
        qsQuery = "INSERT INTO";
        m_qsArchive = "NEW";
    }
    qsQuery += " healthInsurances SET ";
    qsQuery += QString( "licenceId = \"%1\", " ).arg( m_uiLicenceId );
    qsQuery += QString( "name = \"%1\", " ).arg( m_qsName );
    qsQuery += QString( "active = %1, " ).arg( m_bActive );
    qsQuery += QString( "archive = \"%1\" " ).arg( m_qsArchive );
    if( m_uiId )
    {
        qsQuery += QString( " WHERE healthInsuranceId = %1" ).arg( m_uiId );
    }

    QSqlQuery  *poQuery = g_poDB->executeQTQuery( qsQuery );
    if( !m_uiId && poQuery ) m_uiId = poQuery->lastInsertId().toUInt();
    if( poQuery ) delete poQuery;
}

void cDBHealthInsurance::remove() throw( cSevException )
{
    cTracer obTrace( "cDBHealthInsurance::remove" );

    if( m_uiId )
    {
        QString  qsQuery;

        if( m_qsArchive.compare( "NEW" ) == 0 )
        {
            qsQuery = "DELETE FROM healthInsurances ";
        }
        else
        {
            qsQuery = "UPDATE healthInsurances SET active=0, archive=\"MOD\" ";
        }
        qsQuery += QString( " WHERE healthInsuranceId = %1" ).arg( m_uiId );

        QSqlQuery  *poQuery = g_poDB->executeQTQuery( qsQuery );
        if( poQuery ) delete poQuery;
    }
}

void cDBHealthInsurance::createNew() throw()
{
    init();
}

unsigned int cDBHealthInsurance::id() const throw()
{
    return m_uiId;
}

unsigned int cDBHealthInsurance::licenceId() const throw()
{
    return m_uiLicenceId;
}

void cDBHealthInsurance::setLicenceId( const unsigned int p_uiLicenceId ) throw()
{
    m_uiLicenceId = p_uiLicenceId;
}

QString cDBHealthInsurance::name() const throw()
{
    return m_qsName;
}

void cDBHealthInsurance::setName( const QString &p_qsName ) throw()
{
    m_qsName = p_qsName;
}

bool cDBHealthInsurance::active() const throw()
{
    return m_bActive;
}

void cDBHealthInsurance::setActive( const bool p_bActive ) throw()
{
    m_bActive = p_bActive;
}

QString cDBHealthInsurance::archive() const throw()
{
    return m_qsArchive;
}

void cDBHealthInsurance::setArchive( const QString &p_qsArchive ) throw()
{
    m_qsArchive = p_qsArchive;
}

