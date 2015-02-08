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
//  cTracer obTrace( "cBlnsHttp::cBlnsHttp" );

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
//  cTracer obTrace( "cBlnsHttp::~cBlnsHttp" );

    if( m_inTimer > 0 )
        killTimer( m_inTimer );
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
//  cTracer obTrace( "cBlnsHttp::checkHttpServerAvailability" );

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
//  cTracer obTrace( "cBlnsHttp::sendPatientCardData" );

    QString  qsQuery;

    qsQuery = "INSERT INTO httppatientcardinfo SET ";
    qsQuery += QString( "licenceId = \"%1\", " ).arg( g_poPrefs->getLicenceId() );
    qsQuery += QString( "barcode = \"%1\", " ).arg( p_qsBarcode );
    qsQuery += QString( "patientcardInfoText = \"%1\", " ).arg( p_qsPatientCardData );
    qsQuery += QString( "active = 1, " );
    qsQuery += QString( "archive = \"NEW\" " );

    g_poDB->executeQTQuery( qsQuery );

    if( !p_bSendNow )
    {
        return;
    }

    if( !g_poPrefs->isBlnsHttpEnabled() )
    {
        m_qsError = tr("HTTP connection disabled.");
        m_inHttpProcessStep = HTTP_STATUS_DEFAULT;
        g_obLogger(cSeverity::WARNING) << "HTTP: abort sendPatientCardData - connection disabled" << EOM;
        emit signalErrorOccured();
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
    if( !g_poPrefs->isBlnsHttpEnabled() )
    {
        m_qsError = tr("HTTP connection disabled.");
        m_inHttpProcessStep = HTTP_STATUS_DEFAULT;
        g_obLogger(cSeverity::WARNING) << "HTTP: " << EOM;
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
QString cBlnsHttp::errorMessage()
//-------------------------------------------------------------------------------------------------
{
    return m_qsError;
}
//=================================================================================================
int cBlnsHttp::getNumberOfWaitingRecords()
//-------------------------------------------------------------------------------------------------
{
    QString      qsQuery    = "SELECT httpPatientcardInfoId, barcode, patientcardInfoText FROM "
                              "httppatientcardinfo WHERE "
                              "active=1 AND "
                              "archive='NEW' "
                              "LIMIT 1 ";
    QSqlQuery   *poQuery    = g_poDB->executeQTQuery( qsQuery );

    return poQuery->size();
}
//=================================================================================================
void cBlnsHttp::timerEvent(QTimerEvent *)
//-------------------------------------------------------------------------------------------------
{
//  cTracer obTrace( "cBlnsHttp::timerEvent" );

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
bool cBlnsHttp::_downloadFile(QString p_qsFileName)
//-------------------------------------------------------------------------------------------------
{
//  cTracer obTrace( "cBlnsHttp::_downloadFile" );

/*    g_obLogger(cSeverity::DEBUG) << "HTTP Download file ["
                                 << p_qsFileName
                                 << "]"
                                 << EOM;
*/
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
//  cTracer obTrace( "cBlnsHttp::_slotHttpRequestFinished" );

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

//    g_obLogger(cSeverity::DEBUG) << "HTTP Request finished" << EOM;

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
//  cTracer obTrace( "cBlnsHttp::_httpStartProcess" );

    g_obLogger(cSeverity::DEBUG) << "HTTP: Start HTTP process ["
                                 << cBlnsHttpAction::toStr( m_teBlnsHttpProcess )
                                 << "]"
                                 << EOM;
    g_obLogger(cSeverity::DEBUG) << "HTTP: Current step: "
                                 << m_inHttpProcessStep
                                 << EOM;

    if( !g_poPrefs->isBlnsHttpEnabled() )
    {
        g_obLogger(cSeverity::WARNING) << "HTTP: communication DISABLED" << EOM;
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
//  cTracer obTrace( "cBlnsHttp::_httpExecuteProcess" );

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
void cBlnsHttp::_httpProcessResponse()
//-------------------------------------------------------------------------------------------------
{
//  cTracer obTrace( "cBlnsHttp::_httpProcessResponse" );

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
void cBlnsHttp::_httpGetToken()
//-------------------------------------------------------------------------------------------------
{
//  cTracer obTrace( "cBlnsHttp::_httpGetToken" );

    // Empty token and request new from server
    m_qsToken = "";

    QString qsFileName = g_poPrefs->getServerAddress();

    qsFileName.append( "/token.php" );

    qsFileName = qsFileName.replace( "\\", "/" );
    qsFileName = qsFileName.replace( "//token.php", "/token.php" );

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
//  cTracer obTrace( "cBlnsHttp::_httpSendCardData" );

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
void cBlnsHttp::_readTokenFromFile()
//-------------------------------------------------------------------------------------------------
{
//  cTracer obTrace( "cBlnsHttp::_readTokenFromFile" );

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
void cBlnsHttp::_readCardSendResponseFromFile()
//-------------------------------------------------------------------------------------------------
{
//  cTracer obTrace( "cBlnsHttp::_readCardSendResponseFromFile" );

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
//  cTracer obTrace( "cBlnsHttp::_sendProcessFinished" );
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
    //  cTracer obTrace( "cBlnsHttp::_readResponseFromFile" );

    g_obLogger(cSeverity::DEBUG) << "HTTP: Read save.php" << EOM;

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
void cBlnsHttp::_updateProcessedRecord()
//-------------------------------------------------------------------------------------------------
{
    //  cTracer obTrace( "cBlnsHttp::_updateProcessedRecord" );
    g_poDB->executeQTQuery( "DELETE FROM httppatientcardinfo WHERE active = 0 AND archive = \"ARC\" " );
}

