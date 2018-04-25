//====================================================================================
//
// Belenus Server alkalmazas (c) Pagony Multimedia Studio Bt - 2010
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
#include "dbpatientcard.h"

cDBPatientCardType::cDBPatientCardType()
{
    init();
}

cDBPatientCardType::~cDBPatientCardType()
{
}

void cDBPatientCardType::init( const unsigned int p_uiId,
                               const unsigned int p_uiLicenceId,
                               const QString &p_qsName,
                               const float p_fPrice,
                               const int p_nVatpercent,
                               const int p_nUnits,
                               const QString &p_qsValidDateFrom,
                               const QString &p_qsValidDateTo,
                               const int p_nValidDays,
                               const int p_nUnitTime,
                               const QString &p_qsModified,
                               const bool p_bActive,
                               const QString &p_qsArchive ) throw()
{
    m_uiId              = p_uiId;
    m_uiLicenceId       = p_uiLicenceId;
    m_qsName            = p_qsName;
    m_fPrice            = p_fPrice;
    m_nVatpercent       = p_nVatpercent;
    m_nUnits            = p_nUnits;
    m_qsValidDateFrom   = p_qsValidDateFrom;
    m_qsValidDateTo     = p_qsValidDateTo;
    m_nValidDays        = p_nValidDays;
    m_nUnitTime         = p_nUnitTime;
    m_qsModified        = p_qsModified;
    m_bActive           = p_bActive;
    m_qsArchive         = p_qsArchive;
}

void cDBPatientCardType::init( const QSqlRecord &p_obRecord ) throw()
{
    int inIdIdx             = p_obRecord.indexOf( "patientCardTypeId" );
    int inLicenceIdIdx      = p_obRecord.indexOf( "licenceId" );
    int inNameIdx           = p_obRecord.indexOf( "name" );
    int inPriceIdx          = p_obRecord.indexOf( "price" );
    int inVatpercentIdx     = p_obRecord.indexOf( "vatpercent" );
    int inUnitsIdx          = p_obRecord.indexOf( "units" );
    int inValidDateFromIdx  = p_obRecord.indexOf( "validDateFrom" );
    int inValidDateToIdx    = p_obRecord.indexOf( "validDateTo" );
    int inValidDaysIdx      = p_obRecord.indexOf( "validDays" );
    int inUnitTimeIdx       = p_obRecord.indexOf( "unitTime" );
    int inModifiedIdx       = p_obRecord.indexOf( "modified" );
    int inActiveIdx         = p_obRecord.indexOf( "active" );
    int inArchiveIdx        = p_obRecord.indexOf( "archive" );

    init( p_obRecord.value( inIdIdx ).toInt(),
          p_obRecord.value( inLicenceIdIdx ).toInt(),
          p_obRecord.value( inNameIdx ).toString(),
          p_obRecord.value( inPriceIdx ).toFloat(),
          p_obRecord.value( inVatpercentIdx ).toInt(),
          p_obRecord.value( inUnitsIdx ).toInt(),
          p_obRecord.value( inValidDateFromIdx ).toString(),
          p_obRecord.value( inValidDateToIdx ).toString(),
          p_obRecord.value( inValidDaysIdx ).toInt(),
          p_obRecord.value( inUnitTimeIdx ).toInt(),
          p_obRecord.value( inModifiedIdx ).toString(),
          p_obRecord.value( inActiveIdx ).toBool(),
          p_obRecord.value( inArchiveIdx ).toString() );
}

void cDBPatientCardType::load( const unsigned int p_uiId ) throw( cSevException )
{
    cTracer obTrace( "cDBPatientCardType::load", QString( "id: %1" ).arg( p_uiId ) );

    QSqlQuery *poQuery = g_poDB->executeQTQuery( QString( "SELECT * FROM patientCardTypes WHERE patientCardTypeId = %1" ).arg( p_uiId ) );

    if( poQuery->size() != 1 )
        throw cSevException( cSeverity::ERROR, "Patientcard type id not found" );

    poQuery->first();
    init( poQuery->record() );
}

void cDBPatientCardType::load( const QString &p_qsName ) throw( cSevException )
{
    cTracer obTrace( "cDBPatientCardType::load", QString("name: \"%1\"").arg(p_qsName) );

    QSqlQuery *poQuery = g_poDB->executeQTQuery( "SELECT * FROM patientCardTypes WHERE name = \"" + p_qsName + "\"" );

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
        m_qsArchive = "MOD";
    }
    else
    {
        qsQuery = "INSERT INTO";
        m_qsArchive = "NEW";
    }
    qsQuery += " patientCardTypes SET ";
    qsQuery += QString( "licenceId = \"%1\", " ).arg( m_uiLicenceId );
    qsQuery += QString( "name = \"%1\", " ).arg( m_qsName );
    qsQuery += QString( "price = \"%1\", " ).arg( m_fPrice );
    qsQuery += QString( "vatpercent = \"%1\", " ).arg( m_nVatpercent );
    qsQuery += QString( "units = \"%1\", " ).arg( m_nUnits );
    qsQuery += QString( "validDateFrom = \"%1\", " ).arg( m_qsValidDateFrom );
    qsQuery += QString( "validDateTo = \"%1\", " ).arg( m_qsValidDateTo );
    qsQuery += QString( "validDays = \"%1\", " ).arg( m_nValidDays );
    qsQuery += QString( "unitTime = \"%1\", " ).arg( m_nUnitTime );
    qsQuery += QString( "modified = \"%1\", " ).arg( QDateTime::currentDateTime().toString( QString("yyyy-MM-dd hh:mm:ss") ) );
    qsQuery += QString( "active = %1, " ).arg( m_bActive );
    qsQuery += QString( "archive = \"%1\" " ).arg( m_qsArchive );
    if( m_uiId )
    {
        qsQuery += QString( " WHERE patientCardTypeId = %1" ).arg( m_uiId );
    }

    QSqlQuery  *poQuery = g_poDB->executeQTQuery( qsQuery );
    if( !m_uiId && poQuery ) m_uiId = poQuery->lastInsertId().toUInt();
    if( poQuery ) delete poQuery;
/*
    if( m_uiId > 0 && m_uiLicenceId != 1 )
        g_obDBMirror.updateSynchronizationLevel( DB_PATIENTCARD_TYPE );
    if( m_uiId > 0 && m_uiLicenceId == 0 )
        g_obDBMirror.updateGlobalSyncLevel( DB_PATIENTCARD_TYPE );
*/
}

void cDBPatientCardType::remove() throw( cSevException )
{
    cTracer obTrace( "cDBPatientCardType::remove" );

    if( m_uiId )
    {
        QString  qsQuery;

        if( m_qsArchive == "NEW" )
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

QString cDBPatientCardType::name() const throw()
{
    return m_qsName;
}

void cDBPatientCardType::setName( const QString &p_qsName ) throw()
{
    m_qsName = p_qsName;
    m_qsName = m_qsName.replace( QString("\""), QString("\\\"") );
}

float cDBPatientCardType::price() const throw()
{
    return m_fPrice;
}

void cDBPatientCardType::setPrice( const float p_fPrice ) throw()
{
    m_fPrice = p_fPrice;
}

int cDBPatientCardType::vatpercent() const throw()
{
    return m_nVatpercent;
}

void cDBPatientCardType::setVatpercent( const int p_nVatpercent ) throw()
{
    m_nVatpercent = p_nVatpercent;
}

int cDBPatientCardType::units() const throw()
{
    return m_nUnits;
}

void cDBPatientCardType::setUnits( const int p_nUnits ) throw()
{
    m_nUnits = p_nUnits;
}

QString cDBPatientCardType::validDateFrom() const throw()
{
    return m_qsValidDateFrom;
}

void cDBPatientCardType::setValidDateFrom( const QString &p_qsVDFrom ) throw()
{
    m_qsValidDateFrom = p_qsVDFrom;
}

QString cDBPatientCardType::validDateTo() const throw()
{
    return m_qsValidDateTo;
}

void cDBPatientCardType::setValidDateTo( const QString &p_qsVDTo ) throw()
{
    m_qsValidDateTo = p_qsVDTo;
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

QString cDBPatientCardType::modified() const throw()
{
    return m_qsModified;
}

bool cDBPatientCardType::active() const throw()
{
    return m_bActive;
}

void cDBPatientCardType::setActive( const bool p_bActive ) throw()
{
    m_bActive = p_bActive;
}

QString cDBPatientCardType::archive() const throw()
{
    return m_qsArchive;
}

void cDBPatientCardType::setArchive( const QString &p_qsArchive ) throw()
{
    m_qsArchive = p_qsArchive;
}

void cDBPatientCardType::updatePatientCardUnits( int p_nUnitTime ) throw()
{
    QSqlQuery *poQuery = g_poDB->executeQTQuery( QString( "SELECT patientCardId FROM patientcards WHERE patientCardTypeId = %1" ).arg( m_uiId ) );

    while( poQuery->next() )
    {
        try
        {
            cDBPatientCard  obDBPatientCard;

            obDBPatientCard.load( poQuery->value( 0 ).toUInt() );
            obDBPatientCard.synchronizeUnitTime( p_nUnitTime );
        }
        catch( cSevException &e )
        {
            g_obLogger(e.severity()) << e.what() << EOM;
            g_obGen.showTrayError( e.what() );
        }
    }
}
