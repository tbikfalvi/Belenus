//====================================================================================
//
// Belenus Server alkalmazas (c) Pagony Multimedia Studio Bt - 2010
//
//====================================================================================
//
// Filename    : dbdoctor.cpp
// AppVersion  : 1.0
// FileVersion : 1.0
// Author      : Bikfalvi Tamas
//
//====================================================================================
//
//====================================================================================

#include "belenus.h"
#include "dbdoctor.h"

cDBDoctor::cDBDoctor()
{
    init();
}

cDBDoctor::~cDBDoctor()
{
}

void cDBDoctor::init( const unsigned int p_uiId,
                      const unsigned int p_uiLicenceId,
                      const unsigned int p_uiDoctorTypeId,
                      const QString &p_qsName,
                      const QString &p_qsLicence,
                      const QString &p_qsData,
                      const bool p_bActive,
                      const QString &p_qsArchive ) throw()
{
    m_uiId              = p_uiId;
    m_uiLicenceId       = p_uiLicenceId;
    m_uiDoctorTypeId    = p_uiDoctorTypeId;
    m_qsName            = p_qsName;
    m_qsLicence         = p_qsLicence;
    m_qsData            = p_qsData;
    m_bActive           = p_bActive;
    m_qsArchive         = p_qsArchive;
}

void cDBDoctor::init( const QSqlRecord &p_obRecord ) throw()
{
    int inIdIdx             = p_obRecord.indexOf( "doctorId" );
    int inLicenceIdIdx      = p_obRecord.indexOf( "licenceId" );
    int inDoctorTypeIdIdx   = p_obRecord.indexOf( "doctorTypeId" );
    int inNameIdx           = p_obRecord.indexOf( "name" );
    int inLicenceIdx        = p_obRecord.indexOf( "doctorLicence" );
    int inDataIdx           = p_obRecord.indexOf( "data" );
    int inActiveIdx         = p_obRecord.indexOf( "active" );
    int inArchiveIdx        = p_obRecord.indexOf( "archive" );

    init( p_obRecord.value( inIdIdx ).toInt(),
          p_obRecord.value( inLicenceIdIdx ).toUInt(),
          p_obRecord.value( inDoctorTypeIdIdx ).toUInt(),
          p_obRecord.value( inNameIdx ).toString(),
          p_obRecord.value( inLicenceIdx ).toString(),
          p_obRecord.value( inDataIdx ).toString(),
          p_obRecord.value( inActiveIdx ).toBool(),
          p_obRecord.value( inArchiveIdx ).toString() );
}

void cDBDoctor::load( const unsigned int p_uiId ) throw( cSevException )
{
    cTracer obTrace( "cDBDoctor::load", QString( "id: %1" ).arg( p_uiId ) );

    QSqlQuery *poQuery = g_poDB->executeQTQuery( QString( "SELECT * FROM doctors WHERE doctorId = %1" ).arg( p_uiId ) );

    if( poQuery->size() != 1 )
        throw cSevException( cSeverity::ERROR, "Doctor id not found" );

    poQuery->first();
    init( poQuery->record() );
}

void cDBDoctor::load( const QString &p_qsName ) throw( cSevException )
{
    cTracer obTrace( "cDBDoctor::load", QString("name: \"%1\"").arg(p_qsName) );

    QSqlQuery *poQuery = g_poDB->executeQTQuery( "SELECT * FROM doctors WHERE name = \"" + p_qsName + "\"" );

    if( poQuery->size() != 1 )
        throw cSevException( cSeverity::ERROR, "Doctor name not found" );

    poQuery->first();
    init( poQuery->record() );
}

void cDBDoctor::save() throw( cSevException )
{
    cTracer obTrace( "cDBDoctor::save" );
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
    qsQuery += " doctors SET ";
    qsQuery += QString( "licenceId = \"%1\", " ).arg( m_uiLicenceId );
    qsQuery += QString( "doctorTypeId = \"%1\", " ).arg( m_uiDoctorTypeId );
    qsQuery += QString( "name = \"%1\", " ).arg( m_qsName );
    qsQuery += QString( "doctorLicence = \"%1\", " ).arg( m_qsLicence );
    qsQuery += QString( "data = \"%1\", " ).arg( m_qsData );
    qsQuery += QString( "active = %1, " ).arg( m_bActive );
    qsQuery += QString( "archive = \"%1\" " ).arg( m_qsArchive );
    if( m_uiId )
    {
        qsQuery += QString( " WHERE doctorId = %1" ).arg( m_uiId );
    }

    QSqlQuery  *poQuery = g_poDB->executeQTQuery( qsQuery );
    if( !m_uiId && poQuery ) m_uiId = poQuery->lastInsertId().toUInt();
    if( poQuery ) delete poQuery;
}

void cDBDoctor::remove() throw( cSevException )
{
    cTracer obTrace( "cDBDoctor::remove" );

    if( m_uiId )
    {
        QString  qsQuery;

        if( m_qsArchive.compare( "NEW" ) == 0 )
        {
            qsQuery = "DELETE FROM doctors ";
        }
        else
        {
            qsQuery = "UPDATE doctors SET active=0, archive=\"MOD\" ";
        }
        qsQuery += QString( " WHERE doctorId = %1" ).arg( m_uiId );

        QSqlQuery  *poQuery = g_poDB->executeQTQuery( qsQuery );
        if( poQuery ) delete poQuery;
    }
}

void cDBDoctor::createNew() throw()
{
    init();
}

unsigned int cDBDoctor::id() const throw()
{
    return m_uiId;
}

unsigned int cDBDoctor::licenceId() const throw()
{
    return m_uiLicenceId;
}

void cDBDoctor::setLicenceId( const unsigned int p_uiLicenceId ) throw()
{
    m_uiLicenceId = p_uiLicenceId;
}

unsigned int cDBDoctor::doctorTypeId() const throw()
{
    return m_uiDoctorTypeId;
}

void cDBDoctor::setDoctorTypeId( const unsigned int p_uiDoctorTypeId ) throw()
{
    m_uiDoctorTypeId = p_uiDoctorTypeId;
}

QString cDBDoctor::name() const throw()
{
    return m_qsName;
}

void cDBDoctor::setName( const QString &p_qsName ) throw()
{
    m_qsName = p_qsName;
    m_qsName = m_qsName.replace( QString("\""), QString("\\\"") );
}

QString cDBDoctor::licence() const throw()
{
    return m_qsLicence;
}

void cDBDoctor::setLicence( const QString &p_qsLicence ) throw()
{
    m_qsLicence = p_qsLicence;
    m_qsLicence = m_qsLicence.replace( QString("\""), QString("\\\"") );
}

QString cDBDoctor::data() const throw()
{
    return m_qsData;
}

void cDBDoctor::setData( const QString &p_qsData ) throw()
{
    m_qsData = p_qsData;
    m_qsData = m_qsData.replace( QString("\""), QString("\\\"") );
}

bool cDBDoctor::active() const throw()
{
    return m_bActive;
}

void cDBDoctor::setActive( const bool p_bActive ) throw()
{
    m_bActive = p_bActive;
}

QString cDBDoctor::archive() const throw()
{
    return m_qsArchive;
}

void cDBDoctor::setArchive( const QString &p_qsArchive ) throw()
{
    m_qsArchive = p_qsArchive;
}

