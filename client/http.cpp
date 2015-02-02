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

#include "http.h"
#include "belenus.h"

//=================================================================================================
cBlnsHttp::cBlnsHttp()
//-------------------------------------------------------------------------------------------------
{
    cTracer obTrace( "cBlnsHttp::cBlnsHttp" );

    obHttp                  = NULL;
    obFile                  = NULL;
    m_httpRequestAborted    = false;
    m_httpGetId             = 0;

    m_qsHost                = "";
    m_inTimeout             = 0;

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
    cTracer obTrace( "cBlnsHttp::~cBlnsHttp" );

//    if( m_inTimer > 0 )
//        killTimer( m_inTimer );

//    if( m_httpManager )   delete m_httpManager;
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

}
//=================================================================================================
void cBlnsHttp::sendPatientCardData(QString p_qsBarcode, QString p_qsPatientCardData )
//-------------------------------------------------------------------------------------------------
{
    QString  qsQuery;

    qsQuery = "INSERT INTO httppatientcardinfo SET ";
    qsQuery += QString( "licenceId = \"%1\", " ).arg( g_poPrefs->getLicenceId() );
    qsQuery += QString( "barcode = \"%1\", " ).arg( p_qsBarcode );
    qsQuery += QString( "patientcardInfoText = \"%1\", " ).arg( p_qsPatientCardData );
    qsQuery += QString( "active = 1, " );
    qsQuery += QString( "archive = \"NEW\" " );

    g_poDB->executeQTQuery( qsQuery );

//    _activateProcess();

}
//=================================================================================================
// _downloadFile
//-------------------------------------------------------------------------------------------------
bool cBlnsHttp::_downloadFile(QString p_qsFileName)
{
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
        QMessageBox::warning( 0, tr("Warning"),
                              tr("Unable to save the file\n\n%1\n\n%2.").arg( fileName ).arg( obFile->errorString() ) );
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
    m_httpGetId = obHttp->get(path, obFile);

    return true;
}
//=================================================================================================
// _progressText
//-------------------------------------------------------------------------------------------------
void cBlnsHttp::_slotHttpRequestFinished(int requestId, bool error)
{
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
        return;
    }

    if (requestId != m_httpGetId)
        return;

    obFile->close();

    if (error)
    {
        obFile->remove();
        QMessageBox::warning( 0, tr("Warning"),
                              QObject::tr("Error occured during downloading file:\n%1.").arg( obHttp->errorString() ) );
        delete obFile;
        obFile = 0;
        return;
    }

    delete obFile;
    obFile = 0;

//    _log( QString("Downloading file finished. Next: %1\n").arg( m_teProcessStep ) );

//    m_nTimer = startTimer( CONST_PROCESS_STEP_WAIT_MS );
}
//=================================================================================================
// _progressText
//-------------------------------------------------------------------------------------------------
void cBlnsHttp::_slotReadResponseHeader(const QHttpResponseHeader &responseHeader)
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
// _progressText
//-------------------------------------------------------------------------------------------------
void cBlnsHttp::_slotUpdateDataReadProgress(int bytesRead, int totalBytes)
{
    if (m_httpRequestAborted)
        return;

//    _progressMax( totalBytes );
//    _progressValue( bytesRead );
}
//=================================================================================================
// _progressText
//-------------------------------------------------------------------------------------------------
void cBlnsHttp::_slotAuthenticationRequired(const QString &hostName, quint16, QAuthenticator *authenticator)
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
//=================================================================================================
// _progressText
//-------------------------------------------------------------------------------------------------
#ifndef QT_NO_OPENSSL
void cBlnsHttp::_slotSslErrors(const QList<QSslError> &/*errors*/)
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
