//====================================================================================
//
// Belenus Server alkalmazas © Pagony Multimedia Studio Bt - 2010
//
//====================================================================================
//
// Filename    : dbreasontovisit.cpp
// AppVersion  : 1.0
// FileVersion : 1.0
// Author      : Bikfalvi Tamas
//
//====================================================================================
//
//====================================================================================

#include "belenus.h"
#include "dbreasontovisit.h"

cDBReasonToVisit::cDBReasonToVisit()
{
    init();
}

cDBReasonToVisit::~cDBReasonToVisit()
{
}

void cDBReasonToVisit::init( const unsigned int p_uiId,
                             const unsigned int p_uiLicenceId,
                             const string &p_stName,
                             const bool p_bActive,
                             const string &p_stArchive ) throw()
{
    m_uiId          = p_uiId;
    m_uiLicenceId   = p_uiLicenceId;
    m_stName        = p_stName;
    m_bActive       = p_bActive;
    m_stArchive     = p_stArchive;
}

void cDBReasonToVisit::init( const QSqlRecord &p_obRecord ) throw()
{
    int inIdIdx         = p_obRecord.indexOf( "reasonToVisitId" );
    int inLicenceIdIdx  = p_obRecord.indexOf( "licenceId" );
    int inNameIdx       = p_obRecord.indexOf( "name" );
    int inActiveIdx     = p_obRecord.indexOf( "active" );
    int inArchiveIdx    = p_obRecord.indexOf( "archive" );

    init( p_obRecord.value( inIdIdx ).toInt(),
          p_obRecord.value( inLicenceIdIdx ).toInt(),
          p_obRecord.value( inNameIdx ).toString().toStdString(),
          p_obRecord.value( inActiveIdx ).toBool(),
          p_obRecord.value( inArchiveIdx ).toString().toStdString() );
}

void cDBReasonToVisit::load( const unsigned int p_uiId ) throw( cSevException )
{
    cTracer obTrace( "cDBReasonToVisit::load", QString( "id: %1" ).arg( p_uiId ).toStdString() );

    QSqlQuery *poQuery = g_poDB->executeQTQuery( QString( "SELECT * FROM reasonToVisit WHERE reasonToVisitId = %1" ).arg( p_uiId ) );

    if( poQuery->size() != 1 )
        throw cSevException( cSeverity::ERROR, "Reason to visit id not found" );

    poQuery->first();
    init( poQuery->record() );
}

void cDBReasonToVisit::load( const string &p_stName ) throw( cSevException )
{
    cTracer obTrace( "cDBReasonToVisit::load", "name: \""  + p_stName + "\"" );

    QSqlQuery *poQuery = g_poDB->executeQTQuery( "SELECT * FROM reasonToVisit WHERE name = \"" + QString::fromStdString( p_stName ) + "\"" );

    if( poQuery->size() != 1 )
        throw cSevException( cSeverity::ERROR, "Reason to visit name not found" );

    poQuery->first();
    init( poQuery->record() );
}

void cDBReasonToVisit::save() throw( cSevException )
{
    cTracer obTrace( "cDBReasonToVisit::save" );
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
    qsQuery += " reasonToVisit SET ";
    qsQuery += QString( "licenceId = \"%1\", " ).arg( m_uiLicenceId );
    qsQuery += QString( "name = \"%1\", " ).arg( QString::fromStdString( m_stName ) );
    qsQuery += QString( "active = %1" ).arg( m_bActive );
    qsQuery += QString( "archive = \"%1\" " ).arg( QString::fromStdString( m_stArchive ) );
    if( m_uiId )
    {
        qsQuery += QString( " WHERE reasonToVisitId = %1" ).arg( m_uiId );
    }

    QSqlQuery  *poQuery = g_poDB->executeQTQuery( qsQuery );
    if( !m_uiId && poQuery ) m_uiId = poQuery->lastInsertId().toUInt();
    if( poQuery ) delete poQuery;
}

void cDBReasonToVisit::remove() throw( cSevException )
{
    cTracer obTrace( "cDBReasonToVisit::remove" );

    if( m_uiId )
    {
        QString  qsQuery;

        if( m_stArchive.compare( "NEW" ) == 0 )
        {
            qsQuery = "DELETE FROM reasonToVisit ";
        }
        else
        {
            qsQuery = "UPDATE reasonToVisit SET active=0, archive=\"MOD\" ";
        }
        qsQuery += QString( " WHERE reasonToVisitId = %1" ).arg( m_uiId );

        QSqlQuery  *poQuery = g_poDB->executeQTQuery( qsQuery );
        if( poQuery ) delete poQuery;
    }
}

void cDBReasonToVisit::createNew() throw()
{
    init();
}

unsigned int cDBReasonToVisit::id() const throw()
{
    return m_uiId;
}

unsigned int cDBReasonToVisit::licenceId() const throw()
{
    return m_uiLicenceId;
}

void cDBReasonToVisit::setLicenceId( const unsigned int p_uiLicenceId ) throw()
{
    m_uiLicenceId = p_uiLicenceId;
}

string cDBReasonToVisit::name() const throw()
{
    return m_stName;
}

void cDBReasonToVisit::setName( const string &p_stName ) throw()
{
    m_stName = p_stName;
}

bool cDBReasonToVisit::active() const throw()
{
    return m_bActive;
}

void cDBReasonToVisit::setActive( const bool p_bActive ) throw()
{
    m_bActive = p_bActive;
}

string cDBReasonToVisit::archive() const throw()
{
    return m_stArchive;
}

void cDBReasonToVisit::setArchive( const string &p_stArchive ) throw()
{
    m_stArchive = p_stArchive;
}

