//====================================================================================
//
// Belenus Server alkalmazas (c) Pagony Multimedia Studio Bt - 2010
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
                      const unsigned int p_uiPatientId,
                      const unsigned int p_uiPaymentMethodId,
                      const int p_inUnits,
                      const int p_inCash,
                      const int p_inTimeReal,
                      const int p_inTimeLeft,
                      const int p_inTimeCard,
                      const int p_inTimeCash,
                      const QString &p_qsLedgerTime,
                      const QString &p_qsComment,
                      const QString &p_qsModified,
                      const bool p_bActive,
                      const QString &p_qsArchive ) throw()
{
    m_uiId                  = p_uiId;
    m_uiLicenceId           = p_uiLicenceId;
    m_uiUserId              = p_uiUserId;
    m_uiPanelId             = p_uiPanelId;
    m_uiPatientId           = p_uiPatientId;
    m_uiPaymentMethod       = p_uiPaymentMethodId;
    m_inUnits               = p_inUnits;
    m_inCash                = p_inCash;
    m_inTimeReal            = p_inTimeReal;
    m_inTimeLeft            = p_inTimeLeft;
    m_inTimeCard            = p_inTimeCard;
    m_inTimeCash            = p_inTimeCash;
    m_qsLedgerTime          = p_qsLedgerTime;
    m_qsComment             = p_qsComment;
    m_qsModified        = p_qsModified;
    m_bActive               = p_bActive;
    m_qsArchive             = p_qsArchive;
}

void cDBLedgerDevice::init( const QSqlRecord &p_obRecord ) throw()
{
    int inIdIdx                 = p_obRecord.indexOf( "ledgerDeviceId" );
    int inLicenceIdIdx          = p_obRecord.indexOf( "licenceId" );
    int inUserIdIdx             = p_obRecord.indexOf( "userId" );
    int inPanelIdIdx            = p_obRecord.indexOf( "panelId" );
    int inPatientIdIdx          = p_obRecord.indexOf( "patientId" );
    int inPaymentMethodIdx      = p_obRecord.indexOf( "paymentMethodId" );
    int inUnitsIdx              = p_obRecord.indexOf( "units" );
    int inCashIdx               = p_obRecord.indexOf( "cash" );
    int inTimeRealIdx           = p_obRecord.indexOf( "timeReal" );
    int inTimeLeftIdx           = p_obRecord.indexOf( "timeLeft" );
    int inTimeCardIdx           = p_obRecord.indexOf( "timeCard" );
    int inTimeCashIdx           = p_obRecord.indexOf( "timeCash" );
    int inLedgerTimeIdx         = p_obRecord.indexOf( "ledgerTime" );
    int inCommentIdx            = p_obRecord.indexOf( "comment" );
    int inModifiedIdx       = p_obRecord.indexOf( "modified" );
    int inActiveIdx             = p_obRecord.indexOf( "active" );
    int inArchiveIdx            = p_obRecord.indexOf( "archive" );

    init( p_obRecord.value( inIdIdx ).toUInt(),
          p_obRecord.value( inLicenceIdIdx ).toUInt(),
          p_obRecord.value( inUserIdIdx ).toUInt(),
          p_obRecord.value( inPanelIdIdx ).toUInt(),
          p_obRecord.value( inPatientIdIdx ).toUInt(),
          p_obRecord.value( inPaymentMethodIdx ).toUInt(),
          p_obRecord.value( inUnitsIdx ).toInt(),
          p_obRecord.value( inCashIdx ).toInt(),
          p_obRecord.value( inTimeRealIdx ).toInt(),
          p_obRecord.value( inTimeLeftIdx ).toInt(),
          p_obRecord.value( inTimeCardIdx ).toInt(),
          p_obRecord.value( inTimeCashIdx ).toInt(),
          p_obRecord.value( inLedgerTimeIdx ).toString(),
          p_obRecord.value( inCommentIdx ).toString(),
          p_obRecord.value( inModifiedIdx ).toString(),
          p_obRecord.value( inActiveIdx ).toBool(),
          p_obRecord.value( inArchiveIdx ).toString() );
}

void cDBLedgerDevice::load( const unsigned int p_uiId ) throw( cSevException )
{
    cTracer obTrace( "cDBLedgerDevice::load", QString( "id: %1" ).arg( p_uiId ) );

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

        if( m_qsArchive != "NEW" )
        {
            m_qsArchive = "MOD";
        }
    }
    else
    {
        qsQuery = "INSERT INTO";
        m_qsArchive = "NEW";
    }
    qsQuery += " ledgerDevice SET ";
    qsQuery += QString( "licenceId = \"%1\", " ).arg( m_uiLicenceId );
    qsQuery += QString( "userId = \"%1\", " ).arg( m_uiUserId );
    qsQuery += QString( "panelId = \"%1\", " ).arg( m_uiPanelId );
    qsQuery += QString( "patientId = \"%1\", " ).arg( m_uiPatientId );
    qsQuery += QString( "paymentMethodId = \"%1\", " ).arg( m_uiPaymentMethod );
    qsQuery += QString( "units = \"%1\", " ).arg( m_inUnits );
    qsQuery += QString( "cash = \"%1\", " ).arg( m_inCash );
    qsQuery += QString( "timeReal = \"%1\", " ).arg( m_inTimeReal );
    qsQuery += QString( "timeLeft = \"%1\", " ).arg( m_inTimeLeft );
    qsQuery += QString( "timeCard = \"%1\", " ).arg( m_inTimeCard );
    qsQuery += QString( "timeCash = \"%1\", " ).arg( m_inTimeCash );
    qsQuery += QString( "comment = \"%1\", " ).arg( m_qsComment );
    qsQuery += QString( "modified = \"%1\", " ).arg( QDateTime::currentDateTime().toString( QString("yyyy-MM-dd hh:mm:ss") ) );
    qsQuery += QString( "active = %1, " ).arg( m_bActive );
    qsQuery += QString( "archive = \"%1\" " ).arg( m_qsArchive );
    if( m_uiId )
    {
        qsQuery += QString( " WHERE ledgerDeviceId = %1" ).arg( m_uiId );
    }

    QSqlQuery  *poQuery = g_poDB->executeQTQuery( qsQuery );
    if( !m_uiId && poQuery ) m_uiId = poQuery->lastInsertId().toUInt();
    if( poQuery ) delete poQuery;
/*
    if( m_uiId > 0 && m_uiLicenceId != 1 )
        g_obDBMirror.updateSynchronizationLevel( DB_LEDGER_DEVICE );
    if( m_uiId > 0 && m_uiLicenceId == 0 )
        g_obDBMirror.updateGlobalSyncLevel( DB_LEDGER_DEVICE );
*/
}

void cDBLedgerDevice::remove() throw( cSevException )
{
    cTracer obTrace( "cDBLedgerDevice::remove" );

    if( m_uiId )
    {
        QString  qsQuery;

        if( m_qsArchive == "NEW" )
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

unsigned int cDBLedgerDevice::patientId() const throw()
{
    return m_uiPatientId;
}

void cDBLedgerDevice::setPatientId( const unsigned int p_nPatientId ) throw()
{
    m_uiPatientId = p_nPatientId;
}

unsigned int cDBLedgerDevice::paymentMethod() const throw()
{
    return m_uiPaymentMethod;
}

void cDBLedgerDevice::setPaymentMethod( const unsigned int p_nPaymentMethod ) throw()
{
    m_uiPaymentMethod = p_nPaymentMethod;
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

QString cDBLedgerDevice::ledgerTime() const throw()
{
    return m_qsLedgerTime;
}

void cDBLedgerDevice::setLedgerTime( const QString &p_qsLedgerTime ) throw()
{
    m_qsLedgerTime = p_qsLedgerTime;
}

QString cDBLedgerDevice::comment() const throw()
{
    return m_qsComment;
}

void cDBLedgerDevice::setComment( const QString &p_qsComment ) throw()
{
    m_qsComment = p_qsComment;
    m_qsComment = m_qsComment.replace( QString("\""), QString("\\\"") );
}

QString cDBLedgerDevice::modified() const throw()
{
    return m_qsModified;
}

bool cDBLedgerDevice::active() const throw()
{
    return m_bActive;
}

void cDBLedgerDevice::setActive( const bool p_bActive ) throw()
{
    m_bActive = p_bActive;
}

QString cDBLedgerDevice::archive() const throw()
{
    return m_qsArchive;
}

void cDBLedgerDevice::setArchive( const QString &p_qsArchive ) throw()
{
    m_qsArchive = p_qsArchive;
}

