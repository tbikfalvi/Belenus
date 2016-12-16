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

#include "http.h"
//#include "belenus.h"
#include "..\client\licenceManager.h"
#include "dlgmain.h"

//=================================================================================================
cBlnsHttp::cBlnsHttp()
//-------------------------------------------------------------------------------------------------
{
    //cTracer obTrace( "cBlnsHttp::cBlnsHttp" );

    obHttp                  = NULL;
    obFile                  = NULL;
    m_httpRequestAborted    = false;
    m_httpGetId             = 0;

    m_qsError               = "";
    m_inTimeout             = 0;
    m_inTimer               = 0;
    m_qsToken               = "";
    m_inHttpProcessStep     = HTTP_STATUS_DEFAULT;
    m_teBlnsHttpProcess     = cBlnsHttpAction::HA_DEFAULT;

    m_qsServerAddress       = "0.0.0.0";
    m_bIsHttpEnabled        = true;
    m_bIsHttpSuspended      = false;
    m_uiLicenceId           = 0;
    m_qsLicenceString       = "BLNS_SERIAL_DEMO";

    m_uiCommId              = 0;

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
void cBlnsHttp::setStudioLicenceString( QString p_qsLicenceString )
//-------------------------------------------------------------------------------------------------
{
    m_qsLicenceString = p_qsLicenceString;
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

    QString  qsQuery;

    qsQuery = "INSERT INTO httppatientcardinfo SET ";
    qsQuery += QString( "licenceId = \"%1\", " ).arg( m_uiLicenceId );
    qsQuery += QString( "barcode = \"%1\", " ).arg( p_qsBarcode );
    qsQuery += QString( "patientcardInfoText = \"%1\", " ).arg( p_qsPatientCardData );
    qsQuery += QString( "active = 1, " );
    qsQuery += QString( "archive = \"NEW\" " );

    g_poDB->executeQTQuery( qsQuery );

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

    m_teBlnsHttpProcess = cBlnsHttpAction::HA_PROCESSQUEUE;
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

    m_teBlnsHttpProcess = cBlnsHttpAction::HA_AUTHENTICATE;
    m_inHttpProcessStep = 0;

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
    g_obLogger(cSeverity::DEBUG) << "HTTP: Start HTTP process ["
                                 << cBlnsHttpAction::toStr( m_teBlnsHttpProcess )
                                 << "] Step: ["
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
        m_teBlnsHttpProcess == cBlnsHttpAction::HA_PROCESSQUEUE )
    {
        g_obLogger(cSeverity::DEBUG) << "HTTP: Get first unsent record" << EOM;

        QString      qsQuery    = "SELECT httpPatientcardInfoId, barcode, patientcardInfoText FROM "
                                  "httppatientcardinfo WHERE "
                                  "active=1 AND "
                                  "archive='NEW' "
                                  "LIMIT 1 ";
        QSqlQuery   *poQuery    = g_poDB->executeQTQuery( qsQuery );

        if( poQuery->size() != 1 )
        {
            g_obLogger(cSeverity::DEBUG) << "HTTP no more record" << EOM;
            _updateProcessedRecord();
            emit signalHideProgress();
            return;
        }

        poQuery->first();

        m_uiRecordId = poQuery->value(0).toUInt();
        m_qsBarcode  = poQuery->value(1).toString();
        m_qsCardData = poQuery->value(2).toString();
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
        case cBlnsHttpAction::HA_AUTHENTICATE:
        {
            g_obLogger(cSeverity::DEBUG) << "HTTP: Get token" << EOM;
            _httpGetToken();
            break;
        }
        case cBlnsHttpAction::HA_PCSENDDATA:
            g_obLogger(cSeverity::DEBUG) << "HTTP: Send card data" << EOM;
            _httpSendCardData();
            break;

        case cBlnsHttpAction::HA_PCUPDATERECORD:
            g_obLogger(cSeverity::DEBUG) << "HTTP: Update record" << EOM;
            _updateProcessedRecord();
            _httpProcessResponse();
            break;

        case cBlnsHttpAction::HA_REQUESTDATA:
            g_obLogger(cSeverity::DEBUG) << "HTTP: Get online sold cards" << EOM;
            _httpGetOnlineRecords();
            break;

        case cBlnsHttpAction::HA_SENDREQUESTSFINISHED:
            g_obLogger(cSeverity::DEBUG) << "HTTP: Confirm requested data arrived" << EOM;
            _httpConfirmRequestedData();
            break;

        case cBlnsHttpAction::HA_PROCESSFINISHED:
            g_obLogger(cSeverity::DEBUG) << "HTTP: Finish process" << EOM;
            _httpProcessResponse();
            break;

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
void cBlnsHttp::_updateProcessedRecord()
//-------------------------------------------------------------------------------------------------
{
    g_poDB->executeQTQuery( "DELETE FROM httppatientcardinfo WHERE active = 0 AND archive = \"ARC\" " );
}

//=================================================================================================
void cBlnsHttp::_httpGetOnlineRecords()
//-------------------------------------------------------------------------------------------------
{
    // http://www.kiwisun.hu/kiwi_ticket/save.php
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
            _readResponseFromFile();
            if( m_inHttpProcessStep > 0 )
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

        case cBlnsHttpAction::HA_REQUESTDATA:
            g_obLogger(cSeverity::DEBUG) << "HTTP: Process received comm.php and confirm data arrived" << EOM;
            if( !_processCommXML() )
            {
                //
            }
            m_inHttpProcessStep++;
            _httpExecuteProcess();
            break;

        case cBlnsHttpAction::HA_SENDREQUESTSFINISHED:
            g_obLogger(cSeverity::DEBUG) << "HTTP: Nothing to do after confirm" << EOM;
            if( !_processCommResponse() )
            {
                //
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

//    g_obLogger(cSeverity::DEBUG) << "HTTP Token read finished " << m_qsToken << EOM;
}

//=================================================================================================
void cBlnsHttp::_sendProcessFinished()
//-------------------------------------------------------------------------------------------------
{
    g_obLogger(cSeverity::DEBUG) << "HTTP: Current process ["
                                 << cBlnsHttpAction::toStr( m_teBlnsHttpProcess )
                                 << "] last step finished" << EOM;

    switch( m_teBlnsHttpProcess )
    {
        case cBlnsHttpAction::HA_AUTHENTICATE:
            emit signalActionProcessed( QString("%1 Authentication succeeded (%2)")
                                                .arg( cBlnsHttpAction::toStr( m_teBlnsHttpProcess ) )
                                                .arg( m_qsToken ) );
            break;

        case cBlnsHttpAction::HA_PCSENDDATA:
            emit signalActionProcessed( QString("%1 Update patientcard succeeded (%2)")
                                                .arg( cBlnsHttpAction::toStr( m_teBlnsHttpProcess ) )
                                                .arg( m_qsBarcode ) );
            break;

        case cBlnsHttpAction::HA_PROCESSQUEUE:
            emit signalActionProcessed( QString("%1 Update patientcard succeeded (%2)")
                                                .arg( cBlnsHttpAction::toStr( m_teBlnsHttpProcess ) )
                                                .arg( m_qsBarcode ) );
            emit signalStepProgress();
//            processWaitingCardData();
            break;

        default:
            break;
    }
}

//=================================================================================================
void cBlnsHttp::_readResponseFromFile()
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

        QString  qsQuery;

        qsQuery = "UPDATE httppatientcardinfo SET ";
        qsQuery += QString( "active = 0, " );
        qsQuery += QString( "archive = \"ARC\" " );
        qsQuery += QString( "WHERE httpPatientcardInfoId = \"%1\" " ).arg( m_uiRecordId );

        m_uiRecordId = 0;
        g_poDB->executeQTQuery( qsQuery );
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

    QTextStream qtsFile(&file);
    QString     qsSha1File      = qtsFile.readLine();
    QString     qsXmlResponse   = qtsFile.readLine();

    QString qsSha1Xml = QString(QCryptographicHash::hash(qsXmlResponse.toStdString().c_str(),QCryptographicHash::Sha1).toHex());

    g_obLogger(cSeverity::DEBUG) << "HTTP: Response  ["
                                 << qsXmlResponse
                                 << "]"
                                 << EOM;

    if( qsSha1File.compare( qsSha1Xml ) != 0 )
    {
        g_obLogger(cSeverity::ERROR) << "HTTP: SHA1 code mismatch"
                                     << EOM;
        g_obLogger(cSeverity::DEBUG) << "HTTP: Resp Sha1 ["
                                     << qsSha1File
                                     << "]"
                                     << EOM;
        g_obLogger(cSeverity::DEBUG) << "HTTP: Gen Sha1  ["
                                     << qsSha1Xml
                                     << "]"
                                     << EOM;
        // return false;
    }

    QString      qsErrorMsg  = "";
    int          inErrorLine = 0;
    int          inErrorCol = 0;

    if( !obResponseXML->setContent( qsXmlResponse, &qsErrorMsg, &inErrorLine, &inErrorCol ) )
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

    for( int i=0; i<obCards.count(); i++)
    {
        unsigned int    uiPatientId = 0;
        unsigned int    uiCardId    = 0;
        unsigned int    uiLedgerId  = 0;

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
                uiLedgerId = _saveOnlineSell( uiCardId, uiPatientId, qsCardSellDate );
                _savePatientCardUnits( qsUnitCount, uiCardId, uiLedgerId );
            }
        }
    }

    return true;
}

//=================================================================================================
unsigned int cBlnsHttp::_saveGuest(QString p_qsName, QString p_qsUniqueId, QString p_qsEmail)
//-------------------------------------------------------------------------------------------------
{
    return 0;
}

//=================================================================================================
unsigned int cBlnsHttp::_savePatientCard(QString p_qsBarcode, QString p_qsValidDateTo, QString p_qsUnitCount, unsigned int p_uiPatientId)
//-------------------------------------------------------------------------------------------------
{
    return 0;
}

//=================================================================================================
unsigned int cBlnsHttp::_saveOnlineSell(unsigned int p_uiPatientCardId, unsigned int p_uiPatientId, QString p_qsLedgerTime)
//-------------------------------------------------------------------------------------------------
{
    return 0;
}

//=================================================================================================
void cBlnsHttp::_savePatientCardUnits(QString p_qsUnitCount, unsigned int p_uiPatientCardId, unsigned int p_uiLedgerId)
//-------------------------------------------------------------------------------------------------
{

}


//=================================================================================================
bool cBlnsHttp::_processCommResponse()
//-------------------------------------------------------------------------------------------------
{
    bool    bRet = true;

    return bRet;
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
        return;
    }

    if (requestId != m_httpGetId)
        return;

    obFile->close();

    if (error)
    {
        obFile->remove();
        m_qsError = tr("Error occured during downloading HTTP file.");
        g_obLogger(cSeverity::WARNING) << "HTTP: "
                                       << QString("Error occured during downloading file:\n%1.")
                                                 .arg( obHttp->errorString() )
                                       << EOM;
        emit signalErrorOccured();
        delete obFile;
        obFile = 0;
        g_obLogger(cSeverity::ERROR) << "HTTP Error occured" << EOM;
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


