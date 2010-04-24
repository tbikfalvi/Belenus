#include "belenus.h"
#include "dbusercard.h"

cDBUserCard::cDBUserCard()
{
    init();
}

cDBUserCard::~cDBUserCard()
{
}

void cDBUserCard::init( const unsigned int   p_uiId,
                        const unsigned int   p_uiUserCardTypeId,
                        const string         p_stBarCode,
                        const string         p_stComment,
                        const unsigned int   p_uiUnits,
                        const unsigned int   p_uiValidDateYear,
                        const unsigned int   p_uiValidDateMonth,
                        const unsigned int   p_uiValidDateDay,
                        const int            p_inPinCode ) throw()
{
    m_uiId              = p_uiId;
    m_uiUserCardTypeId  = p_uiUserCardTypeId;
    m_stBarCode         = p_stBarCode;
    m_stComment         = p_stComment;
    m_uiUnits           = p_uiUnits;
    m_uiValidDateYear   = p_uiValidDateYear;
    m_uiValidDateMonth  = p_uiValidDateMonth;
    m_uiValidDateDay    = p_uiValidDateDay;
    m_inPinCode         = p_inPinCode;
}

void cDBUserCard::init( const QSqlRecord &p_obRecord ) throw()
{
    int inIdIdx             = p_obRecord.indexOf( "usercardid" );
    int inUserCardTypeIdIdx = p_obRecord.indexOf( "usercardtypeid" );
    int inBarCodeIdx        = p_obRecord.indexOf( "barcode" );
    int inCommentsIdx       = p_obRecord.indexOf( "comment" );
    int inUnitsIdx          = p_obRecord.indexOf( "units" );
    int inValidDateIdx      = p_obRecord.indexOf( "validdate" );
    int inPinCodeIdx        = p_obRecord.indexOf( "pincode" );

    int inValidDateYear;
    int inValidDateMonth;
    int inValidDateDay;

    p_obRecord.value( inValidDateIdx ).toDate().getDate( &inValidDateYear,
                                                         &inValidDateMonth,
                                                         &inValidDateDay );

    init( p_obRecord.value( inIdIdx ).toUInt(),
          p_obRecord.value( inUserCardTypeIdIdx ).toUInt(),
          p_obRecord.value( inBarCodeIdx ).toString().toStdString(),
          p_obRecord.value( inCommentsIdx ).toString().toStdString(),
          p_obRecord.value( inUnitsIdx ).toUInt(),
          (unsigned int)inValidDateYear,
          (unsigned int)inValidDateMonth,
          (unsigned int)inValidDateDay,
          p_obRecord.value( inPinCodeIdx ).toInt() );
}

void cDBUserCard::load( const unsigned int p_uiId ) throw( cSevException )
{
    cTracer obTrace( "cDBUserCard::load", QString( "id: %1" ).arg( p_uiId ).toStdString() );

    QSqlQuery *poQuery = g_poDB->executeQTQuery( QString( "SELECT * FROM usercards WHERE usercardid = %1" ).arg( p_uiId ) );

    if( poQuery->size() != 1 )
        throw cSevException( cSeverity::ERROR, "UserCard id not found" );

    poQuery->first();
    init( poQuery->record() );
}

void cDBUserCard::save() throw( cSevException )
{
    cTracer obTrace( "cDBUserCard::save" );
    QString  qsQuery;

    if( m_uiId )
    {
        qsQuery = "UPDATE";
    }
    else
    {
        qsQuery = "INSERT INTO";
    }
    qsQuery += " patientCards SET ";
    qsQuery += QString( "patientCardTypeId = %1, " ).arg( m_uiUserCardTypeId );
    qsQuery += QString( "barcode = \"%1\", " ).arg( QString::fromStdString( m_stBarCode ) );
    qsQuery += QString( "comment = \"%1\", " ).arg( QString::fromStdString( m_stComment ) );
    qsQuery += QString( "units = %1, " ).arg( m_uiUnits );
    qsQuery += QString( "validDate = \"%1-%2-%3\", " ).arg( m_uiValidDateYear ).arg( m_uiValidDateMonth ).arg( m_uiValidDateDay );
    qsQuery += QString( "pincode = %1" ).arg( m_inPinCode );
    if( m_uiId )
    {
        qsQuery += QString( " WHERE patientCardId = %1" ).arg( m_uiId );
    }

    QSqlQuery  *poQuery = g_poDB->executeQTQuery( qsQuery );
    if( !m_uiId && poQuery ) m_uiId = poQuery->lastInsertId().toUInt();
    if( poQuery ) delete poQuery;
}

void cDBUserCard::createNew() throw()
{
    init( 0 );
}

unsigned int cDBUserCard::id() throw()
{
    return m_uiId;
}

unsigned int cDBUserCard::userCardTypeId() throw()
{
    return m_uiUserCardTypeId;
}

void cDBUserCard::setUserCardTypeId( const unsigned int p_uiUserCardTypeId ) throw()
{
    m_uiUserCardTypeId = p_uiUserCardTypeId;
}

string cDBUserCard::barCode() throw()
{
    return m_stBarCode;
}

void cDBUserCard::setBarCode( const string p_stBarCode ) throw()
{
    m_stBarCode = p_stBarCode;
}

string cDBUserCard::comment() throw()
{
    return m_stComment;
}

void cDBUserCard::setComment( const string p_stComment ) throw()
{
    m_stComment = p_stComment;
}

unsigned int cDBUserCard::units() throw()
{
    return m_uiUnits;
}

void cDBUserCard::setUnits( const unsigned int p_uiUnits ) throw()
{
    m_uiUnits = p_uiUnits;
}

string cDBUserCard::validDate() throw()
{
    stringstream    ssTemp;

    ssTemp << m_uiValidDateYear << "-" << m_uiValidDateMonth << "-" << m_uiValidDateDay;

    return ssTemp.str();
}

unsigned int cDBUserCard::validDateYear() throw()
{
    return m_uiValidDateYear;
}

void cDBUserCard::setValidDateYear( const unsigned int p_uiValidDateYear ) throw()
{
    m_uiValidDateYear = p_uiValidDateYear;
}

unsigned int cDBUserCard::validDateMonth() throw()
{
    return m_uiValidDateMonth;
}

void cDBUserCard::setValidDateMonth( const unsigned int p_uiValidDateMonth ) throw()
{
    m_uiValidDateMonth = p_uiValidDateMonth;
}

unsigned int cDBUserCard::validDateDay() throw()
{
    return m_uiValidDateDay;
}

void cDBUserCard::setValidDateDay( const unsigned int p_uiValidDateDay ) throw()
{
    m_uiValidDateDay = p_uiValidDateDay;
}

int cDBUserCard::pinCode() throw()
{
    return m_inPinCode;
}

void cDBUserCard::setPinCode( const int p_inPinCode ) throw()
{
    m_inPinCode = p_inPinCode;
}

