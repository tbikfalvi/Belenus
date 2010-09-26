//====================================================================================
//
// Belenus Server alkalmazas (c) Pagony Multimedia Studio Bt - 2010
//
//====================================================================================
//
// Filename    : dbcompany.cpp
// AppVersion  : 1.0
// FileVersion : 1.0
// Author      : Bikfalvi Tamas
//
//====================================================================================
//
//====================================================================================

#include "belenus.h"
#include "dbcompany.h"

cDBCompany::cDBCompany()
{
    init();
}

cDBCompany::~cDBCompany()
{
}

void cDBCompany::init( const unsigned int p_uiId,
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

void cDBCompany::init( const QSqlRecord &p_obRecord ) throw()
{
    int inIdIdx         = p_obRecord.indexOf( "companyId" );
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

void cDBCompany::load( const unsigned int p_uiId ) throw( cSevException )
{
    cTracer obTrace( "cDBCompany::load", QString( "id: %1" ).arg( p_uiId ) );

    QSqlQuery *poQuery = g_poDB->executeQTQuery( QString( "SELECT * FROM companies WHERE companyId = %1" ).arg( p_uiId ) );

    if( poQuery->size() != 1 )
        throw cSevException( cSeverity::ERROR, "Company id not found" );

    poQuery->first();
    init( poQuery->record() );
}

void cDBCompany::load( const QString &p_qsName ) throw( cSevException )
{
    cTracer obTrace( "cDBCompany::load", QString("name: \"%1\"").arg(p_qsName) );

    QSqlQuery *poQuery = g_poDB->executeQTQuery( "SELECT * FROM companies WHERE name = \"" + p_qsName + "\"" );

    if( poQuery->size() != 1 )
        throw cSevException( cSeverity::ERROR, "Company name not found" );

    poQuery->first();
    init( poQuery->record() );
}

void cDBCompany::save() throw( cSevException )
{
    cTracer obTrace( "cDBCompany::save" );
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
    qsQuery += " companies SET ";
    qsQuery += QString( "licenceId = \"%1\", " ).arg( m_uiLicenceId );
    qsQuery += QString( "name = \"%1\", " ).arg( m_qsName );
    qsQuery += QString( "active = %1, " ).arg( m_bActive );
    qsQuery += QString( "archive = \"%1\" " ).arg( m_qsArchive );
    if( m_uiId )
    {
        qsQuery += QString( " WHERE companyId = %1" ).arg( m_uiId );
    }

    QSqlQuery  *poQuery = g_poDB->executeQTQuery( qsQuery );
    if( !m_uiId && poQuery ) m_uiId = poQuery->lastInsertId().toUInt();
    if( poQuery ) delete poQuery;
}

void cDBCompany::remove() throw( cSevException )
{
    cTracer obTrace( "cDBCompany::remove" );

    if( m_uiId )
    {
        QString  qsQuery;

        if( m_qsArchive.compare( "NEW" ) == 0 )
        {
            qsQuery = "DELETE FROM companies ";
        }
        else
        {
            qsQuery = "UPDATE companies SET active=0, archive=\"MOD\" ";
        }
        qsQuery += QString( " WHERE companyId = %1" ).arg( m_uiId );

        QSqlQuery  *poQuery = g_poDB->executeQTQuery( qsQuery );
        if( poQuery ) delete poQuery;
    }
}

void cDBCompany::createNew() throw()
{
    init();
}

unsigned int cDBCompany::id() const throw()
{
    return m_uiId;
}

unsigned int cDBCompany::licenceId() const throw()
{
    return m_uiLicenceId;
}

void cDBCompany::setLicenceId( const unsigned int p_uiLicenceId ) throw()
{
    m_uiLicenceId = p_uiLicenceId;
}

QString cDBCompany::name() const throw()
{
    return m_qsName;
}

void cDBCompany::setName( const QString &p_qsName ) throw()
{
    m_qsName = p_qsName;
    m_qsName = m_qsName.replace( QString("\""), QString("\\\"") );
}

bool cDBCompany::active() const throw()
{
    return m_bActive;
}

void cDBCompany::setActive( const bool p_bActive ) throw()
{
    m_bActive = p_bActive;
}

QString cDBCompany::archive() const throw()
{
    return m_qsArchive;
}

void cDBCompany::setArchive( const QString &p_qsArchive ) throw()
{
    m_qsArchive = p_qsArchive;
}

