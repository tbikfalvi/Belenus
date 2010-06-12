//====================================================================================
//
// Belenus Server alkalmazas © Pagony Multimedia Studio Bt - 2010
//
//====================================================================================
//
// Filename    : dbpatientcardtype.cpp
// AppVersion  : 1.0
// FileVersion : 1.0
// Author      : Bikfalvi Tamas
//
//====================================================================================
//
//====================================================================================

#include "belenus.h"
#include "dbpatientcardtype.h"

cDBPatientCardType::cDBPatientCardType()
{
    init();
}

cDBPatientCardType::~cDBPatientCardType()
{
}

void cDBPatientCardType::init( const unsigned int p_uiId,
                             const unsigned int p_uiLicenceId,
                             const string &p_stName,
                             const float p_fPrice,
                             const int p_nUnits,
                             const string &p_stValidDateFrom,
                             const string &p_stValidDateTo,
                             const int p_nValidDays,
                             const int p_nUnitTime,
                             const bool p_bActive,
                             const string &p_stArchive ) throw()
{
    m_uiId              = p_uiId;
    m_uiLicenceId       = p_uiLicenceId;
    m_stName            = p_stName;
    m_fPrice            = p_fPrice;
    m_nUnits            = p_nUnits;
    m_stValidDateFrom   = p_stValidDateFrom;
    m_stValidDateTo     = p_stValidDateTo;
    m_nValidDays        = p_nValidDays;
    m_nUnitTime         = p_nUnitTime;
    m_bActive           = p_bActive;
    m_stArchive         = p_stArchive;
}

void cDBPatientCardType::init( const QSqlRecord &p_obRecord ) throw()
{
    int inIdIdx             = p_obRecord.indexOf( "patientCardTypeId" );
    int inLicenceIdIdx      = p_obRecord.indexOf( "licenceId" );
    int inNameIdx           = p_obRecord.indexOf( "name" );
    int inPriceIdx          = p_obRecord.indexOf( "price" );
    int inUnitsIdx          = p_obRecord.indexOf( "units" );
    int inValidDateFromIdx  = p_obRecord.indexOf( "validDateFrom" );
    int inValidDateToIdx    = p_obRecord.indexOf( "validDateTo" );
    int inValidDaysIdx      = p_obRecord.indexOf( "validDays" );
    int inUnitTimeIdx       = p_obRecord.indexOf( "unitTime" );
    int inActiveIdx         = p_obRecord.indexOf( "active" );
    int inArchiveIdx        = p_obRecord.indexOf( "archive" );

    init( p_obRecord.value( inIdIdx ).toInt(),
          p_obRecord.value( inLicenceIdIdx ).toInt(),
          p_obRecord.value( inNameIdx ).toString().toStdString(),
          p_obRecord.value( inPriceIdx ).toFloat(),
          p_obRecord.value( inUnitsIdx ).toInt(),
          p_obRecord.value( inValidDateFromIdx ).toString().toStdString(),
          p_obRecord.value( inValidDateToIdx ).toString().toStdString(),
          p_obRecord.value( inValidDaysIdx ).toInt(),
          p_obRecord.value( inUnitTimeIdx ).toInt(),
          p_obRecord.value( inActiveIdx ).toBool(),
          p_obRecord.value( inArchiveIdx ).toString().toStdString() );
}

void cDBPatientCardType::load( const unsigned int p_uiId ) throw( cSevException )
{
    cTracer obTrace( "cDBPatientCardType::load", QString( "id: %1" ).arg( p_uiId ).toStdString() );

    QSqlQuery *poQuery = g_poDB->executeQTQuery( QString( "SELECT * FROM patientCardTypes WHERE patientCardTypeId = %1" ).arg( p_uiId ) );

    if( poQuery->size() != 1 )
        throw cSevException( cSeverity::ERROR, "Patientcard type id not found" );

    poQuery->first();
    init( poQuery->record() );
}

void cDBPatientCardType::load( const string &p_stName ) throw( cSevException )
{
    cTracer obTrace( "cDBPatientCardType::load", "name: \""  + p_stName + "\"" );

    QSqlQuery *poQuery = g_poDB->executeQTQuery( "SELECT * FROM patientCardTypes WHERE name = \"" + QString::fromStdString( p_stName ) + "\"" );

    if( poQuery->size() != 1 )
        throw cSevException( cSeverity::ERROR, "Patiencard type name not found" );

    poQuery->first();
    init( poQuery->record() );
}

void cDBPatientCardType::save() throw( cSevException )
{
    cTracer obTrace( "cDBPatientCardType::save" );
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
    qsQuery += " patientCardTypes SET ";
    qsQuery += QString( "licenceId = \"%1\", " ).arg( m_uiLicenceId );
    qsQuery += QString( "name = \"%1\", " ).arg( QString::fromStdString( m_stName ) );
    qsQuery += QString( "price = \"%1\", " ).arg( m_fPrice );
    qsQuery += QString( "units = \"%1\", " ).arg( m_nUnits );
    qsQuery += QString( "validDateFrom = \"%1\", " ).arg( QString::fromStdString( m_stValidDateFrom ) );
    qsQuery += QString( "validDateTo = \"%1\", " ).arg( QString::fromStdString( m_stValidDateTo ) );
    qsQuery += QString( "validDays = \"%1\", " ).arg( m_nValidDays );
    qsQuery += QString( "unitTime = \"%1\", " ).arg( m_nUnitTime );
    qsQuery += QString( "active = %1, " ).arg( m_bActive );
    qsQuery += QString( "archive = \"%1\" " ).arg( QString::fromStdString( m_stArchive ) );
    if( m_uiId )
    {
        qsQuery += QString( " WHERE patientCardTypeId = %1" ).arg( m_uiId );
    }

    QSqlQuery  *poQuery = g_poDB->executeQTQuery( qsQuery );
    if( !m_uiId && poQuery ) m_uiId = poQuery->lastInsertId().toUInt();
    if( poQuery ) delete poQuery;
}

void cDBPatientCardType::remove() throw( cSevException )
{
    cTracer obTrace( "cDBPatientCardType::remove" );

    if( m_uiId )
    {
        QString  qsQuery;

        if( m_stArchive.compare( "NEW" ) == 0 )
        {
            qsQuery = "DELETE FROM patientCardTypes ";
        }
        else
        {
            qsQuery = "UPDATE patientCardTypes SET active=0, archive=\"MOD\" ";
        }
        qsQuery += QString( " WHERE patientCardTypeId = %1" ).arg( m_uiId );

        QSqlQuery  *poQuery = g_poDB->executeQTQuery( qsQuery );
        if( poQuery ) delete poQuery;
    }
}

void cDBPatientCardType::createNew() throw()
{
    init();
}

unsigned int cDBPatientCardType::id() const throw()
{
    return m_uiId;
}

unsigned int cDBPatientCardType::licenceId() const throw()
{
    return m_uiLicenceId;
}

void cDBPatientCardType::setLicenceId( const unsigned int p_uiLicenceId ) throw()
{
    m_uiLicenceId = p_uiLicenceId;
}

string cDBPatientCardType::name() const throw()
{
    return m_stName;
}

void cDBPatientCardType::setName( const string &p_stName ) throw()
{
    m_stName = p_stName;
}

float cDBPatientCardType::price() const throw()
{
    return m_fPrice;
}

void cDBPatientCardType::setPrice( const float p_fPrice ) throw()
{
    m_fPrice = p_fPrice;
}

int cDBPatientCardType::units() const throw()
{
    return m_nUnits;
}

void cDBPatientCardType::setUnits( const int p_nUnits ) throw()
{
    m_nUnits = p_nUnits;
}

string cDBPatientCardType::validDateFrom() const throw()
{
    return m_stValidDateFrom;
}

void cDBPatientCardType::setValidDateFrom( const string &p_stVDFrom ) throw()
{
    m_stValidDateFrom = p_stVDFrom;
}

string cDBPatientCardType::validDateTo() const throw()
{
    return m_stValidDateTo;
}

void cDBPatientCardType::setValidDateTo( const string &p_stVDTo ) throw()
{
    m_stValidDateTo = p_stVDTo;
}

int cDBPatientCardType::validDays() const throw()
{
    return m_nValidDays;
}

void cDBPatientCardType::setValidDays( const int p_nValidDays ) throw()
{
    m_nValidDays = p_nValidDays;
}

int cDBPatientCardType::unitTime() const throw()
{
    return m_nUnitTime;
}

void cDBPatientCardType::setUnitTime( const int p_nUnitTime ) throw()
{
    m_nUnitTime = p_nUnitTime;
}

bool cDBPatientCardType::active() const throw()
{
    return m_bActive;
}

void cDBPatientCardType::setActive( const bool p_bActive ) throw()
{
    m_bActive = p_bActive;
}

string cDBPatientCardType::archive() const throw()
{
    return m_stArchive;
}

void cDBPatientCardType::setArchive( const string &p_stArchive ) throw()
{
    m_stArchive = p_stArchive;
}

