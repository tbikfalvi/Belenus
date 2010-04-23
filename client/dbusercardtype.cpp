#include "belenus.h"
#include "dbusercardtype.h"

cDBUserCardType::cDBUserCardType()
{
    init();
}

cDBUserCardType::~cDBUserCardType()
{
}

void cDBUserCardType::init( const unsigned int p_uiId, const string &p_stName,
                            const int p_inPrice, const unsigned int p_uiUnits,
                            const unsigned int p_uiValidDateFromYear,
                            const unsigned int p_uiValidDateFromMonth,
                            const unsigned int p_uiValidDateFromDay,
                            const unsigned int p_uiValidDateToYear,
                            const unsigned int p_uiValidDateToMonth,
                            const unsigned int p_uiValidDateToDay,
                            const unsigned int p_uiValidDays,
                            const unsigned int p_uiUnitTime ) throw()
{
    m_uiId = p_uiId;
    m_stName = p_stName;
    m_inPrice = p_inPrice;
    m_uiUnits = p_uiUnits;
    m_uiValidDateFromYear = p_uiValidDateFromYear;
    m_uiValidDateFromMonth = p_uiValidDateFromMonth;
    m_uiValidDateFromDay = p_uiValidDateFromDay;
    m_uiValidDateToYear = p_uiValidDateToYear;
    m_uiValidDateToMonth = p_uiValidDateToMonth;
    m_uiValidDateToDay = p_uiValidDateToDay;
    m_uiValidDays = p_uiValidDays;
    m_uiUnitTime = p_uiUnitTime;
}

void cDBUserCardType::init( const QSqlRecord &p_obRecord ) throw()
{
    int inIdIdx             = p_obRecord.indexOf( "usercardtypeid" );
    int inNameIdx           = p_obRecord.indexOf( "name" );
    int inPriceIdx          = p_obRecord.indexOf( "price" );
    int inUnitsIdx          = p_obRecord.indexOf( "units" );
    int inValidDateFromIdx  = p_obRecord.indexOf( "validdatefrom" );
    int inValidDateToIdx    = p_obRecord.indexOf( "validdateto" );
    int inValidDaysIdx      = p_obRecord.indexOf( "validdays" );
    int inUnitTimeIdx       = p_obRecord.indexOf( "unittime" );

    int inValidDateFromYear;
    int inValidDateFromMonth;
    int inValidDateFromDay;
    int inValidDateToYear;
    int inValidDateToMonth;
    int inValidDateToDay;

    p_obRecord.value( inValidDateFromIdx ).toDate().getDate( &inValidDateFromYear,
                                                             &inValidDateFromMonth,
                                                             &inValidDateFromDay );
    p_obRecord.value( inValidDateToIdx ).toDate().getDate( &inValidDateToYear,
                                                           &inValidDateToMonth,
                                                           &inValidDateToDay );

    init( p_obRecord.value( inIdIdx ).toUInt(),
          p_obRecord.value( inNameIdx ).toString().toStdString(),
          p_obRecord.value( inPriceIdx ).toUInt(),
          p_obRecord.value( inUnitsIdx ).toUInt(),
          (unsigned int)inValidDateFromYear,
          (unsigned int)inValidDateFromMonth,
          (unsigned int)inValidDateFromDay,
          (unsigned int)inValidDateToYear,
          (unsigned int)inValidDateToMonth,
          (unsigned int)inValidDateToDay,
          p_obRecord.value( inValidDaysIdx ).toUInt(),
          p_obRecord.value( inUnitTimeIdx ).toUInt() );
}

void cDBUserCardType::load( const unsigned int p_uiId ) throw( cSevException )
{
    cTracer obTrace( "cDBUserCardType::load", QString( "id: %1" ).arg( p_uiId ).toStdString() );

    QSqlQuery *poQuery = g_poDB->executeQTQuery( QString( "SELECT * FROM usercardtypes WHERE usercardtypeid = %1" ).arg( p_uiId ) );

    if( poQuery->size() != 1 )
        throw cSevException( cSeverity::ERROR, "UserCardType id not found" );

    poQuery->first();
    init( poQuery->record() );
}

void cDBUserCardType::save() const throw( cSevException )
{
    cTracer obTrace( "cDBUserCardType::save" );
    stringstream  ssQuery;

    if( m_uiId )
    {
        ssQuery << "UPDATE usercardtypes ";
    }
    else
    {
        ssQuery << "INSERT INTO usercardtypes ";
    }
    ssQuery << "SET name = \"" << m_stName << "\", ";
    ssQuery << "price = \"" << m_inPrice << "\", ";
    ssQuery << "units = \"" << m_uiUnits << "\", ";
    ssQuery << "validdatefrom = \"" << m_uiValidDateFromYear << "-";
    ssQuery << m_uiValidDateFromMonth << "-";
    ssQuery << m_uiValidDateFromDay << "\", ";
    ssQuery << "validdateto = \"" << m_uiValidDateToYear << "-";
    ssQuery << m_uiValidDateToMonth << "-";
    ssQuery << m_uiValidDateToDay << "\", ";
    ssQuery << "validdays = \"" << m_uiValidDays << "\", ";
    ssQuery << "unittime = \"" << m_uiUnitTime << "\" ";
    if( m_uiId )
    {
        ssQuery << "WHERE usercardtypeid = " << m_uiId;
    }

    g_poDB->executeQuery( ssQuery.str(), true );
}

void cDBUserCardType::createNew() throw()
{
    init( 0 );
}

unsigned int cDBUserCardType::id() throw()
{
    return m_uiId;
}

string cDBUserCardType::name() throw()
{
    return m_stName;
}

void cDBUserCardType::setName( const string &p_stName ) throw()
{
    m_stName = p_stName;
}

int cDBUserCardType::price() throw()
{
    return m_inPrice;
}

void cDBUserCardType::setPrice( const int p_inPrice ) throw()
{
    m_inPrice = p_inPrice;
}

unsigned int cDBUserCardType::units() throw()
{
    return m_uiUnits;
}

void cDBUserCardType::setUnits( const unsigned int puiUnits ) throw()
{
    m_uiUnits = puiUnits;
}

string cDBUserCardType::validDateFrom() throw()
{
    stringstream    ssTemp;

    ssTemp << m_uiValidDateFromYear << "-" << m_uiValidDateFromMonth << "-" << m_uiValidDateFromDay;

    return ssTemp.str();
}

unsigned int cDBUserCardType::validDateFromYear() throw()
{
    return m_uiValidDateFromYear;
}

void cDBUserCardType::setValidDateFromYear( const unsigned int p_uiValidDateFromYear ) throw()
{
    m_uiValidDateFromYear = p_uiValidDateFromYear;
}

unsigned int cDBUserCardType::validDateFromMonth() throw()
{
    return m_uiValidDateFromMonth;
}

void cDBUserCardType::setValidDateFromMonth( const unsigned int p_uiValidDateFromMonth )  throw()
{
    m_uiValidDateFromMonth = p_uiValidDateFromMonth;
}

unsigned int cDBUserCardType::validDateFromDay()  throw()
{
    return m_uiValidDateFromDay;
}

void cDBUserCardType::setValidDateFromDay( const unsigned int p_uiValidDateFromDay )  throw()
{
    m_uiValidDateFromDay = p_uiValidDateFromDay;
}

string cDBUserCardType::validDateTo()   throw()
{
    stringstream    ssTemp;

    ssTemp << m_uiValidDateFromYear << "-" << m_uiValidDateFromMonth << "-" << m_uiValidDateFromDay;

    return ssTemp.str();
}

unsigned int cDBUserCardType::validDateToYear() throw()
{
    return m_uiValidDateToYear;
}

void cDBUserCardType::setValidDateToYear( const unsigned int p_uiValidDateToYear ) throw()
{
    m_uiValidDateToYear = p_uiValidDateToYear;
}

unsigned int cDBUserCardType::validDateToMonth() throw()
{
    return m_uiValidDateToMonth;
}

void cDBUserCardType::setValidDateToMonth( const unsigned int p_uiValidDateToMonth ) throw()
{
    m_uiValidDateToMonth = p_uiValidDateToMonth;
}

unsigned int cDBUserCardType::validDateToDay() throw()
{
    return m_uiValidDateToDay;
}

void cDBUserCardType::setValidDateToDay( const unsigned int p_uiValidDateToDay ) throw()
{
    m_uiValidDateToDay = p_uiValidDateToDay;
}

unsigned int cDBUserCardType::validDays() throw()
{
    return m_uiValidDays;
}

void cDBUserCardType::setValidDays( const unsigned int p_uiValidDays ) throw()
{
    m_uiValidDays = p_uiValidDays;
}

unsigned int cDBUserCardType::unitTime() throw()
{
    return m_uiUnitTime;
}

void cDBUserCardType::setUnitTime( const unsigned int p_uiUnitTime ) throw()
{
    m_uiUnitTime = p_uiUnitTime;
}

