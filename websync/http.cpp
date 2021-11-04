//=================================================================================================
//
// Belenus Kliens alkalmazas (c) Pagony Multimedia Studio Bt - 2014
//
//-------------------------------------------------------------------------------------------------
//
// Filename    : http.cpp
// AppVersion  : 1.0
// FileVersion : 1.0
// Author      : Bikfalvi Tamas
//
//-------------------------------------------------------------------------------------------------
// Http kapcsolattal osszefuggo reszek implementalasa
//=================================================================================================

#include <QtNetwork>
#include <QTimer>
#include <QStringList>
#include <QMessageBox>
#include <QCryptographicHash>
#include <iostream>

#include "http.h"
//#include "belenus.h"
#include "..\client\licenceManager.h"
#include "dlgmain.h"

//=================================================================================================
cBlnsHttp::cBlnsHttp()
//-------------------------------------------------------------------------------------------------
{
    //cTracer obTrace( "cBlnsHttp::cBlnsHttp" );

    obHttp                      = NULL;
    obFile                      = NULL;
    m_httpRequestAborted        = false;
    m_httpGetId                 = 0;

    m_qsError                   = "";
    m_inTimeout                 = 0;
    m_inTimer                   = 0;
    m_qsToken                   = "";
    m_inHttpProcessStep         = HTTP_STATUS_DEFAULT;          // -1
    m_teBlnsHttpProcess         = cBlnsHttpAction::HA_DEFAULT;  //  0

    m_qsServerAddress           = "0.0.0.0";
    m_bIsHttpEnabled            = true;
    m_bIsHttpSuspended          = false;
    m_uiLicenceId               = 0;
    m_qsLicenceString           = "BLNS_SERIAL_DEMO";
    m_uiPatientCardTypeId       = 0;
    m_nPatientCardTypePrice     = 0;
    m_nPatientCardTypeUnits     = 1;
    m_nPatientCardTypeUnitTime  = 0;
    m_bGetOnlinePCProcessed     = false;

    m_uiCommId                  = 0;

    m_vrHttpActions.clear();

    obHttp = new QHttp( this );

    connect(obHttp, SIGNAL(requestFinished(int, bool)), this, SLOT(_slotHttpRequestFinished(int,bool)));
    connect(obHttp, SIGNAL(dataReadProgress(int, int)), this, SLOT(_slotUpdateDataReadProgress(int, int)));
    connect(obHttp, SIGNAL(responseHeaderReceived(const QHttpResponseHeader &)), this, SLOT(_slotReadResponseHeader(const QHttpResponseHeader &)));
    connect(obHttp, SIGNAL(authenticationRequired(const QString &, quint16, QAuthenticator *)), this, SLOT(_slotAuthenticationRequired(const QString &, quint16, QAuthenticator *)));
#ifndef QT_NO_OPENSSL
    connect(obHttp, SIGNAL(sslErrors(const QList<QSslError> &)), this, SLOT(_slotSslErrors(const QList<QSslError> &)));
#endif

    obResponseXML = new QDomDocument( "ResponseXML" );
}
//=================================================================================================
cBlnsHttp::~cBlnsHttp()
//-------------------------------------------------------------------------------------------------
{
    //cTracer obTrace( "cBlnsHttp::~cBlnsHttp" );

    if( m_inTimer > 0 )
        killTimer( m_inTimer );
}
//=================================================================================================
//=================================================================================================
//=================================================================================================
//
// GLOBAL functions for define the settings for HTTP
//
//=================================================================================================
//=================================================================================================
//=================================================================================================
void cBlnsHttp::setTimeout( const int p_inTimeout )
//-------------------------------------------------------------------------------------------------
{
    m_inTimeout = p_inTimeout;
}

//=================================================================================================
void cBlnsHttp::setServerAddress( QString p_qsServerAddress )
//-------------------------------------------------------------------------------------------------
{
    m_qsServerAddress = p_qsServerAddress;
}

//=================================================================================================
void cBlnsHttp::setStudioLicence( unsigned int p_uiLicenceId, QString p_qsLicenceString )
//-------------------------------------------------------------------------------------------------
{
    m_uiLicenceId       = p_uiLicenceId;
    m_qsLicenceString   = p_qsLicenceString;
}

//=================================================================================================
void cBlnsHttp::setOnlinePCType(unsigned int p_uiId, int p_nPrice, int p_nUnits, int p_nUnitTime)
//-------------------------------------------------------------------------------------------------
{
    m_uiPatientCardTypeId       = p_uiId;
    m_nPatientCardTypePrice     = p_nPrice;
    m_nPatientCardTypeUnits     = p_nUnits;
    m_nPatientCardTypeUnitTime  = p_nUnitTime;

    if( m_nPatientCardTypeUnits == 0 )  m_nPatientCardTypeUnits = 1;
}

//=================================================================================================
void cBlnsHttp::setOnlinePaymentMethod(unsigned int p_uiPaymentMethod)
{
    m_uiPaymentMethod = p_uiPaymentMethod;
}

//=================================================================================================
void cBlnsHttp::setCommunicationEnabled(bool p_bHttpEnabled)
//-------------------------------------------------------------------------------------------------
{
    m_bIsHttpEnabled = p_bHttpEnabled;
}

//=================================================================================================
void cBlnsHttp::setCommunicationSuspended(bool p_bHttpSuspended)
//-------------------------------------------------------------------------------------------------
{
    m_bIsHttpSuspended = p_bHttpSuspended;
}

//=================================================================================================
//=================================================================================================
//=================================================================================================
//
// GLOBAL functions to initiate http processes
//
//=================================================================================================
//=================================================================================================
//=================================================================================================
void cBlnsHttp::checkHttpServerAvailability()
//-------------------------------------------------------------------------------------------------
{
    g_obLogger(cSeverity::DEBUG) << "HTTP: Check if web server send token" << EOM;
    m_vrHttpActions.clear();
    m_vrHttpActions.push_back( cBlnsHttpAction::HA_AUTHENTICATE );
    m_vrHttpActions.push_back( cBlnsHttpAction::HA_PROCESSFINISHED );

    m_teBlnsHttpProcess = cBlnsHttpAction::HA_AUTHENTICATE;
    m_inHttpProcessStep = 0;

    _httpStartProcess();
}

//=================================================================================================
void cBlnsHttp::sendPatientCardData(QString p_qsBarcode, QString p_qsPatientCardData, bool p_bSendNow )
//-------------------------------------------------------------------------------------------------
{
    if( !m_bIsHttpEnabled )
    {
        m_qsError = tr("HTTP connection disabled.");
        m_inHttpProcessStep = HTTP_STATUS_DEFAULT;
        g_obLogger(cSeverity::WARNING) << "HTTP: abort sendPatientCardData - connection disabled" << EOM;
        emit signalErrorOccured();
        return;
    }

    try
    {
        QString  qsQuery;

        qsQuery = "INSERT INTO httppatientcardinfo SET ";
        qsQuery += QString( "licenceId = \"%1\", " ).arg( m_uiLicenceId );
        qsQuery += QString( "barcode = \"%1\", " ).arg( p_qsBarcode );
        qsQuery += QString( "patientcardInfoText = \"%1\", " ).arg( p_qsPatientCardData );
        qsQuery += QString( "active = 1, " );
        qsQuery += QString( "archive = \"NEW\" " );

        g_poDB->executeQTQuery( qsQuery );
    }
    catch( cSevException &e )
    {
        cerr << ">> " << e.what() << endl << flush;;
        g_obLogger(e.severity()) << e.what() << EOM;
        m_qsError = tr("Error occured during executing database command");
    }

    if( !p_bSendNow )
    {
        return;
    }

    m_vrHttpActions.clear();
    m_vrHttpActions.push_back( cBlnsHttpAction::HA_AUTHENTICATE );
    m_vrHttpActions.push_back( cBlnsHttpAction::HA_PCSENDDATA );
    m_vrHttpActions.push_back( cBlnsHttpAction::HA_PCUPDATERECORD );
    m_vrHttpActions.push_back( cBlnsHttpAction::HA_PROCESSFINISHED );

    m_teBlnsHttpProcess = cBlnsHttpAction::HA_PCSENDDATA;
    m_inHttpProcessStep = 0;

    _httpStartProcess();
}

//=================================================================================================
void cBlnsHttp::processWaitingCardData()
//-------------------------------------------------------------------------------------------------
{
    //cTracer obTrace( "cBlnsHttp::processWaitingCardData" );

    if( !m_bIsHttpEnabled )
    {
        m_qsError = tr("HTTP connection disabled");
        m_inHttpProcessStep = HTTP_STATUS_DEFAULT;
        g_obLogger(cSeverity::WARNING) << "HTTP: " << m_qsError << EOM;
        emit signalErrorOccured();
        return;
    }

    m_vrHttpActions.clear();
    m_vrHttpActions.push_back( cBlnsHttpAction::HA_AUTHENTICATE );
    m_vrHttpActions.push_back( cBlnsHttpAction::HA_PCSENDDATA );
    m_vrHttpActions.push_back( cBlnsHttpAction::HA_PCUPDATERECORD );
    m_vrHttpActions.push_back( cBlnsHttpAction::HA_PROCESSFINISHED );

    m_teBlnsHttpProcess = cBlnsHttpAction::HA_PCPROCESSQUEUE;
    m_inHttpProcessStep = 0;

    _httpStartProcess();
}

//=================================================================================================
void cBlnsHttp::getPatientCardsSoldOnline()
//-------------------------------------------------------------------------------------------------
{
    if( !m_bIsHttpEnabled )
    {
        m_qsError = tr("HTTP connection disabled");
        m_inHttpProcessStep = HTTP_STATUS_DEFAULT;
        g_obLogger(cSeverity::WARNING) << "HTTP: " << m_qsError << EOM;
        emit signalErrorOccured();
        return;
    }

    m_vrHttpActions.clear();
    m_vrHttpActions.push_back( cBlnsHttpAction::HA_AUTHENTICATE );
    m_vrHttpActions.push_back( cBlnsHttpAction::HA_REQUESTDATA );
    m_vrHttpActions.push_back( cBlnsHttpAction::HA_SENDREQUESTSFINISHED );
    m_vrHttpActions.push_back( cBlnsHttpAction::HA_PROCESSFINISHED );

    m_teBlnsHttpProcess     = cBlnsHttpAction::HA_REQUESTDATA;
    m_inHttpProcessStep     = 0;
    m_bGetOnlinePCProcessed = false;

    _httpStartProcess();
}

//=================================================================================================
void cBlnsHttp::processWaitingMails()
//-------------------------------------------------------------------------------------------------
{
    if( !m_bIsHttpEnabled )
    {
        m_qsError = tr("HTTP connection disabled");
        m_inHttpProcessStep = HTTP_STATUS_DEFAULT;
        g_obLogger(cSeverity::WARNING) << "HTTP: " << m_qsError << EOM;
        emit signalErrorOccured();
        return;
    }

    m_vrHttpActions.clear();
    m_vrHttpActions.push_back( cBlnsHttpAction::HA_AUTHENTICATE );
    m_vrHttpActions.push_back( cBlnsHttpAction::HA_SENDMAILTOSERVER );
    m_vrHttpActions.push_back( cBlnsHttpAction::HA_UPDATEMAILRECORD );
    m_vrHttpActions.push_back( cBlnsHttpAction::HA_PROCESSFINISHED );

    m_teBlnsHttpProcess     = cBlnsHttpAction::HA_MAILPROCESSQUEUE;
    m_inHttpProcessStep     = 0;

    _httpStartProcess();
}

//=================================================================================================
void cBlnsHttp::registerLicenceKey( QString p_qsLicenceString, QString p_qsClientCode )
//-------------------------------------------------------------------------------------------------
{
    if( !m_bIsHttpEnabled )
    {
        m_qsError = tr("HTTP connection disabled");
        m_inHttpProcessStep = HTTP_STATUS_DEFAULT;
        g_obLogger(cSeverity::WARNING) << "HTTP: " << m_qsError << EOM;
        emit signalErrorOccured();
        return;
    }

    m_qsLicenceStringNew    = p_qsLicenceString;
    m_qsLicenceClientCode   = p_qsClientCode;

    m_vrHttpActions.clear();
    m_vrHttpActions.push_back( cBlnsHttpAction::HA_AUTHENTICATE );
    m_vrHttpActions.push_back( cBlnsHttpAction::HA_LICENCE_REGISTER );
    m_vrHttpActions.push_back( cBlnsHttpAction::HA_PROCESSFINISHED );

    m_teBlnsHttpProcess     = cBlnsHttpAction::HA_LICENCE_REGISTER;
    m_inHttpProcessStep     = 0;

    _httpStartProcess();
}

//=================================================================================================
void cBlnsHttp::reactivateLicenceKey(QString p_qsLicenceString, QString p_qsClientCode )
//-------------------------------------------------------------------------------------------------
{
    if( !m_bIsHttpEnabled )
    {
        m_qsError = tr("HTTP connection disabled");
        m_inHttpProcessStep = HTTP_STATUS_DEFAULT;
        g_obLogger(cSeverity::WARNING) << "HTTP: " << m_qsError << EOM;
        emit signalErrorOccured();
        return;
    }

    m_qsLicenceStringCurrent= p_qsLicenceString;
    m_qsLicenceClientCode   = p_qsClientCode;

    m_vrHttpActions.clear();
    m_vrHttpActions.push_back( cBlnsHttpAction::HA_AUTHENTICATE );
    m_vrHttpActions.push_back( cBlnsHttpAction::HA_LICENCE_REACTIVATE );
    m_vrHttpActions.push_back( cBlnsHttpAction::HA_PROCESSFINISHED );

    m_teBlnsHttpProcess     = cBlnsHttpAction::HA_LICENCE_REACTIVATE;
    m_inHttpProcessStep     = 0;

    _httpStartProcess();
}

//=================================================================================================
void cBlnsHttp::validateLicenceKey(QString p_qsLicenceString, QString p_qsClientCode, QString p_qsServerCode)
//-------------------------------------------------------------------------------------------------
{
    if( !m_bIsHttpEnabled )
    {
        m_qsError = tr("HTTP connection disabled");
        m_inHttpProcessStep = HTTP_STATUS_DEFAULT;
        g_obLogger(cSeverity::WARNING) << "HTTP: " << m_qsError << EOM;
        emit signalErrorOccured();
        return;
    }

    m_qsLicenceStringCurrent= p_qsLicenceString;
    m_qsLicenceClientCode   = p_qsClientCode;
    m_qsLicenceServerCode   = p_qsServerCode;

    m_vrHttpActions.clear();
    m_vrHttpActions.push_back( cBlnsHttpAction::HA_AUTHENTICATE );
    m_vrHttpActions.push_back( cBlnsHttpAction::HA_LICENCE_CHECK );
    m_vrHttpActions.push_back( cBlnsHttpAction::HA_PROCESSFINISHED );

    m_teBlnsHttpProcess     = cBlnsHttpAction::HA_LICENCE_CHECK;
    m_inHttpProcessStep     = 0;

    _httpStartProcess();
}

//=================================================================================================
//=================================================================================================
//=================================================================================================
//
// GLOBAL functions for retrieving information from http processes
//
//=================================================================================================
//=================================================================================================
//=================================================================================================
int cBlnsHttp::getNumberOfWaitingRecords()
//-------------------------------------------------------------------------------------------------
{
    QString      qsQuery    = "SELECT COUNT(httpPatientcardInfoId) FROM "
                              "httppatientcardinfo WHERE "
                              "active=1 AND "
                              "archive='NEW' ";
    QSqlQuery   *poQuery    = g_poDB->executeQTQuery( qsQuery );

    poQuery->first();

    return poQuery->value(0).toInt();
}

//=================================================================================================
QString cBlnsHttp::errorMessage()
//-------------------------------------------------------------------------------------------------
{
    return m_qsError;
}

//=================================================================================================
QString cBlnsHttp::settingsInfo()
//-------------------------------------------------------------------------------------------------
{
    QString qsRet = "";

    qsRet.append( tr("<b>PatientCardType:</b><br>") );
    qsRet.append( tr("Id: %1  Price: %2  Units: %3  Unittime: %4<br>")
                    .arg( m_uiPatientCardTypeId )
                    .arg( m_nPatientCardTypePrice )
                    .arg( m_nPatientCardTypeUnits )
                    .arg( m_nPatientCardTypeUnitTime ) );
    qsRet.append( tr("<br>") );
    qsRet.append( tr("<b>Payment method:</b><br>") );
    qsRet.append( tr("Id: %1").arg( m_uiPaymentMethod ) );

    return qsRet;
}

//=================================================================================================
//=================================================================================================
//=================================================================================================
//
// GENERAL functions for managing http processes
//
//=================================================================================================
//=================================================================================================
//=================================================================================================
void cBlnsHttp::timerEvent(QTimerEvent *)
//-------------------------------------------------------------------------------------------------
{
    //cTracer obTrace( "cBlnsHttp::timerEvent" );

    killTimer( m_inTimer );
    m_inTimer = 0;

    m_qsError.append( tr("HTTP timeout occured.") );
    m_inHttpProcessStep = HTTP_STATUS_DEFAULT;
    g_obLogger(cSeverity::WARNING) << "HTTP: "
                                   << QString("%1 FAILED due to timeout.")
                                             .arg( cBlnsHttpAction::toStr( m_vrHttpActions.at( m_inHttpProcessStep ) ) )
                                   << EOM;
    emit signalErrorOccured();
}

//=================================================================================================
//=================================================================================================
//=================================================================================================
//
// HTTP process functions
//
//=================================================================================================
//=================================================================================================
//=================================================================================================
void cBlnsHttp::_httpStartProcess()
//-------------------------------------------------------------------------------------------------
{
    g_obLogger(cSeverity::DEBUG) << "HTTP: Start "
                                 << cBlnsHttpAction::toStr( m_teBlnsHttpProcess )
                                 << " - Step: ["
                                 << m_inHttpProcessStep
                                 << "]"
                                 << EOM;

    if( !m_bIsHttpEnabled )
    {
        g_obLogger(cSeverity::WARNING) << "HTTP: communication DISABLED" << EOM;
        emit signalHttpProcessDisabled();
        return;
    }

    if( m_bIsHttpSuspended )
    {
        g_obLogger(cSeverity::WARNING) << "HTTP: communication SUSPENDED" << EOM;
        emit signalHttpProcessSuspended();
        return;
    }

    if( m_vrHttpActions.size() < 1 )
    {
        g_obLogger(cSeverity::ERROR) << "HTTP: actions not prepared correctly" << EOM;
        return;
    }

    if( m_inHttpProcessStep > (int)(m_vrHttpActions.size()-1) ||
        m_inHttpProcessStep < 0 )
    {
        g_obLogger(cSeverity::ERROR) << "HTTP: invalid action step: ["
                                     << m_inHttpProcessStep
                                     << "]"
                                     << EOM;
        return;
    }

    if( m_teBlnsHttpProcess == cBlnsHttpAction::HA_PCSENDDATA ||
        m_teBlnsHttpProcess == cBlnsHttpAction::HA_PCPROCESSQUEUE )
    {
        g_obLogger(cSeverity::DEBUG) << "HTTP: Get first unsent record" << EOM;

        try
        {
            QString      qsQuery    = "SELECT httpPatientcardInfoId, barcode, patientcardInfoText FROM "
                                      "httppatientcardinfo WHERE "
                                      "active=1 AND "
                                      "archive='NEW' "
                                      "LIMIT 1 ";
            QSqlQuery   *poQuery    = g_poDB->executeQTQuery( qsQuery );

            if( poQuery->size() != 1 )
            {
                emit signalHideProgress( "No more patientcard info to process" );
                g_obLogger(cSeverity::DEBUG) << "HTTP no more record" << EOM;
                _updateProcessedRecord();
                return;
            }

            poQuery->first();

            m_uiRecordId = poQuery->value(0).toUInt();
            m_qsBarcode  = poQuery->value(1).toString();
            m_qsCardData = poQuery->value(2).toString();
        }
        catch( cSevException &e )
        {
            cerr << ">> " << e.what() << endl << flush;;
            g_obLogger(e.severity()) << e.what() << EOM;
            m_qsError = tr("Error occured during executing database command");
        }
    }

    if( m_teBlnsHttpProcess == cBlnsHttpAction::HA_SENDMAILTOSERVER ||
        m_teBlnsHttpProcess == cBlnsHttpAction::HA_MAILPROCESSQUEUE )
    {
        g_obLogger(cSeverity::DEBUG) << "HTTP: Get first unsent valid mail" << EOM;

        try
        {
            QString      qsQuery            = "SELECT * FROM "
                                              "httpsendmail WHERE "
                                              "dateOfSending<=\"" + QDate::currentDate().toString( "yyyy-MM-dd" ) + "\" AND "
                                              "active=1 AND "
                                              "( archive='NEW' OR archive='MOD' ) "
                                              "LIMIT 1 ";
            QSqlQuery   *poQuery            = g_poDB->executeQTQuery( qsQuery );
            QByteArray   qbaSha1Base        = "";
            QString      qsUnitIds          = "";

            if( poQuery->size() != 1 )
            {
                emit signalHideProgress( "No more email to send" );
                g_obLogger(cSeverity::DEBUG) << "HTTP no more mail" << EOM;
                _updateMailRecord();
                return;
            }

            poQuery->first();

            m_uiRecordId        = poQuery->value(0).toUInt();
            m_nMailTypeId       = poQuery->value(2).toInt();
            m_qsMailRecipients  = poQuery->value(4).toString();
            m_qsMailSubject     = poQuery->value(5).toString();
            m_qsMailText        = poQuery->value(6).toString();
            m_qsMailVarName     = poQuery->value(7).toString();
            m_qsMailVarBarcode  = poQuery->value(8).toString();
            m_qsMailVarCardInfo = poQuery->value(9).toString();
            m_qsMailVarUnitCount= poQuery->value(10).toString();
            m_qsMailVarDateTime = poQuery->value(11).toString();

            // ^[a-z0-9!#$%&\'*+\=?^_`{|}~-]+(?:\.[a-z0-9!#$%&\'*+\=?^_`{|}~-]+)*@(?:[a-z0-9](?:[a-z0-9-]*[a-z0-9])?\.)+[a-z0-9](?:[a-z0-9-]*[a-z0-9])?
            QRegExp qreEmail( "^[a-z0-9!#$%&\\'*+\\=?^_`{|}~-]+(?:\\.[a-z0-9!#$%&\\'*+\\=?^_`{|}~-]+)*@(?:[a-z0-9](?:[a-z0-9-]*[a-z0-9])?\\.)+[a-z0-9](?:[a-z0-9-]*[a-z0-9])?" );

            if( !qreEmail.exactMatch( m_qsMailRecipients ) )
            {
                g_obLogger( cSeverity::ERROR ) << "HTTP: Invalid email address: ["
                                               << m_qsMailRecipients
                                               << "]"
                                               << EOM;
                qsQuery = "UPDATE httpsendmail SET ";
                qsQuery += QString( "active = 0, " );
                qsQuery += QString( "archive = \"SENT\" " );
                qsQuery += QString( "WHERE httpSendMailId = \"%1\" " ).arg( m_uiRecordId );
                g_poDB->executeQTQuery( qsQuery );

                m_inHttpProcessStep = 2;
                _httpExecuteProcess();
                return;
            }

            if( m_nMailTypeId > 0 )
            {
                m_qsMailText = QString::number( m_nMailTypeId );
            }

            if( m_nMailTypeId == 3 )
            {
                qsUnitIds           = m_qsMailSubject;
                m_qsMailSubject     = "";
                unsigned int uiPCId = 0;

                qsQuery = QString( "SELECT * FROM patientcardunits WHERE patientCardUnitId IN (%1) AND active=1 " ).arg( qsUnitIds );
                poQuery = g_poDB->executeQTQuery( qsQuery );

                if( poQuery->size() > 0 )
                {
                    QString qsCardInfo = "";
                    poQuery->first();
                    uiPCId = poQuery->value( 2 ).toUInt();

                    QString qsQuery = QString( "SELECT patientCardUnitId, "
                                                      "patientCardTypeId, "
                                                      "unitTime, "
                                                      "validDateFrom, "
                                                      "validDateTo, "
                                                      "COUNT(unitTime) "
                                               "FROM patientcardunits "
                                               "WHERE patientCardId=%1 "
                                               "AND validDateFrom<=CURDATE() AND validDateTo>=CURDATE() "
                                               "AND prepared=0 "
                                               "AND active=1 "
                                               "GROUP BY unitTime, validDateTo, patientCardTypeId ORDER BY validDateTo, patientCardUnitId" )
                                             .arg( uiPCId );
                    QSqlQuery  *poQuery = g_poDB->executeQTQuery( qsQuery );

                    while( poQuery->next() )
                    {
                        QString qsValid = QString( "%1 -> %2" ).arg( poQuery->value( 3 ).toString() )
                                                               .arg( poQuery->value( 4 ).toString() );
                        unsigned int uiPCTId = poQuery->value( 1 ).toUInt();

                        if( uiPCTId > 0 )
                        {
                            qsCardInfo.append( poQuery->value( 5 ).toString() );
                            qsCardInfo.append( QObject::tr(" units (") );
                            qsCardInfo.append( poQuery->value( 2 ).toString() );
                            qsCardInfo.append( QObject::tr(" minutes) (") );
                            qsCardInfo.append( _getNameForPatientCardType( uiPCTId ) );
                            qsCardInfo.append( ") " );
                            qsCardInfo.append( qsValid );
                            qsCardInfo.append( "<br>" );
                        }
                    }
                    m_qsMailVarCardInfo = qsCardInfo;
                }
                else
                {
                    g_obLogger(cSeverity::DEBUG) << "HTTP Expiration mail is obsolete, no need to send" << EOM;

                    qsQuery = "UPDATE httpsendmail SET ";
                    qsQuery += QString( "active = 0, " );
                    qsQuery += QString( "archive = \"SENT\" " );
                    qsQuery += QString( "WHERE httpSendMailId = \"%1\" " ).arg( m_uiRecordId );
                    g_poDB->executeQTQuery( qsQuery );

                    m_inHttpProcessStep = 2;
                    _httpExecuteProcess();
                    return;
                }
            }

            qbaSha1Base.append( m_qsMailRecipients );
            qbaSha1Base.append( m_qsLicenceString );
            qbaSha1Base.append( m_qsMailVarBarcode );
            qbaSha1Base.append( m_qsMailVarUnitCount );
            qbaSha1Base.append( m_qsMailVarDateTime );
            qbaSha1Base.append( "belenusLetter" );

            m_qsMailSha1 = QString(QCryptographicHash::hash(qbaSha1Base,QCryptographicHash::Sha1).toHex());

            g_obLogger(cSeverity::DEBUG) << "qbaSha1Base:  [" << _bytearrayToString(qbaSha1Base) << "]" << EOM;
            g_obLogger(cSeverity::DEBUG) << "m_qsMailSha1: [" << m_qsMailSha1 << "]" << EOM;
        }
        catch( cSevException &e )
        {
            cerr << ">> " << e.what() << endl << flush;;
            g_obLogger(e.severity()) << e.what() << EOM;
            m_qsError = tr("Error occured during executing database command");
        }
    }

    _httpExecuteProcess();
}

//=================================================================================================
void cBlnsHttp::_httpExecuteProcess()
//-------------------------------------------------------------------------------------------------
{
    //cTracer obTrace( "cBlnsHttp::_httpExecuteProcess" );

    g_obLogger(cSeverity::DEBUG) << "HTTP: Execute process step ["
                                 << m_inHttpProcessStep
                                 << "]"
                                 << EOM;

    switch( m_vrHttpActions.at( m_inHttpProcessStep ) )
    {
        case cBlnsHttpAction::HA_AUTHENTICATE: // 1
        {
            g_obLogger(cSeverity::DEBUG) << "HTTP: Get token" << EOM;
            _httpGetToken();
            break;
        }
        case cBlnsHttpAction::HA_PCSENDDATA: // 2
        {
            g_obLogger(cSeverity::DEBUG) << "HTTP: Send card data" << EOM;
            _httpSendCardData();
            break;
        }
        case cBlnsHttpAction::HA_PCUPDATERECORD: // 3
        {
            g_obLogger(cSeverity::DEBUG) << "HTTP: Update record" << EOM;
            _updateProcessedRecord();
            _httpProcessResponse();
            break;
        }
        case cBlnsHttpAction::HA_REQUESTDATA: // 5
        {
            g_obLogger(cSeverity::DEBUG) << "HTTP: Get online sold cards" << EOM;
            _httpGetOnlineRecords();
            break;
        }
        case cBlnsHttpAction::HA_SENDREQUESTSFINISHED: // 6
        {
            g_obLogger(cSeverity::DEBUG) << "HTTP: Confirm requested data arrived" << EOM;
            _httpConfirmRequestedData();
            break;
        }
        case cBlnsHttpAction::HA_SENDMAILTOSERVER: // 7
        {
            g_obLogger(cSeverity::DEBUG) << "HTTP: Send mail to server" << EOM;
            _httpSendMailToServer();
            break;
        }
        case cBlnsHttpAction::HA_UPDATEMAILRECORD: // 9
        {
            g_obLogger(cSeverity::DEBUG) << "HTTP: Update mail record" << EOM;
            _updateMailRecord();
            _httpProcessResponse();
            break;
        }
        case cBlnsHttpAction::HA_LICENCE_REGISTER: // 10
        {
            g_obLogger(cSeverity::DEBUG) << "HTTP: Register licence key" << EOM;
            _httpRegisterLicence();
            break;
        }
        case cBlnsHttpAction::HA_LICENCE_REACTIVATE: // 11
        {
            g_obLogger(cSeverity::DEBUG) << "HTTP: Reactivate licence key" << EOM;
            _httpReactivateLicence();
            break;
        }
        case cBlnsHttpAction::HA_LICENCE_CHECK: // 12
        {
            g_obLogger(cSeverity::DEBUG) << "HTTP: Check licence key" << EOM;
            _httpCheckLicence();
            break;
        }
        case cBlnsHttpAction::HA_PROCESSFINISHED: // 13
        {
            g_obLogger(cSeverity::DEBUG) << "HTTP: Finish process" << EOM;
            _httpProcessResponse();
            break;
        }
        default:
            // Nothing to do
            break;
    }
}

//=================================================================================================
void cBlnsHttp::_httpGetToken()
//-------------------------------------------------------------------------------------------------
{
  cTracer obTrace( "cBlnsHttp::_httpGetToken" );

    // Empty token and request new from server
    m_qsToken = "";

    // http://www.kiwisun.hu/kiwi_ticket/token.php
    QString qsFileName = m_qsServerAddress;

    qsFileName.append( "/kiwi_ticket/token.php" );

    qsFileName = qsFileName.replace( "\\", "/" );
    qsFileName = qsFileName.replace( "//kiwi", "/kiwi" );

    g_obLogger(cSeverity::DEBUG) << "HTTP: Download ["
                                 << qsFileName
                                 << "]"
                                 << EOM;
    _downloadFile( qsFileName );
}

//=================================================================================================
void cBlnsHttp::_httpSendCardData()
//-------------------------------------------------------------------------------------------------
{
    // http://www.kiwisun.hu/kiwi_ticket/save.php
    QString qsFileName  = m_qsServerAddress;
    QString qsMd5Source = "";
    QString qsTimeStamp = QString::number( QDateTime::currentDateTime().toTime_t() );

    qsMd5Source.append( m_qsToken );
    qsMd5Source.append( qsTimeStamp );
    qsMd5Source.append( "kiwibérlet" );

    QString qsMd5Hash = QString(QCryptographicHash::hash(qsMd5Source.toStdString().c_str(),QCryptographicHash::Md5).toHex());

    qsFileName.append( "/kiwi_ticket/save.php" );

    qsFileName = qsFileName.replace( "\\", "/" );
    qsFileName = qsFileName.replace( "//kiwi", "/kiwi" );

    qsFileName.append( QString( "?token=%1" ).arg( m_qsToken ) );
    qsFileName.append( QString( "&stamp=%1" ).arg( qsTimeStamp ) );
    qsFileName.append( QString( "&code=%1" ).arg( qsMd5Hash ) );
    qsFileName.append( QString( "&StudioId=%1" ).arg( m_qsLicenceString ) );
    qsFileName.append( QString( "&CardId=%1" ).arg( m_qsBarcode ) );
    qsFileName.append( QString( "&CardData=%1" ).arg( m_qsCardData ) );
    if( m_qsCardData.length() == 0 )
    {
        qsFileName.append( QString( "&DelId=%1" ).arg( m_qsBarcode ) );
    }

    g_obLogger(cSeverity::DEBUG) << "HTTP: Send card data ["
                                 << qsFileName
                                 << "]"
                                 << EOM;

    _downloadFile( qsFileName );
}

//=================================================================================================
void cBlnsHttp::_httpSendMailToServer()
//-------------------------------------------------------------------------------------------------
{
    // https://www.kiwisun.hu/kiwi_ticket/belenusLetter.php
    QString qsFileName      = m_qsServerAddress;

    qsFileName.append( "/kiwi_ticket/belenusLetter.php" );

    qsFileName = qsFileName.replace( "\\", "/" );
    qsFileName = qsFileName.replace( "//kiwi", "/kiwi" );

    qsFileName.append( QString( "?token=%1" ).arg( m_qsToken ) );
    qsFileName.append( QString( "&StudioId=%1" ).arg( m_qsLicenceString ) );
    qsFileName.append( QString( "&email=%1" ).arg( m_qsMailRecipients ) );
    qsFileName.append( QString( "&subject=%1" ).arg( m_qsMailSubject ) );
    qsFileName.append( QString( "&text=%1" ).arg( m_qsMailText ) );
    qsFileName.append( QString( "&name=%1" ).arg( m_qsMailVarName ) );
    qsFileName.append( QString( "&cardId=%1" ).arg( m_qsMailVarBarcode ) );
    qsFileName.append( QString( "&cardInfo=%1" ).arg( m_qsMailVarCardInfo ) );
    qsFileName.append( QString( "&unitCount=%1" ).arg( m_qsMailVarUnitCount ) );
    qsFileName.append( QString( "&dateTime=%1" ).arg( m_qsMailVarDateTime ) );
    qsFileName.append( QString( "&code=%1" ).arg( m_qsMailSha1 ) );

    g_obLogger(cSeverity::DEBUG) << "HTTP: Send mail ["
                                 << qsFileName
                                 << "]"
                                 << EOM;

    _downloadFile( qsFileName );
}

//=================================================================================================
void cBlnsHttp::_updateProcessedRecord()
//-------------------------------------------------------------------------------------------------
{
    try
    {
        g_poDB->executeQTQuery( "DELETE FROM httppatientcardinfo WHERE active = 0 AND archive = \"ARC\" " );
    }
    catch( cSevException &e )
    {
        cerr << ">> " << e.what() << endl << flush;;
        g_obLogger(e.severity()) << e.what() << EOM;
        m_qsError = tr("Error occured during executing database command");
    }
}

//=================================================================================================
void cBlnsHttp::_updateMailRecord()
//-------------------------------------------------------------------------------------------------
{
    try
    {
        g_poDB->executeQTQuery( "DELETE FROM httpsendmail WHERE active = 0 AND archive = \"SENT\" " );
    }
    catch( cSevException &e )
    {
        cerr << ">> " << e.what() << endl << flush;;
        g_obLogger(e.severity()) << e.what() << EOM;
        m_qsError = tr("Error occured during executing database command");
    }
}

//=================================================================================================
void cBlnsHttp::_httpGetOnlineRecords()
//-------------------------------------------------------------------------------------------------
{
    // http://www.kiwisun.hu/kiwi_ticket/comm.php
    QString qsFileName  = m_qsServerAddress;
    QString qsSha1Source = "";

    qsSha1Source.append( m_qsToken );
    qsSha1Source.append( QString::number(m_uiCommId) );
    qsSha1Source.append( m_qsLicenceString );
    qsSha1Source.append( "gifter" );

    QString qsSha1Hash = QString(QCryptographicHash::hash(qsSha1Source.toStdString().c_str(),QCryptographicHash::Sha1).toHex());

    qsFileName.append( "/kiwi_ticket/comm.php" );

    qsFileName = qsFileName.replace( "\\", "/" );
    qsFileName = qsFileName.replace( "//kiwi", "/kiwi" );

    qsFileName.append( QString( "?token=%1" ).arg( m_qsToken ) );
    qsFileName.append( QString( "&code=%1" ).arg( qsSha1Hash ) );
    qsFileName.append( QString( "&StudioId=%1" ).arg( m_qsLicenceString ) );
    qsFileName.append( QString( "&CommId=%1" ).arg( m_uiCommId ) );

    g_obLogger(cSeverity::DEBUG) << "HTTP: Get online sold card data ["
                                 << qsFileName
                                 << "]"
                                 << EOM;

    _downloadFile( qsFileName );
}

//=================================================================================================
void cBlnsHttp::_httpConfirmRequestedData()
//-------------------------------------------------------------------------------------------------
{
    // http://www.kiwisun.hu/kiwi_ticket/comm.php
    QString qsFileName  = m_qsServerAddress;
    QString qsSha1Source = "";

    qsSha1Source.append( m_qsToken );
    qsSha1Source.append( QString::number(m_uiCommId) );
    qsSha1Source.append( m_qsLicenceString );
    qsSha1Source.append( "gifter" );

    QString qsSha1Hash = QString(QCryptographicHash::hash(qsSha1Source.toStdString().c_str(),QCryptographicHash::Sha1).toHex());

    qsFileName.append( "/kiwi_ticket/comm.php" );

    qsFileName = qsFileName.replace( "\\", "/" );
    qsFileName = qsFileName.replace( "//kiwi", "/kiwi" );

    qsFileName.append( QString( "?token=%1" ).arg( m_qsToken ) );
    qsFileName.append( QString( "&code=%1" ).arg( qsSha1Hash ) );
    qsFileName.append( QString( "&StudioId=%1" ).arg( m_qsLicenceString ) );
    qsFileName.append( QString( "&CommId=%1" ).arg( m_uiCommId ) );
    qsFileName.append( QString( "&Result=ok(%1)" ).arg( m_qslProcessedRecordIds.join(",") ) );

    g_obLogger(cSeverity::DEBUG) << "HTTP: Confirm online sold card data processed ["
                                 << qsFileName
                                 << "]"
                                 << EOM;

    _downloadFile( qsFileName );
}

//=================================================================================================
void cBlnsHttp::_httpRegisterLicence()
//-------------------------------------------------------------------------------------------------
{
    // https://www.kiwisun.hu/kiwi_ticket/licence.php
    QString qsFileName      = m_qsServerAddress;

    qsFileName.append( "/kiwi_ticket/licence.php" );

    qsFileName = qsFileName.replace( "\\", "/" );
    qsFileName = qsFileName.replace( "//kiwi", "/kiwi" );

    qsFileName.append( QString( "?token=%1" ).arg( m_qsToken ) );
    qsFileName.append( QString( "&Cmd=LICENCE_REGISTER" ) );
    qsFileName.append( QString( "&StudioId=%1" ).arg( m_qsLicenceStringNew ) );
    qsFileName.append( QString( "&ClientCode=%1" ).arg( m_qsLicenceClientCode ) );

    g_obLogger(cSeverity::DEBUG) << "HTTP: Register licence ["
                                 << qsFileName
                                 << "]"
                                 << EOM;

    _downloadFile( qsFileName );
}

//=================================================================================================
void cBlnsHttp::_httpReactivateLicence()
//-------------------------------------------------------------------------------------------------
{
    // https://www.kiwisun.hu/kiwi_ticket/licence.php
    QString qsFileName      = m_qsServerAddress;

    qsFileName.append( "/kiwi_ticket/licence.php" );

    qsFileName = qsFileName.replace( "\\", "/" );
    qsFileName = qsFileName.replace( "//kiwi", "/kiwi" );

    qsFileName.append( QString( "?token=%1" ).arg( m_qsToken ) );
    qsFileName.append( QString( "&Cmd=LICENCE_REACTIVATE" ) );
    qsFileName.append( QString( "&StudioId=%1" ).arg( m_qsLicenceStringCurrent ) );
    qsFileName.append( QString( "&ClientCode=%1" ).arg( m_qsLicenceClientCode ) );

    g_obLogger(cSeverity::DEBUG) << "HTTP: Reactivate licence ["
                                 << qsFileName
                                 << "]"
                                 << EOM;

    _downloadFile( qsFileName );
}

//=================================================================================================
void cBlnsHttp::_httpChangeLicence()
//-------------------------------------------------------------------------------------------------
{
    // https://www.kiwisun.hu/kiwi_ticket/licence.php
    QString qsFileName      = m_qsServerAddress;

    qsFileName.append( "/kiwi_ticket/licence.php" );

    qsFileName = qsFileName.replace( "\\", "/" );
    qsFileName = qsFileName.replace( "//kiwi", "/kiwi" );

    qsFileName.append( QString( "?token=%1" ).arg( m_qsToken ) );
    qsFileName.append( QString( "&Cmd=LICENCE_CHANGE" ) );
    qsFileName.append( QString( "&StudioId=%1" ).arg( m_qsLicenceStringCurrent ) );
    qsFileName.append( QString( "&StudioIdNew=%1" ).arg( m_qsLicenceStringNew ) );
    qsFileName.append( QString( "&ClientCode=%1" ).arg( m_qsLicenceClientCode ) );

    g_obLogger(cSeverity::DEBUG) << "HTTP: Change licence key ["
                                 << qsFileName
                                 << "]"
                                 << EOM;

    _downloadFile( qsFileName );
}

//=================================================================================================
void cBlnsHttp::_httpCheckLicence()
//-------------------------------------------------------------------------------------------------
{
    // https://www.kiwisun.hu/kiwi_ticket/licence.php
    QString qsFileName      = m_qsServerAddress;

    qsFileName.append( "/kiwi_ticket/licence.php" );

    qsFileName = qsFileName.replace( "\\", "/" );
    qsFileName = qsFileName.replace( "//kiwi", "/kiwi" );

    qsFileName.append( QString( "?token=%1" ).arg( m_qsToken ) );
    qsFileName.append( QString( "&Cmd=LICENCE_CHECK" ) );
    qsFileName.append( QString( "&StudioId=%1" ).arg( m_qsLicenceStringCurrent ) );
    qsFileName.append( QString( "&ClientCode=%1" ).arg( m_qsLicenceClientCode ) );
    qsFileName.append( QString( "&ServerCode=%1" ).arg( m_qsLicenceServerCode ) );

    g_obLogger(cSeverity::DEBUG) << "HTTP: Send mail ["
                                 << qsFileName
                                 << "]"
                                 << EOM;

    _downloadFile( qsFileName );
}


//=================================================================================================
void cBlnsHttp::_httpProcessResponse()
//-------------------------------------------------------------------------------------------------
{
    cTracer obTrace( "cBlnsHttp::_httpProcessResponse" );

    g_obLogger(cSeverity::DEBUG) << "HTTP: Process response for step ["
                                 << m_inHttpProcessStep
                                 << "]"
                                 << EOM;

    switch( m_vrHttpActions.at( m_inHttpProcessStep ) )
    {
        case cBlnsHttpAction::HA_AUTHENTICATE:
            g_obLogger(cSeverity::DEBUG) << "HTTP: Read token from token.php" << EOM;
            _readTokenFromFile();
            if( m_qsToken.length() != 16 )
            {
                m_qsError = tr("Invalid token received: ");
                m_qsError.append( m_qsToken );
                g_obLogger(cSeverity::ERROR) << "HTTP: "
                                               << QString("received token is not valid: [%1].")
                                                         .arg( m_qsToken )
                                               << EOM;
                emit signalErrorOccured();
                m_inHttpProcessStep = HTTP_ERROR_INVALID_TOKEN;
                return;
            }
            m_inHttpProcessStep++;
            _httpExecuteProcess();
            break;

        case cBlnsHttpAction::HA_PCSENDDATA:
            g_obLogger(cSeverity::DEBUG) << "HTTP: Read response from save.php" << EOM;
            _readPCResponseFromFile();
            if( m_inHttpProcessStep > 0 ) // csak akkor lep tovabb, ha volt authentikacio
            {
                m_inHttpProcessStep++;
                _httpExecuteProcess();
            }
            break;

        case cBlnsHttpAction::HA_PCUPDATERECORD:
            g_obLogger(cSeverity::DEBUG) << "HTTP: Nothing to process after update" << EOM;
            m_inHttpProcessStep++;
            _httpExecuteProcess();
            break;

        case cBlnsHttpAction::HA_SENDMAILTOSERVER:
            g_obLogger(cSeverity::DEBUG) << "HTTP: Read response from belenusLetter.php" << EOM;
            _readMailResponseFromFile();
            if( m_inHttpProcessStep > 0 )
            {
                m_inHttpProcessStep++;
                _httpExecuteProcess();
            }
            break;

        case cBlnsHttpAction::HA_UPDATEMAILRECORD:
            g_obLogger(cSeverity::DEBUG) << "HTTP: Nothing to process after update" << EOM;
            m_inHttpProcessStep++;
            _httpExecuteProcess();
            break;

        case cBlnsHttpAction::HA_REQUESTDATA:
            g_obLogger(cSeverity::DEBUG) << "HTTP: Process received comm.php with data" << EOM;
            if( !_processCommXML() )
            {
                g_obLogger(cSeverity::ERROR) << "Error occured during processing comm.php ErrorCode: " << m_inHttpProcessStep << EOM;
                return;
            }
            m_inHttpProcessStep++;
            _httpExecuteProcess();
            break;

        case cBlnsHttpAction::HA_SENDREQUESTSFINISHED:
            g_obLogger(cSeverity::DEBUG) << "HTTP: Nothing to do after confirm" << EOM;
            if( !_processResponse() )
            {
                g_obLogger(cSeverity::ERROR) << "Error occured during processing comm.php ErrorCode: " << m_inHttpProcessStep << EOM;
                return;
            }
            m_inHttpProcessStep++;
            _httpExecuteProcess();
            break;

        case cBlnsHttpAction::HA_LICENCE_REGISTER:
            g_obLogger(cSeverity::DEBUG) << "HTTP: Read register response from licence.php" << EOM;
            if( !_processLicence() )
            {
                g_obLogger(cSeverity::ERROR) << "Error occured during processing licence.php ErrorCode: " << m_inHttpProcessStep << EOM;
                return;
            }
            m_inHttpProcessStep++;
            _httpExecuteProcess();
            break;

        case cBlnsHttpAction::HA_LICENCE_REACTIVATE:
            g_obLogger(cSeverity::DEBUG) << "HTTP: Read reactivate response from licence.php" << EOM;
            if( !_processLicence() )
            {
                g_obLogger(cSeverity::ERROR) << "Error occured during processing licence.php ErrorCode: " << m_inHttpProcessStep << EOM;
                return;
            }
            m_inHttpProcessStep++;
            _httpExecuteProcess();
            break;

        case cBlnsHttpAction::HA_LICENCE_CHECK:
            g_obLogger(cSeverity::DEBUG) << "HTTP: Read check response from licence.php" << EOM;
            if( !_processLicence() )
            {
                g_obLogger(cSeverity::ERROR) << "Error occured during processing licence.php ErrorCode: " << m_inHttpProcessStep << EOM;
                return;
            }
            m_inHttpProcessStep++;
            _httpExecuteProcess();
            break;

        case cBlnsHttpAction::HA_PROCESSFINISHED:
            g_obLogger(cSeverity::DEBUG) << "HTTP: Process finished. Reply to main process" << EOM;
            _sendProcessFinished();
            break;

        default:
            // Nothing to do
            break;
    }
}












//=================================================================================================
bool cBlnsHttp::_downloadFile(QString p_qsFileName)
//-------------------------------------------------------------------------------------------------
{
    QUrl        url( p_qsFileName );
    QFileInfo   fileInfo(url.path());

    if( fileInfo.fileName().isEmpty() )
        return false;

    g_obLogger(cSeverity::DEBUG) << "HTTP: "
                                 << "host ["
                                 << url.host()
                                 << "] path ["
                                 << url.path()
                                 << "]"
                                 << EOM;

    QString     fileName = QString("%1\\%2").arg(QDir::currentPath()).arg( fileInfo.fileName() );

    if( QFile::exists(fileName) )
    {
        QFile::remove(fileName);
    }

    obFile = new QFile(fileName);

    if( !obFile->open(QIODevice::WriteOnly) )
    {
        m_qsError = tr("Unable to save HTTP communication file.");
        g_obLogger(cSeverity::WARNING) << "HTTP: "
                                       << QString("Unable to save the file\n\n%1\n\n%2.")
                                                 .arg( fileName )
                                                 .arg( obFile->errorString() )
                                       << EOM;
        emit signalErrorOccured();
        delete obFile;
        obFile = 0;
        return false;
    }

    QHttp::ConnectionMode mode = url.scheme().toLower() == "https" ? QHttp::ConnectionModeHttps : QHttp::ConnectionModeHttp;
    obHttp->setHost(url.host(), mode, url.port() == -1 ? 0 : url.port());

    // http://username:password@example.com/filename.ext
    if (!url.userName().isEmpty())
        obHttp->setUser(url.userName(), url.password());

    m_httpRequestAborted = false;
    QByteArray path = QUrl::toPercentEncoding(url.path(), "!$&'()*+,;=:@/");
    if (path.isEmpty())
        path = "/";

    QString qsDownload = QString( path );

    if( url.hasQuery() )
    {
        qsDownload.append( "?" );
        qsDownload.append( QString( url.encodedQuery() ) );
    }

    m_httpGetId = obHttp->get(qsDownload, obFile);
    m_inTimer   = startTimer( m_inTimeout );

    return true;
}

//=================================================================================================
void cBlnsHttp::_readTokenFromFile()
//-------------------------------------------------------------------------------------------------
{
  cTracer obTrace( "cBlnsHttp::_readTokenFromFile" );

    QString fileName = QString("%1\\token.php").arg( QDir::currentPath() );
    QFile   file( fileName );

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        g_obLogger(cSeverity::ERROR) << "HTTP Unable to open file " << fileName << EOM;
        return;
    }

    QTextStream qtsFile(&file);
    QString     qsLine = qtsFile.readLine();

    m_qsToken = qsLine.left( 16 );
}

//=================================================================================================
void cBlnsHttp::_sendProcessFinished()
//-------------------------------------------------------------------------------------------------
{
    g_obLogger(cSeverity::DEBUG) << "HTTP: ["
                                 << cBlnsHttpAction::toStr( m_teBlnsHttpProcess )
                                 << "] last step finished" << EOM;

    switch( m_teBlnsHttpProcess )
    {
        case cBlnsHttpAction::HA_AUTHENTICATE:
            emit signalActionProcessed( QString("%1 succeeded (%2)")
                                                .arg( cBlnsHttpAction::toStr( m_teBlnsHttpProcess ) )
                                                .arg( m_qsToken ) );
            break;

        case cBlnsHttpAction::HA_PCSENDDATA:
            emit signalActionProcessed( QString("%1 succeeded (%2)")
                                                .arg( cBlnsHttpAction::toStr( m_teBlnsHttpProcess ) )
                                                .arg( m_qsBarcode ) );
            break;

        case cBlnsHttpAction::HA_PCPROCESSQUEUE:
            emit signalActionProcessed( QString("%1 succeeded (%2)")
                                                .arg( cBlnsHttpAction::toStr( m_teBlnsHttpProcess ) )
                                                .arg( m_qsBarcode ) );
            emit signalStepProgress();
            break;

        case cBlnsHttpAction::HA_SENDMAILTOSERVER:
            emit signalActionProcessed( QString("%1 succeeded")
                                                .arg( cBlnsHttpAction::toStr( m_teBlnsHttpProcess ) ) );
            break;

        case cBlnsHttpAction::HA_MAILPROCESSQUEUE:
            emit signalActionProcessed( QString("%1 succeeded")
                                                .arg( cBlnsHttpAction::toStr( m_teBlnsHttpProcess ) ) );
            emit signalStepProgress();
            break;

        case cBlnsHttpAction::HA_REQUESTDATA:
            emit signalActionProcessed( QString("%1 succeeded")
                                                .arg( cBlnsHttpAction::toStr( m_teBlnsHttpProcess ) ));
            break;

        case cBlnsHttpAction::HA_LICENCE_REGISTER:
            emit signalActionProcessed( QString("%1 succeeded")
                                                .arg( cBlnsHttpAction::toStr( m_teBlnsHttpProcess ) ));
            break;

        case cBlnsHttpAction::HA_LICENCE_REACTIVATE:
            emit signalActionProcessed( QString("%1 succeeded")
                                                .arg( cBlnsHttpAction::toStr( m_teBlnsHttpProcess ) ));
            break;

        case cBlnsHttpAction::HA_LICENCE_CHECK:
            emit signalActionProcessed( QString("%1 succeeded")
                                                .arg( cBlnsHttpAction::toStr( m_teBlnsHttpProcess ) ));
            break;

        default:
            break;
    }
}

//=================================================================================================
void cBlnsHttp::_readPCResponseFromFile()
//-------------------------------------------------------------------------------------------------
{
    QString fileName = QString("%1\\save.php").arg( QDir::currentPath() );
    QFile   file( fileName );

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        g_obLogger(cSeverity::ERROR) << "HTTP Unable to open file " << fileName << EOM;
        return;
    }

    QTextStream qtsFile(&file);
    QString     qsLine = qtsFile.readLine();

    g_obLogger(cSeverity::DEBUG) << "HTTP: Response is ["
                                 << qsLine
                                 << "]"
                                 << EOM;

    if( qsLine.contains( "true" ) || qsLine.contains( "Missing Card" ) )
    {
        g_obLogger(cSeverity::DEBUG) << "HTTP: Update record with id ["
                                     << m_uiRecordId
                                     << "]"
                                     << EOM;

        try
        {
            QString  qsQuery;

            qsQuery = "UPDATE httppatientcardinfo SET ";
            qsQuery += QString( "active = 0, " );
            qsQuery += QString( "archive = \"ARC\" " );
            qsQuery += QString( "WHERE httpPatientcardInfoId = \"%1\" " ).arg( m_uiRecordId );

            m_uiRecordId = 0;
            g_poDB->executeQTQuery( qsQuery );
        }
        catch( cSevException &e )
        {
            cerr << ">> " << e.what() << endl << flush;;
            g_obLogger(e.severity()) << e.what() << EOM;
            m_qsError = tr("Error occured during executing database command");
        }
    }
    else if( qsLine.contains( "false" ) )
    {
        m_qsError = tr("Unknown error occured on server side.");
        g_obLogger(cSeverity::WARNING) << "HTTP: Unknown error occured on server side." << EOM;
        emit signalErrorOccured();
        m_inHttpProcessStep = HTTP_ERROR_UNKNOWN;
    }
    else if( qsLine.contains( "Wrong token" ) )
    {
        m_qsError = tr("HTTP Session expired");
        g_obLogger(cSeverity::WARNING) << "HTTP: Token not accepted by server" << EOM;
        emit signalErrorOccured();
        m_inHttpProcessStep = HTTP_ERROR_WRONG_TOKEN;
    }
    else if( qsLine.contains( "Check error" ) )
    {
        m_qsError = tr("HTTP security check failed");
        g_obLogger(cSeverity::WARNING) << "HTTP: MD5 hash not accepted by server" << EOM;
        emit signalErrorOccured();
        m_inHttpProcessStep = HTTP_ERROR_MD5_MISMATCH;
    }
    else if( qsLine.contains( "StudioId unknown" ) )
    {
        m_qsError = tr("Unknown studio Id");
        g_obLogger(cSeverity::WARNING) << "HTTP: Studio Id not accepted by server" << EOM;
        emit signalErrorOccured();
        m_inHttpProcessStep = HTTP_ERROR_INVALID_STUDIO;
    }
    else if( qsLine.contains( "SQL error" ) )
    {
        m_qsError = tr("Database error occured on server side");
        g_obLogger(cSeverity::WARNING) << "HTTP: Server was unable to execute SQL command" << EOM;
        emit signalErrorOccured();
        m_inHttpProcessStep = HTTP_ERROR_SERVER_SQL;
    }
}

//=================================================================================================
void cBlnsHttp::_readMailResponseFromFile()
//-------------------------------------------------------------------------------------------------
{
    QString fileName = QString("%1\\belenusLetter.php").arg( QDir::currentPath() );
    QFile   file( fileName );

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        g_obLogger(cSeverity::ERROR) << "HTTP Unable to open file " << fileName << EOM;
        return;
    }

    QTextStream qtsFile(&file);
    QString     qsLine = qtsFile.readLine();

    g_obLogger(cSeverity::DEBUG) << "HTTP: Response is ["
                                 << qsLine
                                 << "]"
                                 << EOM;

    if( qsLine.contains( "MailSent" ) )
    {
        g_obLogger(cSeverity::DEBUG) << "HTTP: Update record with id ["
                                     << m_uiRecordId
                                     << "]"
                                     << EOM;

        try
        {
            QString  qsQuery;

            qsQuery = "UPDATE httpsendmail SET ";
            qsQuery += QString( "active = 0, " );
            qsQuery += QString( "archive = \"SENT\" " );
            qsQuery += QString( "WHERE httpSendMailId = \"%1\" " ).arg( m_uiRecordId );

            m_uiRecordId = 0;
            g_poDB->executeQTQuery( qsQuery );
        }
        catch( cSevException &e )
        {
            cerr << ">> " << e.what() << endl << flush;;
            g_obLogger(e.severity()) << e.what() << EOM;
            m_qsError = tr("Error occured during executing database command");
        }
    }
    else
    {
        g_obLogger(cSeverity::ERROR) << "HTTP: Mark error record with id ["
                                     << m_uiRecordId
                                     << "]"
                                     << EOM;

        try
        {
            QString  qsQuery;

            qsQuery = "UPDATE httpsendmail SET ";
            qsQuery += QString( "active = 0, " );
            qsQuery += QString( "archive = \"ERROR\" " );
            qsQuery += QString( "WHERE httpSendMailId = \"%1\" " ).arg( m_uiRecordId );

            m_uiRecordId = 0;
            g_poDB->executeQTQuery( qsQuery );
        }
        catch( cSevException &e )
        {
            cerr << ">> " << e.what() << endl << flush;;
            g_obLogger(e.severity()) << e.what() << EOM;
            m_qsError = tr("Error occured during executing database command");
        }

        if( qsLine.contains( "false" ) )
        {
            m_qsError = tr("Unknown error occured on server side.");
            g_obLogger(cSeverity::WARNING) << "HTTP: Unknown error occured on server side." << EOM;
            emit signalErrorOccured();
            m_inHttpProcessStep = HTTP_ERROR_UNKNOWN;
        }
        else if( qsLine.contains( "SQL error" ) )
        {
            m_qsError = tr("Database error occured on server side");
            g_obLogger(cSeverity::WARNING) << "HTTP: Server was unable to execute SQL command" << EOM;
            emit signalErrorOccured();
            m_inHttpProcessStep = HTTP_ERROR_SERVER_SQL;
        }
        else if( qsLine.contains( "Token repeat" ) )
        {
            m_qsError = tr("Token already used before");
            g_obLogger(cSeverity::WARNING) << "HTTP: Token used in a previous communication" << EOM;
            emit signalErrorOccured();
            m_inHttpProcessStep = HTTP_ERROR_TOKEN_REPEAT;
        }
        else if( qsLine.contains( "Old token" ) )
        {
            m_qsError = tr("Token expired");
            g_obLogger(cSeverity::WARNING) << "HTTP: Token expired" << EOM;
            emit signalErrorOccured();
            m_inHttpProcessStep = HTTP_ERROR_TOKEN_EXPIRED;
        }
        else if( qsLine.contains( "Wrong token" ) )
        {
            m_qsError = tr("HTTP Session expired");
            g_obLogger(cSeverity::WARNING) << "HTTP: Token not accepted by server" << EOM;
            emit signalErrorOccured();
            m_inHttpProcessStep = HTTP_ERROR_WRONG_TOKEN;
        }
        else if( qsLine.contains( "Check error" ) )
        {
            m_qsError = tr("HTTP security check failed");
            g_obLogger(cSeverity::WARNING) << "HTTP: MD5 hash not accepted by server" << EOM;
            emit signalErrorOccured();
            m_inHttpProcessStep = HTTP_ERROR_MD5_MISMATCH;
        }
        else if( qsLine.contains( "Missing code" ) )
        {
            m_qsError = tr("HTTP Missing sha1 code");
            g_obLogger(cSeverity::WARNING) << "HTTP: Missing sha1 code" << EOM;
            emit signalErrorOccured();
            m_inHttpProcessStep = HTTP_ERROR_SHA1_NOT_RECEIVED;
        }
        else if( qsLine.contains( "Missing StudioId" ) )
        {
            m_qsError = tr("HTTP Missing Studio identifier");
            g_obLogger(cSeverity::WARNING) << "HTTP: Missing Studio identifier" << EOM;
            emit signalErrorOccured();
            m_inHttpProcessStep = HTTP_ERROR_MISSING_STUDIOID;
        }
        else if( qsLine.contains( "Missing email" ) )
        {
            m_qsError = tr("HTTP Missing mail recipient");
            g_obLogger(cSeverity::WARNING) << "HTTP: Missing mail recipient" << EOM;
            emit signalErrorOccured();
            m_inHttpProcessStep = HTTP_ERROR_MISSING_MAIL_RECIP;
        }
        else if( qsLine.contains( "Missing subject" ) )
        {
            m_qsError = tr("HTTP Missing mail subject");
            g_obLogger(cSeverity::WARNING) << "HTTP: Missing mail subject" << EOM;
            emit signalErrorOccured();
            m_inHttpProcessStep = HTTP_ERROR_MISSING_MAIL_SUBJ;
        }
        else if( qsLine.contains( "Missing text" ) )
        {
            m_qsError = tr("HTTP Missing mail body");
            g_obLogger(cSeverity::WARNING) << "HTTP: Missing mail body" << EOM;
            emit signalErrorOccured();
            m_inHttpProcessStep = HTTP_ERROR_MISSING_MAIL_BODY;
        }
        else if( qsLine.contains( "Missing name" ) )
        {
            m_qsError = tr("HTTP Missing mail variable NAME");
            g_obLogger(cSeverity::WARNING) << "HTTP: Missing mail variable NAME" << EOM;
            emit signalErrorOccured();
            m_inHttpProcessStep = HTTP_ERROR_MISSING_MAIL_VAR_NAME;
        }
        else if( qsLine.contains( "Missing cardId" ) )
        {
            m_qsError = tr("HTTP Missing mail variable BARCODE");
            g_obLogger(cSeverity::WARNING) << "HTTP: Missing mail variable BARCODE" << EOM;
            emit signalErrorOccured();
            m_inHttpProcessStep = HTTP_ERROR_MISSING_MAIL_VAR_BARCODE;
        }
        else if( qsLine.contains( "Missing cardInfo" ) )
        {
            m_qsError = tr("HTTP Missing mail variable CARDINFO");
            g_obLogger(cSeverity::WARNING) << "HTTP: Missing mail variable CARDINFO" << EOM;
            emit signalErrorOccured();
            m_inHttpProcessStep = HTTP_ERROR_MISSING_MAIL_VAR_CARDINFO;
        }
        else if( qsLine.contains( "Missing unitCount" ) )
        {
            m_qsError = tr("HTTP Missing mail variable UNITCOUNT");
            g_obLogger(cSeverity::WARNING) << "HTTP: Missing mail variable UNITCOUNT" << EOM;
            emit signalErrorOccured();
            m_inHttpProcessStep = HTTP_ERROR_MISSING_MAIL_VAR_UNITCOUNT;
        }
        else if( qsLine.contains( "Missing dateTime" ) )
        {
            m_qsError = tr("HTTP Missing mail variable DATETIME");
            g_obLogger(cSeverity::WARNING) << "HTTP: Missing mail variable DATETIME" << EOM;
            emit signalErrorOccured();
            m_inHttpProcessStep = HTTP_ERROR_MISSING_MAIL_VAR_DATETIME;
        }
        else if( qsLine.contains( "Invalid e-mail address" ) )
        {
            m_qsError = tr("HTTP Invalid e-mail address");
            g_obLogger(cSeverity::WARNING) << "HTTP: Invalid e-mail address" << EOM;
            emit signalErrorOccured();
            m_inHttpProcessStep = HTTP_ERROR_INVALID_EMAIL_ADDRESS;
        }
    }
}

//=================================================================================================
bool cBlnsHttp::_processCommXML()
//-------------------------------------------------------------------------------------------------
{
    QString fileName = QString("%1\\comm.php").arg( QDir::currentPath() );
    QFile   file( fileName );

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        g_obLogger(cSeverity::ERROR) << "HTTP Unable to open file " << fileName << EOM;
        return false;
    }

    QString     qsSha1Sent      = file.readLine();
    QByteArray  qbaSha1Base     = file.readLine();
    int         nXmlLength      = qbaSha1Base.length();

    if( qbaSha1Base.contains( "<response>" ) )
    {
        qbaSha1Base.append( m_qsToken );
        qbaSha1Base.append( "gifter" );

        QString     qsSha1Gen = QString(QCryptographicHash::hash(qbaSha1Base,QCryptographicHash::Sha1).toHex());

        g_obLogger(cSeverity::DEBUG) << "Base:      [" << _bytearrayToString(qbaSha1Base) << "]" << EOM;
        g_obLogger(cSeverity::DEBUG) << "Sha1 sent: [" << qsSha1Sent.left(40)  << "]" << EOM;
        g_obLogger(cSeverity::DEBUG) << "Sha1 gen.: [" << qsSha1Gen  << "]" << EOM;

        if( qsSha1Gen.compare( qsSha1Sent.left(40) ) != 0 )
        {
            m_qsError = tr("SHA1 code mismatch in comm response");
            g_obLogger(cSeverity::WARNING) << "HTTP: SHA1 code mismatch" << EOM;
            emit signalErrorOccured();
            m_inHttpProcessStep = HTTP_ERROR_SHA1_MISMATCH;
            return false;
        }

        return _processCommResponse( qbaSha1Base.left(nXmlLength) );
    }
    else if( qsSha1Sent.contains( "Missing token" ) )
    {
        m_qsError = tr("Server did not received token");
        g_obLogger(cSeverity::WARNING) << "HTTP: Server did not received token" << EOM;
        emit signalErrorOccured();
        m_inHttpProcessStep = HTTP_ERROR_INVALID_TOKEN;
    }
    else if( qsSha1Sent.contains( "Missing code" ) )
    {
        m_qsError = tr("Server did not received Sha1 hash");
        g_obLogger(cSeverity::WARNING) << "HTTP: Server did not received Sha1 hash" << EOM;
        emit signalErrorOccured();
        m_inHttpProcessStep = HTTP_ERROR_SHA1_NOT_RECEIVED;
    }
    else if( qsSha1Sent.contains( "Missing StudioId" ) )
    {
        m_qsError = tr("Server did not received Studio Id");
        g_obLogger(cSeverity::WARNING) << "HTTP: Server did not received Studio Id" << EOM;
        emit signalErrorOccured();
        m_inHttpProcessStep = HTTP_ERROR_MISSING_STUDIOID;
    }
    else if( qsSha1Sent.contains( "Missing CommId" ) )
    {
        m_qsError = tr("Server did not received last successfull request Id");
        g_obLogger(cSeverity::WARNING) << "HTTP: Server did not received last successfull request Id" << EOM;
        emit signalErrorOccured();
        m_inHttpProcessStep = HTTP_ERROR_MISSING_COMMID;
    }
    else if( qsSha1Sent.contains( "Wrong token" ) )
    {
        m_qsError = tr("Token sent to server is invalid");
        g_obLogger(cSeverity::WARNING) << "HTTP: Token sent to server is invalid" << EOM;
        emit signalErrorOccured();
        m_inHttpProcessStep = HTTP_ERROR_INVALID_TOKEN;
    }
    else if( qsSha1Sent.contains( "SQL error" ) )
    {
        m_qsError = tr("SQL error occured on server side");
        g_obLogger(cSeverity::WARNING) << "HTTP: SQL error occured on server side" << EOM;
        emit signalErrorOccured();
        m_inHttpProcessStep = HTTP_ERROR_SERVER_SQL;
    }
    else if( qsSha1Sent.contains( "Token repeat" ) )
    {
        m_qsError = tr("Token sent to server is already sent");
        g_obLogger(cSeverity::WARNING) << "HTTP: Token sent to server is already sent" << EOM;
        emit signalErrorOccured();
        m_inHttpProcessStep = HTTP_ERROR_OBSOLETE_TOKEN;
    }
    else if( qsSha1Sent.contains( "Old token" ) )
    {
        m_qsError = tr("Token sent to server is obsolete");
        g_obLogger(cSeverity::WARNING) << "HTTP: Token sent to server is obsolete" << EOM;
        emit signalErrorOccured();
        m_inHttpProcessStep = HTTP_ERROR_OBSOLETE_TOKEN;
    }
    else if( qsSha1Sent.contains( "Check error" ) )
    {
        m_qsError = tr("Sha1 code sent to server is invalid");
        g_obLogger(cSeverity::WARNING) << "HTTP: Sha1 code sent to server is invalid" << EOM;
        emit signalErrorOccured();
        m_inHttpProcessStep = HTTP_ERROR_SHA1_MISMATCH;
    }
    else if( qsSha1Sent.contains( "No events" ) )
    {
        m_qsError = tr("");
        g_obLogger(cSeverity::INFO) << "HTTP: No new patientcard has been sold" << EOM;
        return true;
    }

    return false;
}

//=================================================================================================
bool cBlnsHttp::_processCommResponse(QByteArray p_qsResponse )
//-------------------------------------------------------------------------------------------------
{
    QString      qsErrorMsg  = "";
    int          inErrorLine = 0;
    int          inErrorCol = 0;

    if( !obResponseXML->setContent( p_qsResponse, &qsErrorMsg, &inErrorLine, &inErrorCol ) )
    {
        g_obLogger(cSeverity::ERROR) << "HTTP: Response XML format error. ["
                                     << qsErrorMsg
                                     << "] ["
                                     << inErrorLine
                                     << ","
                                     << inErrorCol
                                     << "]"
                                     << EOM;
        return false;
    }

    QDomElement     docRoot     = obResponseXML->documentElement();
    int             nCountCards = docRoot.elementsByTagName( "cardNo" ).at(0).toElement().text().toInt();
    QDomNodeList    obCards     = docRoot.elementsByTagName( "cards" ).at(0).toElement().elementsByTagName( "card" );

    m_uiCommIdNew   = docRoot.elementsByTagName( "newCommId" ).at(0).toElement().text().toUInt();

    g_obLogger(cSeverity::DEBUG) << "HTTP: New id ["
                                 << m_uiCommIdNew
                                 << "] cardNo ["
                                 << nCountCards
                                 << "] Cards count ["
                                 << obCards.count()
                                 << "]"
                                 << EOM;

    if( obCards.count() != nCountCards )
    {
        g_obLogger(cSeverity::ERROR) << "HTTP: Response XML data count mismatch."
                                     << "Count ["
                                     << obCards.count()
                                     << "]"
                                     << EOM;
        return false;
    }

    m_qslProcessedRecordIds = QStringList();

    for( int i=0; i<obCards.count(); i++)
    {
        m_qsDisplayMessage = "";

        unsigned int    uiPatientId = 0;
        unsigned int    uiCardId    = 0;
        unsigned int    uiLedgerId  = 0;

        QString qsRecordId = obCards.at(i).toElement().elementsByTagName("recordId").at(0).toElement().text();
        QString qsBarcode = obCards.at(i).toElement().elementsByTagName("BarCode").at(0).toElement().text();
        QString qsUnitCount = obCards.at(i).toElement().elementsByTagName("OccasionNumber").at(0).toElement().text();
        QString qsGuestUniqueId = obCards.at(i).toElement().elementsByTagName("RedemptionCode").at(0).toElement().text();
        QString qsCardValidDate = obCards.at(i).toElement().elementsByTagName("ExpirationDate").at(0).toElement().text();
        QString qsCardSellDate = obCards.at(i).toElement().elementsByTagName("OrderDate").at(0).toElement().text();
        QString qsCardOwnerName = obCards.at(i).toElement().elementsByTagName("CardOwnerName").at(0).toElement().text();
        QString qsCardOwnerEmail = obCards.at(i).toElement().elementsByTagName("CardOwnerEmail").at(0).toElement().text();

        uiPatientId = _saveGuest( qsCardOwnerName, qsGuestUniqueId, qsCardOwnerEmail );

        if( uiPatientId > 0 )
        {
            uiCardId = _savePatientCard( qsBarcode, qsCardValidDate, qsUnitCount, uiPatientId );

            if( uiCardId > 0 )
            {
                uiLedgerId = _saveOnlineSell( uiCardId, qsBarcode, uiPatientId, qsCardSellDate );
                _savePatientCardUnits( qsUnitCount, uiCardId, qsCardValidDate, uiLedgerId );
                emit signalPatientCardUpdated( uiCardId, qsBarcode );
                if( m_qsDisplayMessage.length() > 0 ) m_qsDisplayMessage.append( "\n" );
                m_qsDisplayMessage.append( tr("Patientcard record for '%1' updated\n%2")
                                           .arg(qsBarcode)
                                           .arg(m_qsLedgerComment) );
            }
        }
        emit signalDisplayNotification( m_qsDisplayMessage );
        m_qslProcessedRecordIds.append( qsRecordId );
    }
    return true;
}

//=================================================================================================
unsigned int cBlnsHttp::_saveGuest(QString p_qsName, QString p_qsUniqueId, QString p_qsEmail)
//-------------------------------------------------------------------------------------------------
{
    QString         qsQuery = "";
    unsigned int    uiId    = 0;

    try
    {
        qsQuery = QString( "SELECT patientId FROM patients WHERE uniqueId=\"%1\" " ).arg( p_qsUniqueId );

        QSqlQuery   *poQuery = g_poDB->executeQTQuery( qsQuery );

        if( poQuery->size() > 0 )
        {
            poQuery->first();
            uiId = poQuery->value( 0 ).toUInt();

            qsQuery = "UPDATE patients SET ";
            qsQuery += QString( "name = \"%1\", " ).arg( p_qsName );
            qsQuery += QString( "email = \"%1\", " ).arg( p_qsEmail );
            qsQuery += QString( "modified = \"%1\", " ).arg( QDateTime::currentDateTime().toString( QString("yyyy-MM-dd hh:mm:ss") ) );
            qsQuery += QString( "archive = \"MOD\" " );
            qsQuery += QString( "WHERE patientId = \"%1\" " ).arg( uiId );

            poQuery = g_poDB->executeQTQuery( qsQuery );

            m_qsDisplayMessage.append( tr("Data updated for guest:\n%1").arg(p_qsName) );
        }
        else
        {
            qsQuery = "INSERT INTO patients SET ";
            qsQuery += QString( "licenceId = \"%1\", " ).arg( m_uiLicenceId );
            qsQuery += QString( "companyId = \"0\", " );
            qsQuery += QString( "created = \"%1\", " ).arg( QDateTime::currentDateTime().toString( QString("yyyy-MM-dd hh:mm:ss") ) );
            qsQuery += QString( "name = \"%1\", " ).arg( p_qsName );
            qsQuery += QString( "gender = \"0\", " );
            qsQuery += QString( "ageType = \"0\", " );
            qsQuery += QString( "isReturning = 0, " );
            qsQuery += QString( "uniqueId = \"%1\", " ).arg( p_qsUniqueId );
            qsQuery += QString( "email = \"%1\", " ).arg( p_qsEmail );
            qsQuery += QString( "regularCustomer = 0, " );
            qsQuery += QString( "employee = 0, " );
            qsQuery += QString( "service = 0, " );
            qsQuery += QString( "company = 0, " );
            qsQuery += QString( "discountType = 0, " );
            qsQuery += QString( "membership = \"\", " );
            qsQuery += QString( "dateBirth = \"\", " );
            qsQuery += QString( "address = \"\", " );
            qsQuery += QString( "skinTypeId = 0, " );
            qsQuery += QString( "mobile = \"\", " );
            qsQuery += QString( "comment = \"\", " );
            qsQuery += QString( "loyaltyPoints = 0, " );
            qsQuery += QString( "modified = \"%1\", " ).arg( QDateTime::currentDateTime().toString( QString("yyyy-MM-dd hh:mm:ss") ) );
            qsQuery += QString( "active = 1, " );
            qsQuery += QString( "archive = \"NEW\" " );

            poQuery = g_poDB->executeQTQuery( qsQuery );
            uiId = poQuery->lastInsertId().toUInt();

            m_qsDisplayMessage.append( tr("New guest data record created:\n%1").arg(p_qsName) );
        }
    }
    catch( cSevException &e )
    {
        cerr << ">> " << e.what() << endl << flush;;
        g_obLogger(e.severity()) << e.what() << EOM;
        m_qsError = tr("Error occured during executing database command");
    }

    return uiId;
}

//=================================================================================================
unsigned int cBlnsHttp::_savePatientCard(QString p_qsBarcode, QString p_qsValidDateTo, QString p_qsUnitCount, unsigned int p_uiPatientId)
//-------------------------------------------------------------------------------------------------
{
    QString         qsQuery = "";
    unsigned int    uiId    = 0;

    try
    {
        qsQuery = QString( "SELECT patientCardId FROM patientCards WHERE barcode=\"%1\" " ).arg( p_qsBarcode );

        QSqlQuery   *poQuery = g_poDB->executeQTQuery( qsQuery );

        if( poQuery->size() > 0 )
        {
            poQuery->first();
            uiId = poQuery->value( 0 ).toUInt();

            qsQuery = "UPDATE patientCards SET ";
            qsQuery += QString( "patientCardTypeId = \"%1\", " ).arg( m_uiPatientCardTypeId );
            qsQuery += QString( "patientId = \"%1\", " ).arg( p_uiPatientId );
            qsQuery += QString( "validDateTo = \"%1\", " ).arg( p_qsValidDateTo );
            qsQuery += QString( "modified = \"%1\", " ).arg( QDateTime::currentDateTime().toString( QString("yyyy-MM-dd hh:mm:ss") ) );
            qsQuery += QString( "active = 1, " );
            qsQuery += QString( "archive = \"MOD\" " );
            qsQuery += QString( "WHERE patientCardId = %1" ).arg( uiId );

            poQuery = g_poDB->executeQTQuery( qsQuery );
            m_uiLedgerTypeId = 8;
            m_qsLedgerComment = tr("Patientcard refilled online");
        }
        else
        {
            qsQuery = "INSERT INTO patientCards SET ";
            qsQuery += QString( "licenceId = \"%1\", " ).arg( m_uiLicenceId );
            qsQuery += QString( "patientCardTypeId = \"%1\", " ).arg( m_uiPatientCardTypeId );
            qsQuery += QString( "parentCardId = \"0\", " );
            qsQuery += QString( "patientId = \"%1\", " ).arg( p_uiPatientId );
            qsQuery += QString( "barcode = \"%1\", " ).arg( p_qsBarcode );
            qsQuery += QString( "comment = \"\", " );
            qsQuery += QString( "units = \"%1\", " ).arg( p_qsUnitCount.toInt() );
            qsQuery += QString( "amount = \"0\", " );
            qsQuery += QString( "timeLeft = \"0\", " );
            qsQuery += QString( "validDateFrom = \"%1\", " ).arg( QDateTime::currentDateTime().toString( QString("yyyy-MM-dd hh:mm:ss") ) );
            qsQuery += QString( "validDateTo = \"%1\", " ).arg( p_qsValidDateTo );
            qsQuery += QString( "pincode = \"\", " );
            qsQuery += QString( "modified = \"%1\", " ).arg( QDateTime::currentDateTime().toString( QString("yyyy-MM-dd hh:mm:ss") ) );
            qsQuery += QString( "active = 1, " );
            qsQuery += QString( "archive = \"NEW\" " );

            poQuery = g_poDB->executeQTQuery( qsQuery );
            m_uiLedgerTypeId = 7;
            m_qsLedgerComment = tr("Patientcard sold online");

            uiId = poQuery->lastInsertId().toUInt();
        }
    }
    catch( cSevException &e )
    {
        cerr << ">> " << e.what() << endl << flush;;
        g_obLogger(e.severity()) << e.what() << EOM;
        m_qsError = tr("Error occured during executing database command");
    }

    return uiId;
}

//=================================================================================================
unsigned int cBlnsHttp::_saveOnlineSell(unsigned int p_uiPatientCardId, QString p_qsBarcode, unsigned int p_uiPatientId, QString p_qsLedgerTime)
//-------------------------------------------------------------------------------------------------
{
    QString         qsQuery = "";
    QSqlQuery      *poQuery = NULL;
    unsigned int    uiId    = 0;

    try
    {
        unsigned int    uiCassaId = 0;

        poQuery = g_poDB->executeQTQuery( QString( "SELECT value FROM settings WHERE identifier=\"CURRENT_CASSA_ID\" " ) );
        if( poQuery->first() )
        {
            uiCassaId = poQuery->value( 0 ).toUInt();
        }

        qsQuery = "INSERT INTO ledger SET ";
        qsQuery += QString( "licenceId = \"%1\", " ).arg( m_uiLicenceId );
        qsQuery += QString( "parentId = \"0\", " );
        qsQuery += QString( "ledgerTypeId = \"%1\", " ).arg( m_uiLedgerTypeId );
        qsQuery += QString( "ledgerDeviceId = \"0\", " );
        qsQuery += QString( "paymentMethodId = \"%1\", " ).arg( m_uiPaymentMethod );
        qsQuery += QString( "userId = \"1\", " );
        qsQuery += QString( "productId = \"0\", " );
        qsQuery += QString( "patientCardTypeId = \"%1\", " ).arg( m_uiPatientCardTypeId );
        qsQuery += QString( "patientCardId = \"%1\", " ).arg( p_uiPatientCardId );
        qsQuery += QString( "panelId = \"0\", " );
        qsQuery += QString( "patientId = \"%1\", " ).arg( p_uiPatientId );
        qsQuery += QString( "name = \"%1\", " ).arg( p_qsBarcode );
        qsQuery += QString( "itemCount = \"1\", " );
        qsQuery += QString( "netPrice = \"0\", " );
        qsQuery += QString( "card = \"0\", " );
        qsQuery += QString( "cash = \"0\", " );
        qsQuery += QString( "voucher = \"0\", " );
        qsQuery += QString( "discount = \"0\", " );
        qsQuery += QString( "vatpercent = \"0\", " );
        qsQuery += QString( "totalPrice = \"0\", " );
        qsQuery += QString( "comment = \"%1\", " ).arg( m_qsLedgerComment );
        qsQuery += QString( "modified = \"%1\", " ).arg( p_qsLedgerTime );
        qsQuery += QString( "active = 1, " );
        qsQuery += QString( "archive = \"NEW\" " );

        poQuery = g_poDB->executeQTQuery( qsQuery );
        uiId = poQuery->lastInsertId().toUInt();

        qsQuery = "INSERT INTO cassaHistory SET ";
        qsQuery += QString( "licenceId = \"%1\", " ).arg( m_uiLicenceId );
        qsQuery += QString( "parentId = \"0\", " );
        qsQuery += QString( "cassaId = \"%1\", " ).arg( uiCassaId );
        qsQuery += QString( "ledgerId = \"%1\", " ).arg( uiId );
        qsQuery += QString( "userId = \"1\", " );   // Currently SystemAdministrator
        qsQuery += QString( "patientId = \"%1\", " ).arg( p_uiPatientId );
        qsQuery += QString( "actionValue = \"0\", " );
        qsQuery += QString( "actionCard = \"0\", " );
        qsQuery += QString( "actionCash = \"0\", " );
        qsQuery += QString( "actionBalance = \"0\", " );
        qsQuery += QString( "comment = \"%1\", " ).arg( m_qsLedgerComment );
        qsQuery += QString( "modified = \"%1\", " ).arg( QDateTime::currentDateTime().toString( QString("yyyy-MM-dd hh:mm:ss") ) );
        qsQuery += QString( "active = 1, " );
        qsQuery += QString( "archive = \"NEW\" " );

        poQuery = g_poDB->executeQTQuery( qsQuery );
    }
    catch( cSevException &e )
    {
        cerr << ">> " << e.what() << endl << flush;;
        g_obLogger(e.severity()) << e.what() << EOM;
        m_qsError = tr("Error occured during executing database command");
    }

    return uiId;
}

//=================================================================================================
void cBlnsHttp::_savePatientCardUnits(QString p_qsUnitCount, unsigned int p_uiPatientCardId, QString p_qsValidDateTo, unsigned int p_uiLedgerId)
//-------------------------------------------------------------------------------------------------
{
    try
    {
        for( int i=0; i<p_qsUnitCount.toInt(); i++ )
        {
            QString         qsQuery = "";

            qsQuery = "INSERT INTO patientCardUnits SET ";
            qsQuery += QString( "licenceId = \"%1\", " ).arg( m_uiLicenceId );
            qsQuery += QString( "patientCardId = \"%1\", " ).arg( p_uiPatientCardId );
            qsQuery += QString( "patientCardTypeId = \"%1\", " ).arg( m_uiPatientCardTypeId );
            qsQuery += QString( "ledgerId = \"%1\", " ).arg( p_uiLedgerId );
            qsQuery += QString( "panelId = \"0\", " );
            qsQuery += QString( "unitTime = \"%1\", " ).arg( m_nPatientCardTypeUnitTime );
            qsQuery += QString( "unitPrice = \"%1\", " ).arg( m_nPatientCardTypePrice / m_nPatientCardTypeUnits );
            qsQuery += QString( "validDateFrom = \"%1\", " ).arg( QDateTime::currentDateTime().toString( QString("yyyy-MM-dd hh:mm:ss") ) );
            qsQuery += QString( "validDateTo = \"%1\", " ).arg( p_qsValidDateTo );
            qsQuery += QString( "dateTimeUsed = \"0000-00-00 00:00:00\", " );
            qsQuery += QString( "prepared = 0, " );
            qsQuery += QString( "active = 1, " );
            qsQuery += QString( "archive = \"NEW\" " );

            g_poDB->executeQTQuery( qsQuery );
        }
    }
    catch( cSevException &e )
    {
        cerr << ">> " << e.what() << endl << flush;;
        g_obLogger(e.severity()) << e.what() << EOM;
        m_qsError = tr("Error occured during executing database command");
    }
}

//=================================================================================================
QString cBlnsHttp::_getNameForPatientCardType( unsigned int p_uiPatientCardTypeId )
//-------------------------------------------------------------------------------------------------
{
    QString      qsQuery = QString( "SELECT name FROM `patientcardtypes` WHERE patientCardTypeId=%1 " ).arg( p_uiPatientCardTypeId );
    QSqlQuery   *poQuery = g_poDB->executeQTQuery( qsQuery );

    poQuery->first();

    return poQuery->value( 0 ).toString();
}

//=================================================================================================
bool cBlnsHttp::_processResponse()
//-------------------------------------------------------------------------------------------------
{
    QString fileName = QString("%1\\comm.php").arg( QDir::currentPath() );
    QFile   file( fileName );

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        g_obLogger(cSeverity::ERROR) << "HTTP Unable to open file " << fileName << EOM;
        return false;
    }

    QTextStream qtsFile(&file);

    QString     qsResponse   = qtsFile.readLine();

    g_obLogger(cSeverity::DEBUG) << "HTTP: Response [" << qsResponse << "]" << EOM;

    if( qsResponse.contains( "ok -" ) )
    {
        g_obLogger(cSeverity::INFO) << "HTTP: Response received without error" << EOM;
        return true;
    }
    else if( qsResponse.contains( "Missing token" ) )
    {
        m_qsError = tr("Server did not received token");
        g_obLogger(cSeverity::WARNING) << "HTTP: Server did not received token" << EOM;
        emit signalErrorOccured();
        m_inHttpProcessStep = HTTP_ERROR_INVALID_TOKEN;
    }
    else if( qsResponse.contains( "Missing code" ) )
    {
        m_qsError = tr("Server did not received Sha1 hash");
        g_obLogger(cSeverity::WARNING) << "HTTP: Server did not received Sha1 hash" << EOM;
        emit signalErrorOccured();
        m_inHttpProcessStep = HTTP_ERROR_SHA1_NOT_RECEIVED;
    }
    else if( qsResponse.contains( "Missing StudioId" ) )
    {
        m_qsError = tr("Server did not received Studio Id");
        g_obLogger(cSeverity::WARNING) << "HTTP: Server did not received Studio Id" << EOM;
        emit signalErrorOccured();
        m_inHttpProcessStep = HTTP_ERROR_MISSING_STUDIOID;
    }
    else if( qsResponse.contains( "Missing CommId" ) )
    {
        m_qsError = tr("Server did not received last successfull request Id");
        g_obLogger(cSeverity::WARNING) << "HTTP: Server did not received last successfull request Id" << EOM;
        emit signalErrorOccured();
        m_inHttpProcessStep = HTTP_ERROR_MISSING_COMMID;
    }
    else if( qsResponse.contains( "Wrong token" ) )
    {
        m_qsError = tr("Token sent to server is invalid");
        g_obLogger(cSeverity::WARNING) << "HTTP: Token sent to server is invalid" << EOM;
        emit signalErrorOccured();
        m_inHttpProcessStep = HTTP_ERROR_INVALID_TOKEN;
    }
    else if( qsResponse.contains( "Check error" ) )
    {
        m_qsError = tr("Sha1 code sent to server is invalid");
        g_obLogger(cSeverity::WARNING) << "HTTP: Sha1 code sent to server is invalid" << EOM;
        emit signalErrorOccured();
        m_inHttpProcessStep = HTTP_ERROR_SHA1_MISMATCH;
    }
    else if( qsResponse.contains( "False" ) )
    {
        m_qsError = tr("Result not sent to server");
        g_obLogger(cSeverity::WARNING) << "HTTP: Result not sent to server" << EOM;
        emit signalErrorOccured();
        m_inHttpProcessStep = HTTP_ERROR_RESULT_NOT_SENT;
    }

    return false;
}

//=================================================================================================
bool cBlnsHttp::_processLicence()
//-------------------------------------------------------------------------------------------------
{
    QString fileName = QString("%1\\licence.php").arg( QDir::currentPath() );
    QFile   file( fileName );

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        g_obLogger(cSeverity::ERROR) << "HTTP Unable to open file " << fileName << EOM;
        return false;
    }

    QTextStream qtsFile(&file);

    QString     qsResponse   = qtsFile.readLine();

    g_obLogger(cSeverity::DEBUG) << "HTTP: Response [" << qsResponse << "]" << EOM;

    if( qsResponse.contains( "LICENCE_REGISTRATION_OK" ) ||
        qsResponse.contains( "LICENCE_ACTIVATION_OK" ) )
    {
        g_obLogger(cSeverity::INFO) << "HTTP: Response received without error" << EOM;
        m_qsLicenceServerCode = qsResponse.right( qsResponse.length() - qsResponse.indexOf( '-' ) - 2 );
        return true;
    }
    else if( qsResponse.contains( "LICENCE_OK" ) )
    {
        g_obLogger(cSeverity::INFO) << "HTTP: Response received without error" << EOM;
        return true;
    }
    else if( qsResponse.contains( "Missing token" ) )
    {
        m_qsError = tr("Server did not received token");
        g_obLogger(cSeverity::WARNING) << "HTTP: Server did not received token" << EOM;
        emit signalErrorOccured();
        m_inHttpProcessStep = HTTP_ERROR_INVALID_TOKEN;
    }
    else if( qsResponse.contains( "LICENCE_INVALID" ) )
    {
        m_qsError = tr("Licence code not found in server database");
        g_obLogger(cSeverity::WARNING) << "HTTP: Server did not received token" << EOM;
        emit signalErrorOccured();
        m_inHttpProcessStep = HTTP_ERROR_LICENCE_INVALID;
    }
    else if( qsResponse.contains( "LICENCE_ALREADY_REGISTERED" ) )
    {
        m_qsError = tr("Server did not received token");
        g_obLogger(cSeverity::WARNING) << "HTTP: Server did not received token" << EOM;
        emit signalErrorOccured();
        m_inHttpProcessStep = HTTP_ERROR_LICENCE_ALREADY_REGISTERED;
    }
    else if( qsResponse.contains( "LICENCE_INACTIVE" ) )
    {
        m_qsError = tr("Server did not received token");
        g_obLogger(cSeverity::WARNING) << "HTTP: Server did not received token" << EOM;
        emit signalErrorOccured();
        m_inHttpProcessStep = HTTP_ERROR_LICENCE_INACTIVE;
    }
    else if( qsResponse.contains( "LICENCE_CLIENT_CODE_INVALID" ) )
    {
        m_qsError = tr("Server did not received token");
        g_obLogger(cSeverity::WARNING) << "HTTP: Server did not received token" << EOM;
        emit signalErrorOccured();
        m_inHttpProcessStep = HTTP_ERROR_LICENCE_CLIENT_CODE_INVALID;
    }
    else if( qsResponse.contains( "LICENCE_SERVER_CODE_INVALID" ) )
    {
        m_qsError = tr("Server did not received token");
        g_obLogger(cSeverity::WARNING) << "HTTP: Server did not received token" << EOM;
        emit signalErrorOccured();
        m_inHttpProcessStep = HTTP_ERROR_LICENCE_SERVER_CODE_INVALID;
    }
    else
    {
        m_qsError = tr("Response from server is invalid or other unknown error occured");
        g_obLogger(cSeverity::WARNING) << "HTTP: Response from server is invalid or other unknown error occured" << EOM;
        emit signalErrorOccured();
        m_inHttpProcessStep = HTTP_ERROR_INVALID_ANSWER;
    }

    return false;
}

//=================================================================================================
QString cBlnsHttp::_bytearrayToString(QString p_qsString)
//-------------------------------------------------------------------------------------------------
{
    return p_qsString;
}

//=================================================================================================
//=================================================================================================
//=================================================================================================
//
// HTTP slots
//
//=================================================================================================
//=================================================================================================
//=================================================================================================
// _slotHttpRequestFinished
//-------------------------------------------------------------------------------------------------
void cBlnsHttp::_slotHttpRequestFinished(int requestId, bool error)
{
    killTimer( m_inTimer );
    m_inTimer = 0;

    if (requestId != m_httpGetId) return;

    if (m_httpRequestAborted)
    {
        if (obFile)
        {
            obFile->close();
            obFile->remove();
            delete obFile;
            obFile = 0;
        }
        g_obLogger(cSeverity::ERROR) << "HTTP Request aborted" << EOM;
        m_qsError.append( tr( "Http request aborted" ) );
        return;
    }

    if (requestId != m_httpGetId) return;

    obFile->close();

    if (error)
    {
        obFile->remove();
        m_qsError = tr("Error occured during downloading HTTP file.");
        g_obLogger(cSeverity::WARNING) << "HTTP: "
                                       << QString("Error occured during downloading file: %1.")
                                                 .arg( obHttp->errorString() )
                                       << EOM;
        emit signalErrorOccured();
        delete obFile;
        obFile = 0;
        g_obLogger(cSeverity::ERROR) << "HTTP Error occured" << EOM;
        m_qsError.append( tr( "Http error occured: %1" ).arg( obHttp->errorString() ) );
        return;
    }

    delete obFile;
    obFile = 0;

    _httpProcessResponse();
}

//=================================================================================================
void cBlnsHttp::_slotUpdateDataReadProgress(int /*bytesRead*/, int /*totalBytes*/)
//-------------------------------------------------------------------------------------------------
{
    if (m_httpRequestAborted)
        return;

//    _progressMax( totalBytes );
//    _progressValue( bytesRead );
}

//=================================================================================================
void cBlnsHttp::_slotReadResponseHeader(const QHttpResponseHeader &responseHeader)
//-------------------------------------------------------------------------------------------------
{
    switch (responseHeader.statusCode())
    {
        case 200:                   // Ok
        case 301:                   // Moved Permanently
        case 302:                   // Found
        case 303:                   // See Other
        case 307:                   // Temporary Redirect
            // these are not error conditions
            break;

        default:
//            m_teProcessStep = ST_WAIT;
//            QMessageBox::warning( this, tr("Warning"),
//                                  tr("Download failed: %1.").arg( responseHeader.reasonPhrase() ) );
//            m_httpRequestAborted = true;
//            obHttp->abort();
//            m_teProcessStep = ST_EXIT;
//            m_nTimer = startTimer( CONST_PROCESS_STEP_WAIT_MS );
            ;
    }
}

//=================================================================================================
void cBlnsHttp::_slotAuthenticationRequired(const QString &/*hostName*/, quint16, QAuthenticator */*authenticator*/)
//-------------------------------------------------------------------------------------------------
{/*
    QDialog dlg;
    Ui::Dialog ui;
    ui.setupUi(&dlg);
    dlg.adjustSize();
    ui.siteDescription->setText(tr("%1 at %2").arg(authenticator->realm()).arg(hostName));

    if (dlg.exec() == QDialog::Accepted)
    {
        authenticator->setUser(ui.userEdit->text());
        authenticator->setPassword(ui.passwordEdit->text());
    }*/
}

#ifndef QT_NO_OPENSSL

//=================================================================================================
void cBlnsHttp::_slotSslErrors(const QList<QSslError> &/*errors*/)
//-------------------------------------------------------------------------------------------------
{
/*
    QString errorString;
    foreach( const QSslError &error, errors )
    {
        if (!errorString.isEmpty())
            errorString += ", ";
        errorString += error.errorString();
    }

    if( QMessageBox::warning( this, tr("Warning"),
                             tr("One or more SSL errors has occurred: %1").arg(errorString),
                             QMessageBox::Ignore | QMessageBox::Abort) == QMessageBox::Ignore)
    {
*/
        obHttp->ignoreSslErrors();
//    }
}
#endif


