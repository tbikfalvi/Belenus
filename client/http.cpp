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
#include "belenus.h"
#include "licenceManager.h"

extern cLicenceManager g_obLicenceManager;

//=================================================================================================
cBlnsHttp::cBlnsHttp()
//-------------------------------------------------------------------------------------------------
{
//    cTracer obTrace( "cBlnsHttp::cBlnsHttp" );

    obHttp                  = NULL;
    obFile                  = NULL;
    m_httpRequestAborted    = false;
    m_httpGetId             = 0;

    m_qsHost                = "";
    m_qsError               = "";
    m_inTimeout             = 0;
    m_inTimer               = 0;
    m_qsToken               = "";
    m_inHttpProcessStep     = HTTP_STATUS_DEFAULT;
    m_teBlnsHttpProcess     = cBlnsHttpAction::HA_DEFAULT;

    m_vrHttpActions.clear();

    obHttp = new QHttp( this );

    connect(obHttp, SIGNAL(requestFinished(int, bool)), this, SLOT(_slotHttpRequestFinished(int,bool)));
    connect(obHttp, SIGNAL(dataReadProgress(int, int)), this, SLOT(_slotUpdateDataReadProgress(int, int)));
    connect(obHttp, SIGNAL(responseHeaderReceived(const QHttpResponseHeader &)), this, SLOT(_slotReadResponseHeader(const QHttpResponseHeader &)));
    connect(obHttp, SIGNAL(authenticationRequired(const QString &, quint16, QAuthenticator *)), this, SLOT(_slotAuthenticationRequired(const QString &, quint16, QAuthenticator *)));
#ifndef QT_NO_OPENSSL
    connect(obHttp, SIGNAL(sslErrors(const QList<QSslError> &)), this, SLOT(_slotSslErrors(const QList<QSslError> &)));
#endif

}
//=================================================================================================
cBlnsHttp::~cBlnsHttp()
//-------------------------------------------------------------------------------------------------
{
//    cTracer obTrace( "cBlnsHttp::~cBlnsHttp" );

    if( m_inTimer > 0 )
        killTimer( m_inTimer );
}
//=================================================================================================
void cBlnsHttp::setHost( const QString p_qsHost )
//-------------------------------------------------------------------------------------------------
{
    m_qsHost = p_qsHost;
}
//=================================================================================================
void cBlnsHttp::setTimeout( const int p_inTimeout )
//-------------------------------------------------------------------------------------------------
{
    m_inTimeout = p_inTimeout;
}
//=================================================================================================
void cBlnsHttp::checkHttpServerAvailability()
//-------------------------------------------------------------------------------------------------
{
//    cTracer obTrace( "cBlnsHttp::checkHttpServerAvailability" );

    m_vrHttpActions.clear();
    m_vrHttpActions.push_back( cBlnsHttpAction::HA_AUTHENTICATE );
    m_vrHttpActions.push_back( cBlnsHttpAction::HA_PROCESSFINISHED );

    m_teBlnsHttpProcess = cBlnsHttpAction::HA_AUTHENTICATE;
    m_inHttpProcessStep = 0;

    _httpStartProcess();
}
//=================================================================================================
void cBlnsHttp::sendPatientCardData(QString p_qsBarcode, QString p_qsPatientCardData )
//-------------------------------------------------------------------------------------------------
{
//    cTracer obTrace( "cBlnsHttp::sendPatientCardData" );

    QString  qsQuery;

    qsQuery = "INSERT INTO httppatientcardinfo SET ";
    qsQuery += QString( "licenceId = \"%1\", " ).arg( g_poPrefs->getLicenceId() );
    qsQuery += QString( "barcode = \"%1\", " ).arg( p_qsBarcode );
    qsQuery += QString( "patientcardInfoText = \"%1\", " ).arg( p_qsPatientCardData );
    qsQuery += QString( "active = 1, " );
    qsQuery += QString( "archive = \"NEW\" " );

    g_poDB->executeQTQuery( qsQuery );

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
void cBlnsHttp::timerEvent(QTimerEvent *)
//-------------------------------------------------------------------------------------------------
{
//    cTracer obTrace( "cBlnsHttp::timerEvent" );

    killTimer( m_inTimer );
    m_inTimer = 0;

    m_qsError.append( tr("HTTP timeout.\n") );
    m_qsError.append( tr("%1 FAILED.").arg( cBlnsHttpAction::toStr( m_vrHttpActions.at( m_inHttpProcessStep ) ) ) );
    m_inHttpProcessStep = HTTP_STATUS_DEFAULT;
    emit signalDebugMessage( m_qsError );
    emit signalErrorOccured();
}
//=================================================================================================
bool cBlnsHttp::_downloadFile(QString p_qsFileName)
//-------------------------------------------------------------------------------------------------
{
//    cTracer obTrace( "cBlnsHttp::_downloadFile" );

    g_obLogger(cSeverity::DEBUG) << "HTTP Download file ["
                                 << p_qsFileName
                                 << "]"
                                 << EOM;

    QUrl        url( p_qsFileName );
    QFileInfo   fileInfo(url.path());

    if( fileInfo.fileName().isEmpty() )
        return false;

    QString     fileName = QString("%1\\%2").arg(QDir::currentPath()).arg( fileInfo.fileName() );

    if( QFile::exists(fileName) )
    {
        QFile::remove(fileName);
    }

    obFile = new QFile(fileName);

    if( !obFile->open(QIODevice::WriteOnly) )
    {
        m_qsError = tr("Unable to save HTTP communication file.");
        emit signalDebugMessage( QString("Unable to save the file\n\n%1\n\n%2.").arg( fileName )
                                                                           .arg( obFile->errorString() ) );
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

    QString qsDowload = QString( path );

    if( url.hasQuery() )
    {
        qsDowload.append( "?" );
        qsDowload.append( QString( url.encodedQuery() ) );
    }

    g_obLogger(cSeverity::DEBUG) << "HTTP GET ["
                                 << qsDowload
                                 << "]"
                                 << EOM;

    m_httpGetId = obHttp->get(qsDowload, obFile);
    m_inTimer   = startTimer( m_inTimeout );

    return true;
}
//=================================================================================================
// _slotHttpRequestFinished
//-------------------------------------------------------------------------------------------------
void cBlnsHttp::_slotHttpRequestFinished(int requestId, bool error)
{
//    cTracer obTrace( "cBlnsHttp::_slotHttpRequestFinished" );

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
        emit signalDebugMessage( QString("Error occured during downloading file:\n%1.").arg( obHttp->errorString() ) );
        emit signalErrorOccured();
        delete obFile;
        obFile = 0;
        g_obLogger(cSeverity::ERROR) << "HTTP Error occured" << EOM;
        return;
    }

    delete obFile;
    obFile = 0;

    g_obLogger(cSeverity::DEBUG) << "HTTP Request finished" << EOM;

    _httpProcessResponse();
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
void cBlnsHttp::_slotUpdateDataReadProgress(int /*bytesRead*/, int /*totalBytes*/)
//-------------------------------------------------------------------------------------------------
{
    if (m_httpRequestAborted)
        return;

//    _progressMax( totalBytes );
//    _progressValue( bytesRead );
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
//=================================================================================================
void cBlnsHttp::_httpStartProcess()
//-------------------------------------------------------------------------------------------------
{
//    cTracer obTrace( "cBlnsHttp::_httpStartProcess" );

    if( !g_poPrefs->isBlnsHttpEnabled() )
    {
        g_obLogger(cSeverity::DEBUG) << "HTTP communication DISABLED" << EOM;
        return;
    }

    if( m_vrHttpActions.size() < 1 )
    {
        g_obLogger(cSeverity::ERROR) << "HTTP actions not prepared correctly" << EOM;
        return;
    }

    if( m_inHttpProcessStep > (int)(m_vrHttpActions.size()-1) ||
        m_inHttpProcessStep < 0 )
    {
        g_obLogger(cSeverity::ERROR) << "HTTP invalid action step: ["
                                     << m_inHttpProcessStep
                                     << "]"
                                     << EOM;
        return;
    }

    if( m_teBlnsHttpProcess == cBlnsHttpAction::HA_PCSENDDATA ||
        m_teBlnsHttpProcess == cBlnsHttpAction::HA_PROCESSQUEUE )
    {
        QString      qsQuery    = "SELECT barcode, patientcardInfoText FROM "
                                  "httppatientcardinfo WHERE "
                                  "active=1 AND "
                                  "archive='NEW' "
                                  "LIMIT 1 ";
        QSqlQuery   *poQuery    = g_poDB->executeQTQuery( qsQuery );

        if( poQuery->size() != 1 )
        {
            g_obLogger(cSeverity::DEBUG) << "HTTP no more record" << EOM;
            return;
        }

        poQuery->first();

        m_qsBarcode  = poQuery->value(0).toString();
        m_qsCardData = poQuery->value(1).toString();

        g_obLogger(cSeverity::DEBUG) << "HTTP Barcode ["
                                     << m_qsBarcode
                                     << "]"
                                     << EOM;
        g_obLogger(cSeverity::DEBUG) << "HTTP CardData ["
                                     << m_qsCardData
                                     << "]"
                                     << EOM;
    }

    _httpExecuteProcess();
}
//=================================================================================================
void cBlnsHttp::_httpExecuteProcess()
//-------------------------------------------------------------------------------------------------
{
//    cTracer obTrace( "cBlnsHttp::_httpExecuteProcess" );

    switch( m_vrHttpActions.at( m_inHttpProcessStep ) )
    {
        case cBlnsHttpAction::HA_AUTHENTICATE:
        {
            _httpGetToken();
            break;
        }

        case cBlnsHttpAction::HA_PCSENDDATA:
            // Send first record of httpmessage table with previously requested token
            _httpSendCardData();
            break;

        case cBlnsHttpAction::HA_PCUPDATERECORD:
            break;

        case cBlnsHttpAction::HA_PROCESSFINISHED:
            _httpProcessResponse();
            break;

        default:
            // Nothing to do
            break;
    }
}
//=================================================================================================
void cBlnsHttp::_httpProcessResponse()
//-------------------------------------------------------------------------------------------------
{
//    cTracer obTrace( "cBlnsHttp::_httpProcessResponse" );

    switch( m_vrHttpActions.at( m_inHttpProcessStep ) )
    {
        case cBlnsHttpAction::HA_AUTHENTICATE:
            _readTokenFromFile();
            if( m_qsToken.length() != 16 )
            {
                m_qsError = tr("Invalid token received: ");
                m_qsError.append( m_qsToken );
                emit signalDebugMessage( QString("received token is not valid:\n%1.").arg( m_qsToken ) );
                emit signalErrorOccured();
                g_obLogger(cSeverity::ERROR) << "HTTP Invalid token" << EOM;
                m_inHttpProcessStep = HTTP_ERROR_INVALID_TOKEN;
                return;
            }
            m_inHttpProcessStep++;
            _httpExecuteProcess();
            break;

        case cBlnsHttpAction::HA_PCSENDDATA:
            _readResponseFromFile();
            m_inHttpProcessStep++;
            break;

        case cBlnsHttpAction::HA_PCUPDATERECORD:
            _updateProcessedRecord();
            m_inHttpProcessStep++;
            break;

        case cBlnsHttpAction::HA_PROCESSFINISHED:
            _sendProcessFinished();
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
//    cTracer obTrace( "cBlnsHttp::_httpGetToken" );

    // Empty token and request new from server
    m_qsToken = "";
    // http://www.kiwisun.hu/kiwi_ticket
    QString qsFileName = g_poPrefs->getServerAddress();

    qsFileName.append( "/token.php" );

    qsFileName = qsFileName.replace( "\\", "/" );
    qsFileName = qsFileName.replace( "//token.php", "/token.php" );

    _downloadFile( qsFileName );
}

//=================================================================================================
void cBlnsHttp::_httpSendCardData()
//-------------------------------------------------------------------------------------------------
{
//    cTracer obTrace( "cBlnsHttp::_httpSendCardData" );

    // http://www.kiwisun.hu/kiwi_ticket/save.php
    QString qsFileName  = g_poPrefs->getServerAddress();
    QString qsMd5Source = "";
    QString qsTimeStamp = QString::number( QDateTime::currentDateTime().toTime_t() );

    qsMd5Source.append( m_qsToken );
    qsMd5Source.append( qsTimeStamp );
    qsMd5Source.append( "kiwibérlet" );

    QString qsMd5Hash = QString(QCryptographicHash::hash(qsMd5Source.toStdString().c_str(),QCryptographicHash::Md5).toHex());


    qsFileName.append( "/save.php" );

    qsFileName = qsFileName.replace( "\\", "/" );
    qsFileName = qsFileName.replace( "//save.php", "/save.php" );

    qsFileName.append( QString( "?token=%1" ).arg( m_qsToken ) );
    qsFileName.append( QString( "&stamp=%1" ).arg( qsTimeStamp ) );
    qsFileName.append( QString( "&code=%1" ).arg( qsMd5Hash ) );
    qsFileName.append( QString( "&StudioId=%1" ).arg( g_obLicenceManager.licenceKey() ) );
    qsFileName.append( QString( "&CardId=%1" ).arg( m_qsBarcode ) );
    qsFileName.append( QString( "&CardData=%1" ).arg( m_qsCardData ) );

    _downloadFile( qsFileName );
}
//=================================================================================================
void cBlnsHttp::_readTokenFromFile()
//-------------------------------------------------------------------------------------------------
{
//    cTracer obTrace( "cBlnsHttp::_readTokenFromFile" );

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

    g_obLogger(cSeverity::DEBUG) << "HTTP Token read finished " << m_qsToken << EOM;
}
//=================================================================================================
void cBlnsHttp::_readCardSendResponseFromFile()
//-------------------------------------------------------------------------------------------------
{
//    cTracer obTrace( "cBlnsHttp::_readCardSendResponseFromFile" );

    QString fileName = QString("%1\\save.php").arg( QDir::currentPath() );
    QFile   file( fileName );

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return;

    QTextStream qtsFile(&file);
    m_qsCardDataSendResponse = qtsFile.readLine();
}
//=================================================================================================
void cBlnsHttp::_sendProcessFinished()
//-------------------------------------------------------------------------------------------------
{
//    cTracer obTrace( "cBlnsHttp::_sendProcessFinished" );

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
            processWaitingCardData();
            break;

        default:
            break;
    }
}
//=================================================================================================
void cBlnsHttp::_readResponseFromFile()
//-------------------------------------------------------------------------------------------------
{
    //    cTracer obTrace( "cBlnsHttp::_readResponseFromFile" );

    QString fileName = QString("%1\\save.php").arg( QDir::currentPath() );
    QFile   file( fileName );

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        g_obLogger(cSeverity::ERROR) << "HTTP Unable to open file " << fileName << EOM;
        return;
    }

    QTextStream qtsFile(&file);
    QString     qsLine = qtsFile.readLine();

    if( qsLine.contains( "true" ) )
    {
        QString  qsQuery;

        qsQuery = "UPDATE httppatientcardinfo SET ";
        qsQuery += QString( "active = 0, " );
        qsQuery += QString( "archive = \"ARC\" " );
        qsQuery += QString( "WHERE barcode = \"%1\", " ).arg( m_qsBarcode );

        g_poDB->executeQTQuery( qsQuery );
    }
    else if( qsLine.contains( "false" ) )
    {
        m_qsError = tr("Unknown error occured on server side.");
        emit signalDebugMessage( m_qsError );
        emit signalErrorOccured();
        m_inHttpProcessStep = HTTP_ERROR_UNKNOWN;
    }
    else if( qsLine.contains( "Wrong token" ) )
    {
        m_qsError = tr("HTTP Session expired");
        emit signalDebugMessage( QString("Token not accepted by server") );
        emit signalErrorOccured();
        m_inHttpProcessStep = HTTP_ERROR_WRONG_TOKEN;
    }
    else if( qsLine.contains( "Check error" ) )
    {
        m_qsError = tr("HTTP security check failed");
        emit signalDebugMessage( QString("MD5 hash not accepted by server") );
        emit signalErrorOccured();
        m_inHttpProcessStep = HTTP_ERROR_MD5_MISMATCH;
    }
    else if( qsLine.contains( "StudioId unknown" ) )
    {
        m_qsError = tr("Unknown studio Id");
        emit signalDebugMessage( QString("Studio Id not accepted by server") );
        emit signalErrorOccured();
        m_inHttpProcessStep = HTTP_ERROR_INVALID_STUDIO;
    }
    else if( qsLine.contains( "SQL error" ) )
    {
        m_qsError = tr("Database error occured on server side");
        emit signalDebugMessage( QString("Server was unable to execute SQL command") );
        emit signalErrorOccured();
        m_inHttpProcessStep = HTTP_ERROR_SERVER_SQL;
    }
}
//=================================================================================================
void cBlnsHttp::_updateProcessedRecord()
//-------------------------------------------------------------------------------------------------
{
    //    cTracer obTrace( "cBlnsHttp::_updateProcessedRecord" );

}





/*
//=================================================================================================
cBlnsHttp::cBlnsHttp()
//-------------------------------------------------------------------------------------------------
{
    cTracer obTrace( "cBlnsHttp::cBlnsHttp" );

    m_httpManager               = NULL;

    m_qsHost                    = "";
    m_qsPort                    = "";
    m_qsGbUserName              = "";
    m_qsGbPassword              = "";
    m_inTimeout                 = 0;

    m_qsMessage                 = "";
    m_qsInfo                    = "";
    m_qsError                   = "";
    m_inTimer                   = 0;

    m_qsToken                   = "";
    m_qsPatientCard             = "";
    m_qsPatientCardType         = "";

    m_bErrorOccured             = false;
    m_teBlnsHttpAction          = cBlnsHttpAction::GA_DEFAULT;

    m_uiMessageId               = 0;

    m_qslResponseIds.clear();

    m_qdtExpiration.setTime_t(0);

    g_obLogger(cSeverity::DEBUG) << "Create QNetworkAccessManager" << EOM;

    m_httpManager = new QNetworkAccessManager( this );

    g_obLogger(cSeverity::DEBUG) << "Connect QNetworkAccessManager" << EOM;

    connect( m_httpManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(slotRestRequestFinished(QNetworkReply*)) );

    g_obLogger(cSeverity::DEBUG) << "Set header information" << EOM;

    m_gbRequest.setRawHeader( "Authorization", "Basic" );
    m_gbRequest.setRawHeader( "Accept-Encoding", "gzip,deflate" );
    m_gbRequest.setRawHeader( "Content-Type", "application/json" );
    m_gbRequest.setRawHeader( "Connection", "Keep-Alive" );
    m_gbRequest.setRawHeader( "User-Agent", "Apache-HttpClient/4.1.1 (java 1.5)" );
    m_gbRequest.setRawHeader( "Accept", "application/json" );
}
//=================================================================================================
cBlnsHttp::~cBlnsHttp()
//-------------------------------------------------------------------------------------------------
{
    if( m_inTimer > 0 )
        killTimer( m_inTimer );

    if( m_httpManager )   delete m_httpManager;
}
//=================================================================================================
void cBlnsHttp::setHost( const QString p_qsHost )
//-------------------------------------------------------------------------------------------------
{
    cTracer obTrace( "cBlnsHttp::setHost" );

    m_qsHost = p_qsHost;

    m_gbRequest.setRawHeader( "Host", m_qsHost.toStdString().c_str() );
}
//=================================================================================================
void cBlnsHttp::setPort( const QString p_qsPort )
//-------------------------------------------------------------------------------------------------
{
    cTracer obTrace( "cBlnsHttp::setPort" );

    m_qsPort = p_qsPort;
}
//=================================================================================================
void cBlnsHttp::setUserName( const QString p_qsUserName )
//-------------------------------------------------------------------------------------------------
{
    cTracer obTrace( "cBlnsHttp::setUserName" );

    m_qsGbUserName = p_qsUserName;
}
//=================================================================================================
void cBlnsHttp::setPassword( const QString p_qsPassword )
//-------------------------------------------------------------------------------------------------
{
    cTracer obTrace( "cBlnsHttp::setPassword" );

    m_qsGbPassword = p_qsPassword;
}
//=================================================================================================
void cBlnsHttp::setTimeout(const int p_inTimeout)
//-------------------------------------------------------------------------------------------------
{
    cTracer obTrace( "cBlnsHttp::setTimeout" );

    m_inTimeout = p_inTimeout;
}
//=================================================================================================
void cBlnsHttp::blnshttpAuthenticate()
//-------------------------------------------------------------------------------------------------
{
    cTracer obTrace( "cBlnsHttp::blnshttpAuthenticate" );

    blnshttpClearError();

    QNetworkReply *gbReply;

    QByteArray  qbMessage( QString( "{\"username\":\"%1\",\"password\":\"%2\"}" ).arg(m_qsGbUserName).arg(m_qsGbPassword).toStdString().c_str() );

    m_teGibbigAction = cBlnsHttpAction::GA_AUTHENTICATE;
    m_gbRequest.setUrl( QUrl( QString("https://%1/unifiedid/rest/user/authenticate").arg(m_qsHost) ) );
    gbReply = m_httpManager->post( m_gbRequest, qbMessage );
    gbReply->ignoreSslErrors();
    m_inTimer = startTimer( m_inTimeout );
}
//=================================================================================================
void cBlnsHttp::blnshttpPCTCreate( QString p_qsPatientCardType )
//-------------------------------------------------------------------------------------------------
{
    cTracer obTrace( "cBlnsHttp::blnshttpPCTCreate" );

    QString  qsQuery;

    qsQuery = "INSERT INTO blnshttpmessages SET ";
    qsQuery += QString( "licenceId = \"%1\", " ).arg( g_poPrefs->getLicenceId() );
    qsQuery += QString( "blnshttpMessageTypeId = 1, " );//.arg( "PCT_CREATE" );
    qsQuery += QString( "blnshttpMessage = \"%1\", " ).arg( p_qsPatientCardType );
    qsQuery += QString( "active = 1, " );
    qsQuery += QString( "archive = \"NEW\" " );

    g_poDB->executeQTQuery( qsQuery );

    _activateProcess();
}
//=================================================================================================
void cBlnsHttp::blnshttpPCTModify( QString p_qsPatientCardType )
//-------------------------------------------------------------------------------------------------
{
    cTracer obTrace( "cBlnsHttp::blnshttpPCTModify" );

    QString  qsQuery;

    qsQuery = "INSERT INTO blnshttpmessages SET ";
    qsQuery += QString( "licenceId = \"%1\", " ).arg( g_poPrefs->getLicenceId() );
    qsQuery += QString( "blnshttpMessageTypeId = 2, " );//.arg( "PCT_MODIFY" );
    qsQuery += QString( "blnshttpMessage = \"%1\", " ).arg( p_qsPatientCardType );
    qsQuery += QString( "active = 1, " );
    qsQuery += QString( "archive = \"NEW\" " );

    g_poDB->executeQTQuery( qsQuery );

    _activateProcess();
}
//=================================================================================================
void cBlnsHttp::blnshttpPCTDelete( QString p_qsPatientCardType )
//-------------------------------------------------------------------------------------------------
{
    cTracer obTrace( "cBlnsHttp::blnshttpPCTDelete" );

    QString  qsQuery;

    qsQuery = "INSERT INTO blnshttpmessages SET ";
    qsQuery += QString( "licenceId = \"%1\", " ).arg( g_poPrefs->getLicenceId() );
    qsQuery += QString( "blnshttpMessageTypeId = 3, " );//.arg( "PCT_DELETE" );
    qsQuery += QString( "blnshttpMessage = \"%1\", " ).arg( p_qsPatientCardType );
    qsQuery += QString( "active = 1, " );
    qsQuery += QString( "archive = \"NEW\" " );

    g_poDB->executeQTQuery( qsQuery );

    _activateProcess();
}
//=================================================================================================
void cBlnsHttp::blnshttpPCRegister(QString p_qsPatientCard)
//-------------------------------------------------------------------------------------------------
{
    cTracer obTrace( "cBlnsHttp::blnshttpPCRegister" );

    QString  qsQuery;

    qsQuery = "INSERT INTO blnshttpmessages SET ";
    qsQuery += QString( "licenceId = \"%1\", " ).arg( g_poPrefs->getLicenceId() );
    qsQuery += QString( "blnshttpMessageTypeId = 4, " );//.arg( "PC_SELL" );
    qsQuery += QString( "blnshttpMessage = \"%1\", " ).arg( p_qsPatientCard );
    qsQuery += QString( "active = 1, " );
    qsQuery += QString( "archive = \"NEW\" " );

    g_poDB->executeQTQuery( qsQuery );

    _activateProcess();
}
//=================================================================================================
void cBlnsHttp::blnshttpPCRefill(QString p_qsPatientCard)
//-------------------------------------------------------------------------------------------------
{
    cTracer obTrace( "cBlnsHttp::blnshttpPCRefill" );

    QString  qsQuery;

    qsQuery = "INSERT INTO blnshttpmessages SET ";
    qsQuery += QString( "licenceId = \"%1\", " ).arg( g_poPrefs->getLicenceId() );
    qsQuery += QString( "blnshttpMessageTypeId = 5, " );//.arg( "PC_REFILL" );
    qsQuery += QString( "blnshttpMessage = \"%1\", " ).arg( p_qsPatientCard );
    qsQuery += QString( "active = 1, " );
    qsQuery += QString( "archive = \"NEW\" " );

    g_poDB->executeQTQuery( qsQuery );

    _activateProcess();
}
//=================================================================================================
void cBlnsHttp::blnshttpPCUse(QString p_qsPatientCard)
//-------------------------------------------------------------------------------------------------
{
    cTracer obTrace( "cBlnsHttp::blnshttpPCUse" );

    QString  qsQuery;

    qsQuery = "INSERT INTO blnshttpmessages SET ";
    qsQuery += QString( "licenceId = \"%1\", " ).arg( g_poPrefs->getLicenceId() );
    qsQuery += QString( "blnshttpMessageTypeId = 6, " );//.arg( "PC_USE" );
    qsQuery += QString( "blnshttpMessage = \"%1\", " ).arg( p_qsPatientCard );
    qsQuery += QString( "active = 1, " );
    qsQuery += QString( "archive = \"NEW\" " );

    g_poDB->executeQTQuery( qsQuery );

    _activateProcess();
}
//=================================================================================================
void cBlnsHttp::blnshttpPCDelete(QString p_qsPatientCard)
//-------------------------------------------------------------------------------------------------
{
    cTracer obTrace( "cBlnsHttp::blnshttpPCDelete" );

    QString  qsQuery;

    qsQuery = "INSERT INTO blnshttpmessages SET ";
    qsQuery += QString( "licenceId = \"%1\", " ).arg( g_poPrefs->getLicenceId() );
    qsQuery += QString( "blnshttpMessageTypeId = 7, " );//.arg( "PC_DELETE" );
    qsQuery += QString( "blnshttpMessage = \"%1\", " ).arg( p_qsPatientCard );
    qsQuery += QString( "active = 1, " );
    qsQuery += QString( "archive = \"NEW\" " );

    g_poDB->executeQTQuery( qsQuery );

    _activateProcess();
}
//=================================================================================================
void cBlnsHttp::timerEvent(QTimerEvent *)
//-------------------------------------------------------------------------------------------------
{
    cTracer obTrace( "cBlnsHttp::timerEvent" );

    killTimer( m_inTimer );
    m_inTimer = 0;

    m_qsError.append( tr("Gibbig timeout.\n") );
    m_qsError.append( tr("%1 FAILED.").arg( cBlnsHttpAction::toStr( m_teGibbigAction ) ) );
    m_teGibbigAction = cBlnsHttpAction::GA_DEFAULT;
    m_bErrorOccured = true;
    emit signalDebugMessage( m_qsError );
    emit signalErrorOccured();
}
//=================================================================================================
void cBlnsHttp::slotRestRequestFinished(QNetworkReply *p_gbReply)
//-------------------------------------------------------------------------------------------------
{
    cTracer obTrace( "cBlnsHttp::slotRestRequestFinished" );

    killTimer( m_inTimer );
    m_inTimer = 0;

    if( p_gbReply->error() > 0)
    {
        m_qsError.append( tr("Rest error: %1\n").arg( p_gbReply->errorString() ) );
        m_bErrorOccured = true;
        emit signalDebugMessage( m_qsError );
        emit signalErrorOccured();
    }
    else
    {
        m_qsError   = "";
        m_qsMessage = p_gbReply->readAll();
        _processMessage();
    }
}
//=================================================================================================
void cBlnsHttp::_processPCTCreate( QString p_qsPatientCardType )
//-------------------------------------------------------------------------------------------------
{
    cTracer obTrace( "cBlnsHttp::_processPCTCreate" );

    blnshttpClearError();

    m_qsPatientCardType = p_qsPatientCardType;
    m_teGibbigAction    = cBlnsHttpAction::GA_PCTCREATE;

    _sendPatientCardTypeData();
}
//=================================================================================================
void cBlnsHttp::_processPCTModify( QString p_qsPatientCardType )
//-------------------------------------------------------------------------------------------------
{
    cTracer obTrace( "cBlnsHttp::_processPCTModify" );

    blnshttpClearError();

    m_qsPatientCardType = p_qsPatientCardType;
    m_teGibbigAction    = cBlnsHttpAction::GA_PCTMODIFY;

    _sendPatientCardTypeData();
}
//=================================================================================================
void cBlnsHttp::_processPCTDelete( QString p_qsPatientCardType )
//-------------------------------------------------------------------------------------------------
{
    cTracer obTrace( "cBlnsHttp::_processPCTDelete" );

    blnshttpClearError();

    m_qsPatientCardType = p_qsPatientCardType;
    m_teGibbigAction    = cBlnsHttpAction::GA_PCTDELETE;

    _sendPatientCardTypeData();
}
//=================================================================================================
void cBlnsHttp::_processPCRegister( QString p_qsPatientCard )
//-------------------------------------------------------------------------------------------------
{
    cTracer obTrace( "cBlnsHttp::_processPCRegister" );

    blnshttpClearError();

    m_qsPatientCard     = p_qsPatientCard;
    m_teGibbigAction    = cBlnsHttpAction::GA_PCREGISTER;

    _sendPatientCardData();
}
//=================================================================================================
void cBlnsHttp::_processPCRefill( QString p_qsPatientCard )
//-------------------------------------------------------------------------------------------------
{
    cTracer obTrace( "cBlnsHttp::_processPCRefill" );

    blnshttpClearError();

    m_qsPatientCard     = p_qsPatientCard;
    m_teGibbigAction    = cBlnsHttpAction::GA_PCREFILL;

    _sendPatientCardData();
}
//=================================================================================================
void cBlnsHttp::_processPCUse( QString p_qsPatientCard )
//-------------------------------------------------------------------------------------------------
{
    cTracer obTrace( "cBlnsHttp::_processPCUse" );

    blnshttpClearError();

    m_qsPatientCard     = p_qsPatientCard;
    m_teGibbigAction    = cBlnsHttpAction::GA_PCUSE;

    _sendPatientCardData();
}
//=================================================================================================
void cBlnsHttp::_processPCDelete( QString p_qsPatientCard )
//-------------------------------------------------------------------------------------------------
{
    cTracer obTrace( "cBlnsHttp::_processPCDelete" );

    blnshttpClearError();

    m_qsPatientCard     = p_qsPatientCard;
    m_teGibbigAction    = cBlnsHttpAction::GA_PCDELETE;

    _sendPatientCardData();
}
//=================================================================================================
void cBlnsHttp::_processMessage()
//-------------------------------------------------------------------------------------------------
{
    cTracer obTrace( "cBlnsHttp::_processMessage" );

    emit signalDebugMessage( m_qsMessage );
    switch( m_teGibbigAction )
    {
        case cBlnsHttpAction::GA_AUTHENTICATE:
        {
            if( m_qsMessage.left(10).compare( "{\"token\":\"" ) == 0 )
            {
                _getTokenExpFromMessage();
                emit signalActionProcessed( QString("%1 Authentication succeeded (%2)\n%3 %4")
                                                    .arg( cBlnsHttpAction::toStr( m_teGibbigAction ) )
                                                    .arg(m_qsMessage)
                                                    .arg(m_qsToken)
                                                    .arg(m_qdtExpiration.toString("yyyy-MM-dd hh:mm:ss")) );
                m_teGibbigAction = cBlnsHttpAction::GA_DEFAULT;
            }
            else
            {
                m_qsError.append( tr("Invalid format, token string not received. '%1'\n").arg(m_qsMessage) );
                m_bErrorOccured = true;
                emit signalErrorOccured();
                m_teGibbigAction = cBlnsHttpAction::GA_DEFAULT;
                return;
            }
            break;
        }

        case cBlnsHttpAction::GA_PCTCREATE:
        case cBlnsHttpAction::GA_PCTMODIFY:
        case cBlnsHttpAction::GA_PCTDELETE:
        case cBlnsHttpAction::GA_PCREGISTER:
        case cBlnsHttpAction::GA_PCREFILL:
        case cBlnsHttpAction::GA_PCUSE:
        case cBlnsHttpAction::GA_PCDELETE:
        {
            m_qslResponseIds.clear();
            if( _getResult() && m_uiMessageId > 0 )
            {
                if( m_teGibbigAction == cBlnsHttpAction::GA_PCREGISTER ||
                    m_teGibbigAction == cBlnsHttpAction::GA_PCREFILL )
                {
                    QSqlQuery *poQuery = g_poDB->executeQTQuery( QString("SELECT blnshttpMessage FROM blnshttpmessages "
                                                                         "WHERE blnshttpMessageId=%1 " )
                                                                         .arg( m_uiMessageId ) );
                    poQuery->first();
                    emit signalUpdatePatientCard( poQuery->value(0).toString(), m_qslResponseIds.at(0) );
                }
                g_poDB->executeQTQuery( QString("UPDATE blnshttpmessages SET archive='ARC' "
                                                "WHERE blnshttpMessageId=%1 " )
                                                .arg( m_uiMessageId ) );
                emit signalActionProcessed( QString( "%1 SUCCEEDED" ).arg( cBlnsHttpAction::toStr( m_teGibbigAction ) ) );
            }
            else
            {
                m_qsError.append( QString( "%1 FAILED" ).arg( cBlnsHttpAction::toStr( m_teGibbigAction ) ) );
                m_bErrorOccured = true;
                emit signalErrorOccured();
            }
            m_uiMessageId = 0;
            m_teGibbigAction = cBlnsHttpAction::GA_DEFAULT;
            break;
        }

        default:
        {
            m_uiMessageId = 0;
            m_qsError.append( tr("Unexpected message received.\n'%1'\n").arg(m_qsMessage) );
            m_bErrorOccured = true;
            emit signalErrorOccured();
            return;
        }
    }
    _prepareProcess();
}
//=================================================================================================
void cBlnsHttp::_sendPatientCardTypeData()
//-------------------------------------------------------------------------------------------------
{
    cTracer obTrace( "cBlnsHttp::_sendPatientCardTypeData" );

}
//=================================================================================================
void cBlnsHttp::_sendPatientCardData()
//-------------------------------------------------------------------------------------------------
{
    cTracer obTrace( "cBlnsHttp::_sendPatientCardData" );

    QString qsBarcode = _getBarcode();
    QString qsMessage = "";

    switch( m_teGibbigAction )
    {
        case cBlnsHttpAction::GA_PCREGISTER:
            qsMessage.append( "{\"transaction\":\"grant\",\"masterCoupons\":" );
            break;

        case cBlnsHttpAction::GA_PCREFILL:
            qsMessage.append( "{\"transaction\":\"grant\",\"masterCoupons\":" );
            break;

        case cBlnsHttpAction::GA_PCUSE:
            qsMessage.append( "{\"transaction\":\"use\",\"masterCoupons\":" );
            break;

        default:
            // Do nothing
            return;
    }

    QStringList qslUnits = _getUnits().split('|');

    qsMessage.append( "[" );

    for( int i=0; i<qslUnits.count(); i++ )
    {
        QStringList qslUnitData = qslUnits.at(i).split('/');

        qsMessage.append( QString( "{\"amount\":%1,\"id\":%2,\"couponParameters\":"
                                   "[{\"name\":\"UNIT_VALUE\",\"value\":\"%3\"},"
                                   "{\"name\":\"VALID\",\"value\":\"%4\"}]}" ).arg(qslUnitData.at(0))
                                                                              .arg(qslUnitData.at(1))
                                                                              .arg(qslUnitData.at(2))
                                                                              .arg(qslUnitData.at(3)) );
        if( i<qslUnits.count()-1 )
        {
            qsMessage.append( "," );
        }
    }

    qsMessage.append( "]}" );

    QNetworkReply  *gbReply;
    QByteArray      qbMessage( qsMessage.toStdString().c_str() );
    QString         qsUrl = QString("https://%1/unifiedid/rest/vendor/coupons/sync/%2?token=%3")
                                    .arg(m_qsHost)
                                    .arg(qsBarcode)
                                    .arg(m_qsToken);

    emit signalDebugMessage( qsUrl );
    emit signalDebugMessage( qsMessage );

    m_gbRequest.setUrl( QUrl( qsUrl ) );
    gbReply = m_httpManager->post( m_gbRequest, qbMessage );
    gbReply->ignoreSslErrors();
    m_inTimer = startTimer( m_inTimeout );

}
//=================================================================================================
void cBlnsHttp::_getTokenExpFromMessage()
//-------------------------------------------------------------------------------------------------
{
    // {"token":"2c6a7f24-6862-4cf6-8ff2-2931fd7e253e","expiration":1401644234657}
    QStringList     qslTemp;
    QString         qsTemp;
    QString         qsExpiration;

    qslTemp         = m_qsMessage.split( ',' );
    qsTemp          = qslTemp.at(0);
    qslTemp         = qsTemp.split( ':' );
    qsTemp          = qslTemp.at(1);
    m_qsToken       = qsTemp.remove( '"' );

    qslTemp         = m_qsMessage.split( ',' );
    qsTemp          = qslTemp.at(1);
    qslTemp         = qsTemp.split( ':' );
    qsExpiration    = qslTemp.at(1);
    m_qdtExpiration.setTime_t( qsExpiration.left(qsExpiration.length()-4).toInt() );
}
//=================================================================================================
QString cBlnsHttp::_getBarcode()
//-------------------------------------------------------------------------------------------------
{
    QStringList qslPatientCard  = m_qsPatientCard.split( '#' );

    return qslPatientCard.at(0);
}
//=================================================================================================
QString cBlnsHttp::_getUnits()
//-------------------------------------------------------------------------------------------------
{
    QStringList qslPatientCard  = m_qsPatientCard.split( '#' );

    return qslPatientCard.at(2);
}
//=================================================================================================
void cBlnsHttp::_activateProcess()
//-------------------------------------------------------------------------------------------------
{
    cTracer obTrace( "cBlnsHttp::_activateProcess" );

    if( !g_poPrefs->isGibbigEnabled() )
    {
        g_obLogger(cSeverity::DEBUG) << "GIBBIG DISABLED" << EOM;
        return;
    }

    if( m_teGibbigAction == cBlnsHttpAction::GA_DEFAULT )
    {
        if( QDateTime::currentDateTime() >= m_qdtExpiration )
        {
            blnshttpAuthenticate();
        }
        _prepareProcess();
    }
}
//=================================================================================================
void cBlnsHttp::_prepareProcess()
//-------------------------------------------------------------------------------------------------
{
    cTracer obTrace( "cBlnsHttp::_prepareProcess" );

    if( !g_poPrefs->isGibbigEnabled() )
    {
        g_obLogger(cSeverity::DEBUG) << "GIBBIG DISABLED" << EOM;
        return;
    }

    if( m_teGibbigAction != cBlnsHttpAction::GA_DEFAULT )
    {
        g_obLogger(cSeverity::DEBUG) << "GIBBIG ACTION IN PROGRESS" << EOM;
        return;
    }

    QString      qsQuery    = "SELECT blnshttpMessageId, blnshttpMessageType, blnshttpMessage FROM "
                              "blnshttpMessages, blnshttpMessageTypes WHERE "
                              "blnshttpMessages.blnshttpMessageTypeId=blnshttpMessageTypes.blnshttpMessageTypeId AND "
                              "blnshttpMessages.active=1 AND "
                              "blnshttpMessages.archive='NEW' "
                              "LIMIT 1 ";
    QSqlQuery   *poQuery    = g_poDB->executeQTQuery( qsQuery );
    QString      qsType;
    QString      qsMessage;

    g_obLogger(cSeverity::DEBUG) << "GIBBIG Number of records: " << poQuery->size() << EOM;

    if( poQuery->size() < 1 )
    {
        m_teGibbigAction = cBlnsHttpAction::GA_DEFAULT;
        return;
    }

    poQuery->first();

    m_uiMessageId   = poQuery->value( 0 ).toUInt();
    qsType          = poQuery->value( 1 ).toString();
    qsMessage       = poQuery->value( 2 ).toString();

    g_obLogger(cSeverity::DEBUG) << "GIBBIG message record: "
                                 << m_uiMessageId
                                 << " "
                                 << qsType
                                 << " "
                                 << qsMessage
                                 << EOM;

    if( qsType.compare( "PCT_CREATE" ) == 0 )
    {
        _processPCTCreate( qsMessage );
    }
    else if( qsType.compare( "PCT_MODIFY" ) == 0 )
    {
        _processPCTModify( qsMessage );
    }
    else if( qsType.compare( "PCT_DELETE" ) == 0 )
    {
        _processPCTDelete( qsMessage );
    }
    else if( qsType.compare( "PC_SELL" ) == 0 )
    {
        _processPCRegister( qsMessage );
    }
    else if( qsType.compare( "PC_REFILL" ) == 0 )
    {
        _processPCRefill( qsMessage );
    }
    else if( qsType.compare( "PC_USE" ) == 0 )
    {
        _processPCUse( qsMessage );
    }
    else if( qsType.compare( "PC_DELETE" ) == 0 )
    {
        _processPCDelete( qsMessage );
    }
    else
    {
        m_teGibbigAction = cBlnsHttpAction::GA_DEFAULT;
        return;
    }
}
//=================================================================================================
bool cBlnsHttp::_getResult()
//-------------------------------------------------------------------------------------------------
{
    bool    bRet        = false;
    QString qsResults   = m_qsMessage.mid( 13, m_qsMessage.length()-16 );

    QStringList qslResults = qsResults.split( "},{" );


    for( int i=0; i<qslResults.count(); i++ )
    {
        if( _getResponseStatus( qslResults.at(i) ) )
        {
            bRet = true;
            m_qslResponseIds << _getResponseId( qslResults.at(i) );
        }
        else
        {
            bRet = false;
            break;
        }
    }

    return bRet;
}
//=================================================================================================
bool cBlnsHttp::_getResponseStatus( QString p_qsResponse )
//-------------------------------------------------------------------------------------------------
{
    bool        bRet    = false;
    QStringList qslTags = p_qsResponse.split( "," );

    for( int i=0; i<qslTags.count(); i++ )
    {
        QString qsName  = qslTags.at(i).split(":").at(0);
        QString qsValue = qslTags.at(i).split(":").at(1);

        if( qsName.compare("\"result\"") == 0 )
        {
            g_obLogger(cSeverity::DEBUG) << "GIBBIG status: "
                                         << qsValue
                                         << EOM;
            if( qsValue.compare("\"SUCCESS\"") == 0 )
                bRet = true;
            break;
        }
    }
    return bRet;
}
//=================================================================================================
QString cBlnsHttp::_getResponseId( QString p_qsResponse )
//-------------------------------------------------------------------------------------------------
{
    QString     qsRet   = "";
    QStringList qslTags = p_qsResponse.split( "," );

    for( int i=0; i<qslTags.count(); i++ )
    {
        QString qsName  = qslTags.at(i).split(":").at(0);
        QString qsValue = qslTags.at(i).split(":").at(1);

        if( qsName.compare("\"id\"") == 0 )
        {
            g_obLogger(cSeverity::DEBUG) << "GIBBIG value: "
                                         << qsValue
                                         << EOM;
            qsRet = qsValue;
            break;
        }
    }
    return qsRet;
}
//=================================================================================================
*/
