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
#include "dbdiscount.h"

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
                      const int p_nDiscountType,
                      const int p_nDiscount,
                      const bool p_bActive,
                      const QString &p_qsArchive ) throw()
{
    m_uiId              = p_uiId;
    m_uiLicenceId       = p_uiLicenceId;
    m_uiDoctorTypeId    = p_uiDoctorTypeId;
    m_qsName            = p_qsName;
    m_qsLicence         = p_qsLicence;
    m_qsData            = p_qsData;
    m_nDiscountType     = p_nDiscountType;
    m_nDiscount         = p_nDiscount;
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

    int p_nDiscountType = 0;
    int p_nDiscount     = 0;

    try
    {
        cDBDiscount obDBDiscount;

        obDBDiscount.loadDoctor( p_obRecord.value( inIdIdx ).toInt() );
        if( obDBDiscount.discountValue() > 0 )
        {
            p_nDiscountType = 1;
            p_nDiscount = obDBDiscount.discountValue();
        }
        else if( obDBDiscount.discountPercent() > 0 )
        {
            p_nDiscountType = 2;
            p_nDiscount = obDBDiscount.discountPercent();
        }
    }
    catch( cSevException &e )
    {
    }

    init( p_obRecord.value( inIdIdx ).toInt(),
          p_obRecord.value( inLicenceIdIdx ).toUInt(),
          p_obRecord.value( inDoctorTypeIdIdx ).toUInt(),
          p_obRecord.value( inNameIdx ).toString(),
          p_obRecord.value( inLicenceIdx ).toString(),
          p_obRecord.value( inDataIdx ).toString(),
          p_nDiscountType,
          p_nDiscount,
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
    qsQuery += QString( "modified = \"%1\", " ).arg( QDateTime::currentDateTime().toString( QString("yyyy-MM-dd hh:mm:ss") ) );
    qsQuery += QString( "active = %1, " ).arg( m_bActive );
    qsQuery += QString( "archive = \"%1\" " ).arg( m_qsArchive );
    if( m_uiId )
    {
        qsQuery += QString( " WHERE doctorId = %1" ).arg( m_uiId );
    }

    QSqlQuery  *poQuery = g_poDB->executeQTQuery( qsQuery );
    if( !m_uiId && poQuery ) m_uiId = poQuery->lastInsertId().toUInt();
    try
    {
        cDBDiscount obDBDiscount;

        obDBDiscount.loadDoctor( m_uiId );
        if( m_nDiscountType == 1 )
        {
            obDBDiscount.setDiscountValue( m_nDiscount );
            obDBDiscount.setDiscountPercent( 0 );
        }
        else if( m_nDiscountType == 2 )
        {
            obDBDiscount.setDiscountValue( 0 );
            obDBDiscount.setDiscountPercent( m_nDiscount );
        }
        obDBDiscount.setName( m_qsName );
        obDBDiscount.save();
    }
    catch( cSevException &e )
    {
        if( QString(e.what()).compare("Discount id not found") != 0 )
        {
            g_obLogger(e.severity()) << e.what() << EOM;
        }
        else
        {
            cDBDiscount obDBDiscount;

            obDBDiscount.createNew();
            obDBDiscount.setLicenceId( g_poPrefs->getLicenceId() );
            obDBDiscount.setDoctorId( m_uiId);
            if( m_nDiscountType == 1 )
            {
                obDBDiscount.setDiscountValue( m_nDiscount );
            }
            else if( m_nDiscountType == 2 )
            {
                obDBDiscount.setDiscountPercent( m_nDiscount );
            }
            obDBDiscount.setName( m_qsName );
            obDBDiscount.save();
        }
    }
    if( poQuery ) delete poQuery;

    if( m_uiId > 0 && m_uiLicenceId != 1 )
        g_obDBMirror.updateSynchronizationLevel( DB_DOCTOR );
    if( m_uiId > 0 && m_uiLicenceId == 0 )
        g_obDBMirror.updateGlobalSyncLevel( DB_DOCTOR );
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

int cDBDoctor::discountType() const throw()
{
    return m_nDiscountType;
}

void cDBDoctor::setDiscountType( const int p_nDiscountType ) throw()
{
    m_nDiscountType = p_nDiscountType;
}

int cDBDoctor::discount() const throw()
{
    return m_nDiscount;
}

void cDBDoctor::setDiscount( const int p_nDiscount ) throw()
{
    m_nDiscount = p_nDiscount;
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

