//====================================================================================
//
// Belenus Server alkalmazas (c) Pagony Multimedia Studio Bt - 2010
//
//====================================================================================
//
// Filename    : dbskintypes.cpp
// AppVersion  : 1.0
// FileVersion : 1.0
// Author      : Bikfalvi Tamas
//
//====================================================================================
//
//====================================================================================

#include "belenus.h"
#include "dbsendmail.h"

cDBSendMail::cDBSendMail()
{
    init();
}

cDBSendMail::~cDBSendMail()
{
}

void cDBSendMail::init(const unsigned int p_uiId,
                       const unsigned int p_uiLicenceId,
                       unsigned int p_uiMailTypeId,
                       QString p_qsDateSend,
                       QString p_qsRecipients,
                       QString p_qsSubject,
                       QString p_qsMessage,
                       QString p_qsVarName,
                       QString p_qsVarBarcode,
                       QString p_qsVarCardInfo,
                       QString p_qsVarUnitCount,
                       QString p_qsVarDateTime,
                       const bool p_bActive,
                       const QString &p_qsArchive ) throw()
{
    m_uiId              = p_uiId;
    m_uiLicenceId       = p_uiLicenceId;
    m_uiMailTypeId      = p_uiMailTypeId;
    m_qsDateSend        = p_qsDateSend;
    m_qsRecipients      = p_qsRecipients;
    m_qsSubject         = p_qsSubject;
    m_qsMessage         = p_qsMessage;
    m_qsVarName         = p_qsVarName;
    m_qsVarBarcode      = p_qsVarBarcode;
    m_qsVarCardInfo     = p_qsVarCardInfo;
    m_qsVarUnitCount    = p_qsVarUnitCount;
    m_qsVarDateTime     = p_qsVarDateTime;
    m_bActive           = p_bActive;
    m_qsArchive         = p_qsArchive;
}

void cDBSendMail::init( const QSqlRecord &p_obRecord ) throw()
{
    int inIdIdx             = p_obRecord.indexOf( "httpSendMailId" );
    int inLicenceIdIdx      = p_obRecord.indexOf( "licenceId" );
    int inMailTypeIdx       = p_obRecord.indexOf( "mailTypeId" );
    int inDateOfSendingIdx  = p_obRecord.indexOf( "dateOfSending" );
    int inRecipientsIdx     = p_obRecord.indexOf( "recipients" );
    int inSubjectIdx        = p_obRecord.indexOf( "subject" );
    int inMessageIdx        = p_obRecord.indexOf( "mailbody" );
    int inVarNameIdx        = p_obRecord.indexOf( "var_name" );
    int inVarBarcodeIdx     = p_obRecord.indexOf( "var_barcode" );
    int inVarCardInfoIdx    = p_obRecord.indexOf( "var_cardinfo" );
    int inVarUnitCountIdx   = p_obRecord.indexOf( "var_unitcount" );
    int inVarDateTimeIdx    = p_obRecord.indexOf( "var_datetime" );
    int inActiveIdx         = p_obRecord.indexOf( "active" );
    int inArchiveIdx        = p_obRecord.indexOf( "archive" );

    init( p_obRecord.value( inIdIdx ).toInt(),
          p_obRecord.value( inLicenceIdIdx ).toInt(),
          p_obRecord.value( inMailTypeIdx ).toInt(),
          p_obRecord.value( inDateOfSendingIdx ).toString(),
          p_obRecord.value( inRecipientsIdx ).toString(),
          p_obRecord.value( inSubjectIdx ).toString(),
          p_obRecord.value( inMessageIdx ).toString(),
          p_obRecord.value( inVarNameIdx ).toString(),
          p_obRecord.value( inVarBarcodeIdx ).toString(),
          p_obRecord.value( inVarCardInfoIdx ).toString(),
          p_obRecord.value( inVarUnitCountIdx ).toString(),
          p_obRecord.value( inVarDateTimeIdx ).toString(),
          p_obRecord.value( inActiveIdx ).toBool(),
          p_obRecord.value( inArchiveIdx ).toString() );
}

void cDBSendMail::load( const unsigned int p_uiId ) throw( cSevException )
{
    cTracer obTrace( "cDBSendMail::load", QString( "id: %1" ).arg( p_uiId ) );

    QSqlQuery *poQuery = g_poDB->executeQTQuery( QString( "SELECT * FROM httpsendmail WHERE httpSendMailId = %1" ).arg( p_uiId ) );

    if( poQuery->size() != 1 )
        throw cSevException( cSeverity::ERROR, "Mail id not found" );

    poQuery->first();
    init( poQuery->record() );
}

void cDBSendMail::save() throw( cSevException )
{
    cTracer obTrace( "cDBSendMail::save" );
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
    qsQuery += " httpsendmail SET ";
    qsQuery += QString( "licenceId = \"%1\", " ).arg( m_uiLicenceId );
    qsQuery += QString( "mailTypeId = \"%1\", " ).arg( m_uiMailTypeId );
    qsQuery += QString( "dateOfSending = \"%1\", " ).arg( m_qsDateSend );
    qsQuery += QString( "recipients = \"%1\", " ).arg( m_qsRecipients );
    qsQuery += QString( "subject = \"%1\", " ).arg( m_qsSubject );
    qsQuery += QString( "mailbody = \"%1\", " ).arg( m_qsMessage );
    qsQuery += QString( "var_name = \"%1\", " ).arg( m_qsVarName );
    qsQuery += QString( "var_barcode = \"%1\", " ).arg( m_qsVarBarcode );
    qsQuery += QString( "var_cardinfo = \"%1\", " ).arg( m_qsVarCardInfo );
    qsQuery += QString( "var_unitcount = \"%1\", " ).arg( m_qsVarUnitCount );
    qsQuery += QString( "var_datetime = \"%1\", " ).arg( m_qsVarDateTime );
    qsQuery += QString( "active = %1, " ).arg( m_bActive );
    qsQuery += QString( "archive = \"%1\" " ).arg( m_qsArchive );
    if( m_uiId )
    {
        qsQuery += QString( " WHERE httpSendMailId = %1" ).arg( m_uiId );
    }

    QSqlQuery  *poQuery = g_poDB->executeQTQuery( qsQuery );
    if( !m_uiId && poQuery ) m_uiId = poQuery->lastInsertId().toUInt();
    if( poQuery ) delete poQuery;
}

void cDBSendMail::remove() throw( cSevException )
{
    cTracer obTrace( "cDBSendMail::remove" );

    if( m_uiId )
    {
        QString  qsQuery;

        qsQuery = "DELETE FROM httpsendmail ";
        qsQuery += QString( " WHERE httpSendMailId = %1" ).arg( m_uiId );

        QSqlQuery  *poQuery = g_poDB->executeQTQuery( qsQuery );
        if( poQuery ) delete poQuery;
    }
}

void cDBSendMail::createNew() throw()
{
    init();
}

unsigned int cDBSendMail::id() const throw()
{
    return m_uiId;
}

unsigned int cDBSendMail::licenceId() const throw()
{
    return m_uiLicenceId;
}

void cDBSendMail::setLicenceId( const unsigned int p_uiLicenceId ) throw()
{
    m_uiLicenceId = p_uiLicenceId;
}

unsigned int cDBSendMail::mailTypeId() const throw()
{
    return m_uiMailTypeId;
}

void cDBSendMail::setMailTypeId( const unsigned int p_nMailTypeId ) throw()
{
    m_uiMailTypeId = p_nMailTypeId;
}

QString cDBSendMail::dateSend() const throw()
{
    return m_qsDateSend;
}

void cDBSendMail::setDateSend( const QString &p_qsDateSend ) throw()
{
    m_qsDateSend = p_qsDateSend;
}

QString cDBSendMail::recipients() const throw()
{
    return m_qsRecipients;
}

void cDBSendMail::setRecipients( const QString &p_qsRecipients ) throw()
{
    m_qsRecipients = p_qsRecipients;
}

QString cDBSendMail::subject() const throw()
{
    return m_qsSubject;
}

void cDBSendMail::setSubject( const QString &p_qsSubject ) throw()
{
    m_qsSubject = p_qsSubject;
    m_qsSubject = m_qsSubject.replace( QString("\""), QString("\\\"") );
}

QString cDBSendMail::message() const throw()
{
    return m_qsMessage;
}

void cDBSendMail::setMessage( const QString &p_qsMessage ) throw()
{
    m_qsMessage = p_qsMessage;
    m_qsMessage = m_qsMessage.replace( QString("\""), QString("\\\"") );
}

QString cDBSendMail::varName() const throw()
{
    return m_qsVarName;
}

void cDBSendMail::setVarName( const QString &p_qsVarName ) throw()
{
    m_qsVarName = p_qsVarName;
}

QString cDBSendMail::varBardcode() const throw()
{
    return m_qsVarBarcode;
}

void cDBSendMail::setVarBardcode( const QString &p_qsVarBardcode ) throw()
{
    m_qsVarBarcode = p_qsVarBardcode;
}

QString cDBSendMail::varCardInfo() const throw()
{
    return m_qsVarCardInfo;
}

void cDBSendMail::setVarCardInfo( const QString &p_qsVarCardInfo ) throw()
{
    m_qsVarCardInfo = p_qsVarCardInfo;
}

QString cDBSendMail::varUnitCount() const throw()
{
    return m_qsVarUnitCount;
}

void cDBSendMail::setVarUnitCount( const QString &p_qsVarUnitCount ) throw()
{
    m_qsVarUnitCount = p_qsVarUnitCount;
}

QString cDBSendMail::varDateTime() const throw()
{
    return m_qsVarDateTime;
}

void cDBSendMail::setVarDateTime( const QString &p_qsVarDateTime ) throw()
{
    m_qsVarDateTime = p_qsVarDateTime;
}

bool cDBSendMail::active() const throw()
{
    return m_bActive;
}

void cDBSendMail::setActive( const bool p_bActive ) throw()
{
    m_bActive = p_bActive;
}

QString cDBSendMail::archive() const throw()
{
    return m_qsArchive;
}

void cDBSendMail::setArchive( const QString &p_qsArchive ) throw()
{
    m_qsArchive = p_qsArchive;
}
