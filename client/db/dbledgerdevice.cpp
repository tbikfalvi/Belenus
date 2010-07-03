//====================================================================================
//
// Belenus Server alkalmazas © Pagony Multimedia Studio Bt - 2010
//
//====================================================================================
//
// Filename    : dbledgerdevice.cpp
// AppVersion  : 1.0
// FileVersion : 1.0
// Author      : Bikfalvi Tamas
//
//====================================================================================
//
//====================================================================================

#include "belenus.h"
#include "dbledgerdevice.h"

cDBLedgerDevice::cDBLedgerDevice()
{
    init();
}

cDBLedgerDevice::~cDBLedgerDevice()
{
}

void cDBLedgerDevice::init( const unsigned int p_uiId,
                      const unsigned int p_uiLicenceId,
                      const unsigned int p_uiUserId,
                      const unsigned int p_uiPanelId,
                      const unsigned int p_uiPatientCardId,
                      const int p_inUnits,
                      const int p_inCash,
                      const int p_inTimeReal,
                      const int p_inTimeLeft,
                      const int p_inTimeCard,
                      const int p_inTimeCash,
                      const string &p_stLedgerTime,
                      const string &p_stComment,
                      const bool p_bActive,
                      const string &p_stArchive ) throw()
{
    m_uiId                  = p_uiId;
    m_uiLicenceId           = p_uiLicenceId;
    m_uiUserId              = p_uiUserId;
    m_uiPanelId             = p_uiPanelId;
    m_uiPatientCardId       = p_uiPatientCardId;
    m_inUnits               = p_inUnits;
    m_inCash                = p_inCash;
    m_inTimeReal            = p_inTimeReal;
    m_inTimeLeft            = p_inTimeLeft;
    m_inTimeCard            = p_inTimeCard;
    m_inTimeCash            = p_inTimeCash;
    m_stLedgerTime          = p_stLedgerTime;
    m_stComment             = p_stComment;
    m_bActive               = p_bActive;
    m_stArchive             = p_stArchive;
}

void cDBLedgerDevice::init( const QSqlRecord &p_obRecord ) throw()
{
    int inIdIdx                 = p_obRecord.indexOf( "ledgerDeviceId" );
    int inLicenceIdIdx          = p_obRecord.indexOf( "licenceId" );
    int inUserIdIdx             = p_obRecord.indexOf( "userId" );
    int inPanelIdIdx            = p_obRecord.indexOf( "panelId" );
    int inPatientCardIdIdx      = p_obRecord.indexOf( "patientCardId" );
    int inUnitsIdx              = p_obRecord.indexOf( "units" );
    int inCashIdx               = p_obRecord.indexOf( "cash" );
    int inTimeRealIdx           = p_obRecord.indexOf( "timeReal" );
    int inTimeLeftIdx           = p_obRecord.indexOf( "timeLeft" );
    int inTimeCardIdx           = p_obRecord.indexOf( "timeCard" );
    int inTimeCashIdx           = p_obRecord.indexOf( "timeCash" );
    int inLedgerTimeIdx         = p_obRecord.indexOf( "ledgerTime" );
    int inCommentIdx            = p_obRecord.indexOf( "comment" );
    int inActiveIdx             = p_obRecord.indexOf( "active" );
    int inArchiveIdx            = p_obRecord.indexOf( "archive" );

    init( p_obRecord.value( inIdIdx ).toUInt(),
          p_obRecord.value( inLicenceIdIdx ).toUInt(),
          p_obRecord.value( inUserIdIdx ).toUInt(),
          p_obRecord.value( inPanelIdIdx ).toUInt(),
          p_obRecord.value( inPatientCardIdIdx ).toUInt(),
          p_obRecord.value( inUnitsIdx ).toInt(),
          p_obRecord.value( inCashIdx ).toInt(),
          p_obRecord.value( inTimeRealIdx ).toInt(),
          p_obRecord.value( inTimeLeftIdx ).toInt(),
          p_obRecord.value( inTimeCardIdx ).toInt(),
          p_obRecord.value( inTimeCashIdx ).toInt(),
          p_obRecord.value( inLedgerTimeIdx ).toString().toStdString(),
          p_obRecord.value( inCommentIdx ).toString().toStdString(),
          p_obRecord.value( inActiveIdx ).toBool(),
          p_obRecord.value( inArchiveIdx ).toString().toStdString() );
}

void cDBLedgerDevice::load( const unsigned int p_uiId ) throw( cSevException )
{
    cTracer obTrace( "cDBLedgerDevice::load", QString( "id: %1" ).arg( p_uiId ).toStdString() );

    QSqlQuery *poQuery = g_poDB->executeQTQuery( QString( "SELECT * FROM ledgerDevice WHERE ledgerDeviceId = %1" ).arg( p_uiId ) );

    if( poQuery->size() != 1 )
        throw cSevException( cSeverity::ERROR, "LedgerDevice id not found" );

    poQuery->first();
    init( poQuery->record() );
}

void cDBLedgerDevice::save() throw( cSevException )
{
    cTracer obTrace( "cDBLedgerDevice::save" );
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
    qsQuery += " ledgerDevice SET ";
    qsQuery += QString( "licenceId = \"%1\", " ).arg( m_uiLicenceId );
    qsQuery += QString( "userId = \"%1\", " ).arg( m_uiUserId );
    qsQuery += QString( "panelId = \"%1\", " ).arg( m_uiPanelId );
    qsQuery += QString( "patientCardId = \"%1\", " ).arg( m_uiPatientCardId );
    qsQuery += QString( "units = \"%1\", " ).arg( m_inUnits );
    qsQuery += QString( "cash = \"%1\", " ).arg( m_inCash );
    qsQuery += QString( "timeReal = \"%1\", " ).arg( m_inTimeReal );
    qsQuery += QString( "timeLeft = \"%1\", " ).arg( m_inTimeLeft );
    qsQuery += QString( "timeCard = \"%1\", " ).arg( m_inTimeCard );
    qsQuery += QString( "timeCash = \"%1\", " ).arg( m_inTimeCash );
    qsQuery += QString( "comment = \"%1\", " ).arg( QString::fromStdString( m_stComment ) );
    qsQuery += QString( "active = %1, " ).arg( m_bActive );
    qsQuery += QString( "archive = \"%1\" " ).arg( QString::fromStdString( m_stArchive ) );
    if( m_uiId )
    {
        qsQuery += QString( " WHERE ledgerDeviceId = %1" ).arg( m_uiId );
    }

    QSqlQuery  *poQuery = g_poDB->executeQTQuery( qsQuery );
    if( !m_uiId && poQuery ) m_uiId = poQuery->lastInsertId().toUInt();
    if( poQuery ) delete poQuery;
}

void cDBLedgerDevice::remove() throw( cSevException )
{
    cTracer obTrace( "cDBLedgerDevice::remove" );

    if( m_uiId )
    {
        QString  qsQuery;

        if( m_stArchive.compare( "NEW" ) == 0 )
        {
            qsQuery = "DELETE FROM ledgerDevice ";
        }
        else
        {
            qsQuery = "UPDATE ledgerDevice SET active=0, archive=\"MOD\" ";
        }
        qsQuery += QString( " WHERE ledgerDeviceId = %1" ).arg( m_uiId );

        QSqlQuery  *poQuery = g_poDB->executeQTQuery( qsQuery );
        if( poQuery ) delete poQuery;
    }
}

void cDBLedgerDevice::createNew() throw()
{
    init();
}

unsigned int cDBLedgerDevice::id() const throw()
{
    return m_uiId;
}

unsigned int cDBLedgerDevice::licenceId() const throw()
{
    return m_uiLicenceId;
}

void cDBLedgerDevice::setLicenceId( const unsigned int p_uiLicenceId ) throw()
{
    m_uiLicenceId = p_uiLicenceId;
}

unsigned int cDBLedgerDevice::userId() const throw()
{
    return m_uiUserId;
}

void cDBLedgerDevice::setUserId( const unsigned int p_nUserId ) throw()
{
    m_uiUserId = p_nUserId;
}

unsigned int cDBLedgerDevice::panelId() const throw()
{
    return m_uiPanelId;
}

void cDBLedgerDevice::setPanelId( const unsigned int p_nPanelId ) throw()
{
    m_uiPanelId = p_nPanelId;
}

unsigned int cDBLedgerDevice::patientCardId() const throw()
{
    return m_uiPatientCardId;
}

void cDBLedgerDevice::setPatientCardId( const unsigned int p_nPatientCardId ) throw()
{
    m_uiPatientCardId = p_nPatientCardId;
}

int cDBLedgerDevice::units() const throw()
{
    return m_inUnits;
}

void cDBLedgerDevice::setUnits( const int p_inUnits ) throw()
{
    m_inUnits = p_inUnits;
}

int cDBLedgerDevice::cash() const throw()
{
    return m_inCash;
}

void cDBLedgerDevice::setCash( const int p_inCash ) throw()
{
    m_inCash = p_inCash;
}

int cDBLedgerDevice::timeReal() const throw()
{
    return m_inTimeReal;
}

void cDBLedgerDevice::setTimeReal( const int p_inTimeReal ) throw()
{
    m_inTimeReal = p_inTimeReal;
}

int cDBLedgerDevice::timeLeft() const throw()
{
    return m_inTimeLeft;
}

void cDBLedgerDevice::setTimeLeft( const int p_inTimeLeft ) throw()
{
    m_inTimeLeft = p_inTimeLeft;
}

int cDBLedgerDevice::timeCard() const throw()
{
    return m_inTimeCard;
}

void cDBLedgerDevice::setTimeCard( const int p_inTimeCard ) throw()
{
    m_inTimeCard = p_inTimeCard;
}

int cDBLedgerDevice::timeCash() const throw()
{
    return m_inTimeCash;
}

void cDBLedgerDevice::setTimeCash( const int p_inTimeCash ) throw()
{
    m_inTimeCash = p_inTimeCash;
}

string cDBLedgerDevice::ledgerTime() const throw()
{
    return m_stLedgerTime;
}

void cDBLedgerDevice::setLedgerTime( const string &p_stLedgerTime ) throw()
{
    m_stLedgerTime = p_stLedgerTime;
}

string cDBLedgerDevice::comment() const throw()
{
    return m_stComment;
}

void cDBLedgerDevice::setComment( const string &p_stComment ) throw()
{
    m_stComment = p_stComment;
}

bool cDBLedgerDevice::active() const throw()
{
    return m_bActive;
}

void cDBLedgerDevice::setActive( const bool p_bActive ) throw()
{
    m_bActive = p_bActive;
}

string cDBLedgerDevice::archive() const throw()
{
    return m_stArchive;
}

void cDBLedgerDevice::setArchive( const string &p_stArchive ) throw()
{
    m_stArchive = p_stArchive;
}

