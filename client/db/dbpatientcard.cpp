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
                           const unsigned int p_uiPatientCardTypeId,
                           const unsigned int p_uiPatientId,
                           const string p_stBarcode,
                           const string p_stComment,
                           const int p_nUnits,
                           const string p_stTimeLeft,
                           const string p_stValidDate,
                           const string p_stPincode,
                           const bool p_bActive,
                           const string &p_stArchive ) throw()
{
    m_uiId                  = p_uiId;
    m_uiLicenceId           = p_uiLicenceId;
    m_uiPatientCardTypeId   = p_uiPatientCardTypeId;
    m_uiPatientId           = p_uiPatientId;
    m_stBarcode             = p_stBarcode;
    m_stComment             = p_stComment;
    m_nUnits                = p_nUnits;
    m_stTimeLeft            = p_stTimeLeft;
    m_stValidDate           = p_stValidDate;
    m_stPincode             = p_stPincode;
    m_bActive               = p_bActive;
    m_stArchive             = p_stArchive;
}

void cDBPatientCard::init( const QSqlRecord &p_obRecord ) throw()
{
    int inIdIdx                 = p_obRecord.indexOf( "patientCardId" );
    int inLicenceIdIdx          = p_obRecord.indexOf( "licenceId" );
    int inPatientCardTypeIdIdx  = p_obRecord.indexOf( "patientCardTypeId" );
    int inPatientIdIdx          = p_obRecord.indexOf( "patientId" );
    int inBarcodeIdx            = p_obRecord.indexOf( "barcode" );
    int inCommentIdx            = p_obRecord.indexOf( "comment" );
    int inUnitsIdx              = p_obRecord.indexOf( "units" );
    int inTimeLeftIdx           = p_obRecord.indexOf( "timeLeft" );
    int inValidDateIdx          = p_obRecord.indexOf( "validDate" );
    int inPincodeIdx            = p_obRecord.indexOf( "pincode" );
    int inActiveIdx             = p_obRecord.indexOf( "active" );
    int inArchiveIdx            = p_obRecord.indexOf( "archive" );

    init( p_obRecord.value( inIdIdx ).toInt(),
          p_obRecord.value( inLicenceIdIdx ).toInt(),
          p_obRecord.value( inPatientCardTypeIdIdx ).toInt(),
          p_obRecord.value( inPatientIdIdx ).toInt(),
          p_obRecord.value( inBarcodeIdx ).toString().toStdString(),
          p_obRecord.value( inCommentIdx ).toString().toStdString(),
          p_obRecord.value( inUnitsIdx ).toInt(),
          p_obRecord.value( inTimeLeftIdx ).toString().toStdString(),
          p_obRecord.value( inValidDateIdx ).toString().toStdString(),
          p_obRecord.value( inPincodeIdx ).toString().toStdString(),
          p_obRecord.value( inActiveIdx ).toBool(),
          p_obRecord.value( inArchiveIdx ).toString().toStdString() );
}

void cDBPatientCard::load( const unsigned int p_uiId ) throw( cSevException )
{
    cTracer obTrace( "cDBPatientCard::load", QString( "id: %1" ).arg( p_uiId ).toStdString() );

    QSqlQuery *poQuery = g_poDB->executeQTQuery( QString( "SELECT * FROM patientCards WHERE patientCardId = %1" ).arg( p_uiId ) );

    if( poQuery->size() != 1 )
        throw cSevException( cSeverity::ERROR, "Patientcard id not found" );

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
    qsQuery += " patientCards SET ";
    qsQuery += QString( "licenceId = \"%1\", " ).arg( m_uiLicenceId );
    qsQuery += QString( "patientCardTypeId = \"%1\", " ).arg( m_uiPatientCardTypeId );
    qsQuery += QString( "patientId = \"%1\", " ).arg( m_uiPatientId );
    qsQuery += QString( "barcode = \"%1\", " ).arg( QString::fromStdString( m_stBarcode ) );
    qsQuery += QString( "comment = \"%1\", " ).arg( QString::fromStdString( m_stComment ) );
    qsQuery += QString( "units = \"%1\", " ).arg( m_nUnits );
    qsQuery += QString( "timeLeft = \"%1\", " ).arg( QString::fromStdString( m_stTimeLeft ) );
    qsQuery += QString( "validDate = \"%1\", " ).arg( QString::fromStdString( m_stValidDate ) );
    qsQuery += QString( "pincode = \"%1\", " ).arg( QString::fromStdString( m_stPincode ) );
    qsQuery += QString( "active = %1, " ).arg( m_bActive );
    qsQuery += QString( "archive = \"%1\" " ).arg( QString::fromStdString( m_stArchive ) );
    if( m_uiId )
    {
        qsQuery += QString( " WHERE patientCardId = %1" ).arg( m_uiId );
    }

    QSqlQuery  *poQuery = g_poDB->executeQTQuery( qsQuery );
    if( !m_uiId && poQuery ) m_uiId = poQuery->lastInsertId().toUInt();
    if( poQuery ) delete poQuery;
}

void cDBPatientCard::remove() throw( cSevException )
{
    cTracer obTrace( "cDBPatientCard::remove" );

    if( m_uiId )
    {
        QString  qsQuery;

        if( m_stArchive.compare( "NEW" ) == 0 )
        {
            qsQuery = "DELETE FROM patientCards ";
        }
        else
        {
            qsQuery = "UPDATE patientCards SET active=0, archive=\"MOD\" ";
        }
        qsQuery += QString( " WHERE patientCardId = %1" ).arg( m_uiId );

        QSqlQuery  *poQuery = g_poDB->executeQTQuery( qsQuery );
        if( poQuery ) delete poQuery;
    }
}

bool cDBPatientCard::isPatientCardTypeLinked( const unsigned int p_PCTId ) throw()
{
    cTracer obTrace( "cDBPatientCard::isPatientCardTypeLinked", QString( "id: %1" ).arg( p_PCTId ).toStdString() );

    QSqlQuery *poQuery = g_poDB->executeQTQuery( QString( "SELECT * FROM patientCards WHERE patientCardTypeId = %1" ).arg( p_PCTId ) );

    if( poQuery->size() > 0 )
        return true;
    else
        return false;
}

void cDBPatientCard::createNew() throw()
{
    init();
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

unsigned int cDBPatientCard::patientCardTypeId() const throw()
{
    return m_uiPatientCardTypeId;
}

void cDBPatientCard::setPatientCardTypeId( const unsigned int p_uiPCardTypeId ) throw()
{
    m_uiPatientCardTypeId = p_uiPCardTypeId;
}

unsigned int cDBPatientCard::patientId() const throw()
{
    return m_uiPatientId;
}

void cDBPatientCard::setPatientId( const unsigned int p_uiPatientId ) throw()
{
    m_uiPatientId = p_uiPatientId;
}

string cDBPatientCard::barcode() const throw()
{
    return m_stBarcode;
}

void cDBPatientCard::setBarcode( const string &p_stBarcode ) throw()
{
    m_stBarcode = p_stBarcode;
}

string cDBPatientCard::comment() const throw()
{
    return m_stComment;
}

void cDBPatientCard::setComment( const string &p_stComment ) throw()
{
    m_stComment = p_stComment;
}

int cDBPatientCard::units() const throw()
{
    return m_nUnits;
}

void cDBPatientCard::setUnits( const int p_nUnits ) throw()
{
    m_nUnits = p_nUnits;
}

string cDBPatientCard::timeLeft() const throw()
{
    return m_stTimeLeft;
}

void cDBPatientCard::setTimeLeft( const string &p_stTimeLeft ) throw()
{
    m_stTimeLeft = p_stTimeLeft;
}

string cDBPatientCard::validDate() const throw()
{
    return m_stValidDate;
}

void cDBPatientCard::setValidDate( const string &p_stValidDate ) throw()
{
    m_stValidDate = p_stValidDate;
}

string cDBPatientCard::pincode() const throw()
{
    return m_stPincode;
}

void cDBPatientCard::setPincode( const string &p_stPincode ) throw()
{
    m_stPincode = p_stPincode;
}

bool cDBPatientCard::active() const throw()
{
    return m_bActive;
}

void cDBPatientCard::setActive( const bool p_bActive ) throw()
{
    m_bActive = p_bActive;
}

string cDBPatientCard::archive() const throw()
{
    return m_stArchive;
}

void cDBPatientCard::setArchive( const string &p_stArchive ) throw()
{
    m_stArchive = p_stArchive;
}
