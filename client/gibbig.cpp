//=================================================================================================
//
// Belenus Kliens alkalmazas (c) Pagony Multimedia Studio Bt - 2014
//
//-------------------------------------------------------------------------------------------------
//
// Filename    : gibbig.cpp
// AppVersion  : 1.0
// FileVersion : 1.0
// Author      : Bikfalvi Tamas
//
//-------------------------------------------------------------------------------------------------
// Gibbig rendszerrel kapcsolatos reszek implementalasa
//=================================================================================================

#include <QtNetwork>
#include <QTimer>
#include <QStringList>

#include "gibbig.h"
#include "belenus.h"

//=================================================================================================
cGibbig::cGibbig()
//-------------------------------------------------------------------------------------------------
{
//    cTracer obTrace( "cGibbig::cGibbig" );

    m_gbRestManager             = NULL;

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
    m_teGibbigAction            = cGibbigAction::GA_DEFAULT;

    m_uiMessageId               = 0;

    m_qslResponseIds.clear();

    m_qdtExpiration.setTime_t(0);

    g_obLogger(cSeverity::DEBUG) << "Create QNetworkAccessManager" << EOM;

    m_gbRestManager = new QNetworkAccessManager( this );

    g_obLogger(cSeverity::DEBUG) << "Connect QNetworkAccessManager" << EOM;

    connect( m_gbRestManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(slotRestRequestFinished(QNetworkReply*)) );

    g_obLogger(cSeverity::DEBUG) << "Set header information" << EOM;

    m_gbRequest.setRawHeader( "Authorization", "Basic" );
    m_gbRequest.setRawHeader( "Accept-Encoding", "gzip,deflate" );
    m_gbRequest.setRawHeader( "Content-Type", "application/json" );
    m_gbRequest.setRawHeader( "Connection", "Keep-Alive" );
    m_gbRequest.setRawHeader( "User-Agent", "Apache-HttpClient/4.1.1 (java 1.5)" );
    m_gbRequest.setRawHeader( "Accept", "application/json" );
}
//=================================================================================================
cGibbig::~cGibbig()
//-------------------------------------------------------------------------------------------------
{
    if( m_inTimer > 0 )
        killTimer( m_inTimer );

    if( m_gbRestManager )   delete m_gbRestManager;
}
//=================================================================================================
void cGibbig::setHost( const QString p_qsHost )
//-------------------------------------------------------------------------------------------------
{
    cTracer obTrace( "cGibbig::setHost" );

    m_qsHost = p_qsHost;

    m_gbRequest.setRawHeader( "Host", m_qsHost.toStdString().c_str() );
}
//=================================================================================================
void cGibbig::setPort( const QString p_qsPort )
//-------------------------------------------------------------------------------------------------
{
    cTracer obTrace( "cGibbig::setPort" );

    m_qsPort = p_qsPort;
}
//=================================================================================================
void cGibbig::setUserName( const QString p_qsUserName )
//-------------------------------------------------------------------------------------------------
{
    cTracer obTrace( "cGibbig::setUserName" );

    m_qsGbUserName = p_qsUserName;
}
//=================================================================================================
void cGibbig::setPassword( const QString p_qsPassword )
//-------------------------------------------------------------------------------------------------
{
    cTracer obTrace( "cGibbig::setPassword" );

    m_qsGbPassword = p_qsPassword;
}
//=================================================================================================
void cGibbig::setTimeout(const int p_inTimeout)
//-------------------------------------------------------------------------------------------------
{
    cTracer obTrace( "cGibbig::setTimeout" );

    m_inTimeout = p_inTimeout;
}
//=================================================================================================
void cGibbig::gibbigAuthenticate()
//-------------------------------------------------------------------------------------------------
{
    cTracer obTrace( "cGibbig::gibbigAuthenticate" );

    gibbigClearError();

    QNetworkReply *gbReply;

    QByteArray  qbMessage( QString( "{\"username\":\"%1\",\"password\":\"%2\"}" ).arg(m_qsGbUserName).arg(m_qsGbPassword).toStdString().c_str() );

    m_teGibbigAction = cGibbigAction::GA_AUTHENTICATE;
    m_gbRequest.setUrl( QUrl( QString("https://%1/unifiedid/rest/user/authenticate").arg(m_qsHost) ) );
    gbReply = m_gbRestManager->post( m_gbRequest, qbMessage );
    gbReply->ignoreSslErrors();
    m_inTimer = startTimer( m_inTimeout );
}
//=================================================================================================
void cGibbig::gibbigPCTCreate( QString p_qsPatientCardType )
//-------------------------------------------------------------------------------------------------
{
    cTracer obTrace( "cGibbig::gibbigPCTCreate" );

    QString  qsQuery;

    qsQuery = "INSERT INTO gibbigmessages SET ";
    qsQuery += QString( "licenceId = \"%1\", " ).arg( g_poPrefs->getLicenceId() );
    qsQuery += QString( "gibbigMessageTypeId = 1, " );//.arg( "PCT_CREATE" );
    qsQuery += QString( "gibbigMessage = \"%1\", " ).arg( p_qsPatientCardType );
    qsQuery += QString( "active = 1, " );
    qsQuery += QString( "archive = \"NEW\" " );

    g_poDB->executeQTQuery( qsQuery );

    _activateProcess();
}
//=================================================================================================
void cGibbig::gibbigPCTModify( QString p_qsPatientCardType )
//-------------------------------------------------------------------------------------------------
{
    cTracer obTrace( "cGibbig::gibbigPCTModify" );

    QString  qsQuery;

    qsQuery = "INSERT INTO gibbigmessages SET ";
    qsQuery += QString( "licenceId = \"%1\", " ).arg( g_poPrefs->getLicenceId() );
    qsQuery += QString( "gibbigMessageTypeId = 2, " );//.arg( "PCT_MODIFY" );
    qsQuery += QString( "gibbigMessage = \"%1\", " ).arg( p_qsPatientCardType );
    qsQuery += QString( "active = 1, " );
    qsQuery += QString( "archive = \"NEW\" " );

    g_poDB->executeQTQuery( qsQuery );

    _activateProcess();
}
//=================================================================================================
void cGibbig::gibbigPCTDelete( QString p_qsPatientCardType )
//-------------------------------------------------------------------------------------------------
{
    cTracer obTrace( "cGibbig::gibbigPCTDelete" );

    QString  qsQuery;

    qsQuery = "INSERT INTO gibbigmessages SET ";
    qsQuery += QString( "licenceId = \"%1\", " ).arg( g_poPrefs->getLicenceId() );
    qsQuery += QString( "gibbigMessageTypeId = 3, " );//.arg( "PCT_DELETE" );
    qsQuery += QString( "gibbigMessage = \"%1\", " ).arg( p_qsPatientCardType );
    qsQuery += QString( "active = 1, " );
    qsQuery += QString( "archive = \"NEW\" " );

    g_poDB->executeQTQuery( qsQuery );

    _activateProcess();
}
//=================================================================================================
void cGibbig::gibbigPCRegister(QString p_qsPatientCard)
//-------------------------------------------------------------------------------------------------
{
    cTracer obTrace( "cGibbig::gibbigPCRegister" );

    QString  qsQuery;

    qsQuery = "INSERT INTO gibbigmessages SET ";
    qsQuery += QString( "licenceId = \"%1\", " ).arg( g_poPrefs->getLicenceId() );
    qsQuery += QString( "gibbigMessageTypeId = 4, " );//.arg( "PC_SELL" );
    qsQuery += QString( "gibbigMessage = \"%1\", " ).arg( p_qsPatientCard );
    qsQuery += QString( "active = 1, " );
    qsQuery += QString( "archive = \"NEW\" " );

    g_poDB->executeQTQuery( qsQuery );

    _activateProcess();
}
//=================================================================================================
void cGibbig::gibbigPCRefill(QString p_qsPatientCard)
//-------------------------------------------------------------------------------------------------
{
    cTracer obTrace( "cGibbig::gibbigPCRefill" );

    QString  qsQuery;

    qsQuery = "INSERT INTO gibbigmessages SET ";
    qsQuery += QString( "licenceId = \"%1\", " ).arg( g_poPrefs->getLicenceId() );
    qsQuery += QString( "gibbigMessageTypeId = 5, " );//.arg( "PC_REFILL" );
    qsQuery += QString( "gibbigMessage = \"%1\", " ).arg( p_qsPatientCard );
    qsQuery += QString( "active = 1, " );
    qsQuery += QString( "archive = \"NEW\" " );

    g_poDB->executeQTQuery( qsQuery );

    _activateProcess();
}
//=================================================================================================
void cGibbig::gibbigPCUse(QString p_qsPatientCard)
//-------------------------------------------------------------------------------------------------
{
    cTracer obTrace( "cGibbig::gibbigPCUse" );

    QString  qsQuery;

    qsQuery = "INSERT INTO gibbigmessages SET ";
    qsQuery += QString( "licenceId = \"%1\", " ).arg( g_poPrefs->getLicenceId() );
    qsQuery += QString( "gibbigMessageTypeId = 6, " );//.arg( "PC_USE" );
    qsQuery += QString( "gibbigMessage = \"%1\", " ).arg( p_qsPatientCard );
    qsQuery += QString( "active = 1, " );
    qsQuery += QString( "archive = \"NEW\" " );

    g_poDB->executeQTQuery( qsQuery );

    _activateProcess();
}
//=================================================================================================
void cGibbig::gibbigPCDelete(QString p_qsPatientCard)
//-------------------------------------------------------------------------------------------------
{
    cTracer obTrace( "cGibbig::gibbigPCDelete" );

    QString  qsQuery;

    qsQuery = "INSERT INTO gibbigmessages SET ";
    qsQuery += QString( "licenceId = \"%1\", " ).arg( g_poPrefs->getLicenceId() );
    qsQuery += QString( "gibbigMessageTypeId = 7, " );//.arg( "PC_DELETE" );
    qsQuery += QString( "gibbigMessage = \"%1\", " ).arg( p_qsPatientCard );
    qsQuery += QString( "active = 1, " );
    qsQuery += QString( "archive = \"NEW\" " );

    g_poDB->executeQTQuery( qsQuery );

    _activateProcess();
}
//=================================================================================================
void cGibbig::timerEvent(QTimerEvent *)
//-------------------------------------------------------------------------------------------------
{
    cTracer obTrace( "cGibbig::timerEvent" );

    killTimer( m_inTimer );
    m_inTimer = 0;

    m_qsError.append( tr("Gibbig timeout.\n") );
    m_qsError.append( tr("%1 FAILED.").arg( cGibbigAction::toStr( m_teGibbigAction ) ) );
    m_teGibbigAction = cGibbigAction::GA_DEFAULT;
    m_bErrorOccured = true;
    emit signalDebugMessage( m_qsError );
    emit signalErrorOccured();
}
//=================================================================================================
void cGibbig::slotRestRequestFinished(QNetworkReply *p_gbReply)
//-------------------------------------------------------------------------------------------------
{
    cTracer obTrace( "cGibbig::slotRestRequestFinished" );

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
void cGibbig::_processPCTCreate( QString p_qsPatientCardType )
//-------------------------------------------------------------------------------------------------
{
    cTracer obTrace( "cGibbig::_processPCTCreate" );

    gibbigClearError();

    m_qsPatientCardType = p_qsPatientCardType;
    m_teGibbigAction    = cGibbigAction::GA_PCTCREATE;

    _sendPatientCardTypeData();
}
//=================================================================================================
void cGibbig::_processPCTModify( QString p_qsPatientCardType )
//-------------------------------------------------------------------------------------------------
{
    cTracer obTrace( "cGibbig::_processPCTModify" );

    gibbigClearError();

    m_qsPatientCardType = p_qsPatientCardType;
    m_teGibbigAction    = cGibbigAction::GA_PCTMODIFY;

    _sendPatientCardTypeData();
}
//=================================================================================================
void cGibbig::_processPCTDelete( QString p_qsPatientCardType )
//-------------------------------------------------------------------------------------------------
{
    cTracer obTrace( "cGibbig::_processPCTDelete" );

    gibbigClearError();

    m_qsPatientCardType = p_qsPatientCardType;
    m_teGibbigAction    = cGibbigAction::GA_PCTDELETE;

    _sendPatientCardTypeData();
}
//=================================================================================================
void cGibbig::_processPCRegister( QString p_qsPatientCard )
//-------------------------------------------------------------------------------------------------
{
    cTracer obTrace( "cGibbig::_processPCRegister" );

    gibbigClearError();

    m_qsPatientCard     = p_qsPatientCard;
    m_teGibbigAction    = cGibbigAction::GA_PCREGISTER;

    _sendPatientCardData();
}
//=================================================================================================
void cGibbig::_processPCRefill( QString p_qsPatientCard )
//-------------------------------------------------------------------------------------------------
{
    cTracer obTrace( "cGibbig::_processPCRefill" );

    gibbigClearError();

    m_qsPatientCard     = p_qsPatientCard;
    m_teGibbigAction    = cGibbigAction::GA_PCREFILL;

    _sendPatientCardData();
}
//=================================================================================================
void cGibbig::_processPCUse( QString p_qsPatientCard )
//-------------------------------------------------------------------------------------------------
{
    cTracer obTrace( "cGibbig::_processPCUse" );

    gibbigClearError();

    m_qsPatientCard     = p_qsPatientCard;
    m_teGibbigAction    = cGibbigAction::GA_PCUSE;

    _sendPatientCardData();
}
//=================================================================================================
void cGibbig::_processPCDelete( QString p_qsPatientCard )
//-------------------------------------------------------------------------------------------------
{
    cTracer obTrace( "cGibbig::_processPCDelete" );

    gibbigClearError();

    m_qsPatientCard     = p_qsPatientCard;
    m_teGibbigAction    = cGibbigAction::GA_PCDELETE;

    _sendPatientCardData();
}
//=================================================================================================
void cGibbig::_processMessage()
//-------------------------------------------------------------------------------------------------
{
    cTracer obTrace( "cGibbig::_processMessage" );

    emit signalDebugMessage( m_qsMessage );
    switch( m_teGibbigAction )
    {
        case cGibbigAction::GA_AUTHENTICATE:
        {
            if( m_qsMessage.left(10).compare( "{\"token\":\"" ) == 0 )
            {
                _getTokenExpFromMessage();
                emit signalActionProcessed( QString("%1 Authentication succeeded (%2)\n%3 %4")
                                                    .arg( cGibbigAction::toStr( m_teGibbigAction ) )
                                                    .arg(m_qsMessage)
                                                    .arg(m_qsToken)
                                                    .arg(m_qdtExpiration.toString("yyyy-MM-dd hh:mm:ss")) );
                m_teGibbigAction = cGibbigAction::GA_DEFAULT;
            }
            else
            {
                m_qsError.append( tr("Invalid format, token string not received. '%1'\n").arg(m_qsMessage) );
                m_bErrorOccured = true;
                emit signalErrorOccured();
                m_teGibbigAction = cGibbigAction::GA_DEFAULT;
                return;
            }
            break;
        }

        case cGibbigAction::GA_PCTCREATE:
        case cGibbigAction::GA_PCTMODIFY:
        case cGibbigAction::GA_PCTDELETE:
        case cGibbigAction::GA_PCREGISTER:
        case cGibbigAction::GA_PCREFILL:
        case cGibbigAction::GA_PCUSE:
        case cGibbigAction::GA_PCDELETE:
        {
            m_qslResponseIds.clear();
            if( _getResult() && m_uiMessageId > 0 )
            {
                g_poDB->executeQTQuery( QString("UPDATE gibbigmessages SET archive='MOD' "
                                                "WHERE gibbigMessageId=%1 " )
                                                .arg( m_uiMessageId ) );
                emit signalActionProcessed( QString( "%1 SUCCEEDED" ).arg( cGibbigAction::toStr( m_teGibbigAction ) ) );
            }
            else
            {
                m_qsError.append( QString( "%1 FAILED" ).arg( cGibbigAction::toStr( m_teGibbigAction ) ) );
                m_bErrorOccured = true;
                emit signalErrorOccured();
            }
            m_uiMessageId = 0;
            m_teGibbigAction = cGibbigAction::GA_DEFAULT;
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
void cGibbig::_sendPatientCardTypeData()
//-------------------------------------------------------------------------------------------------
{
    cTracer obTrace( "cGibbig::_sendPatientCardTypeData" );

}
//=================================================================================================
void cGibbig::_sendPatientCardData()
//-------------------------------------------------------------------------------------------------
{
    cTracer obTrace( "cGibbig::_sendPatientCardData" );

    QString qsBarcode = _getBarcode();
    QString qsMessage = "";

    switch( m_teGibbigAction )
    {
        case cGibbigAction::GA_PCREGISTER:
            qsMessage.append( "{\"transaction\":\"grant\",\"masterCoupons\":" );
            break;

        case cGibbigAction::GA_PCREFILL:
            qsMessage.append( "{\"transaction\":\"grant\",\"masterCoupons\":" );
            break;

        case cGibbigAction::GA_PCUSE:
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
    gbReply = m_gbRestManager->post( m_gbRequest, qbMessage );
    gbReply->ignoreSslErrors();
    m_inTimer = startTimer( m_inTimeout );

}
//=================================================================================================
void cGibbig::_getTokenExpFromMessage()
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
QString cGibbig::_getBarcode()
//-------------------------------------------------------------------------------------------------
{
    QStringList qslPatientCard  = m_qsPatientCard.split( '#' );

    return qslPatientCard.at(0);
}
//=================================================================================================
QString cGibbig::_getUnits()
//-------------------------------------------------------------------------------------------------
{
    QStringList qslPatientCard  = m_qsPatientCard.split( '#' );

    return qslPatientCard.at(2);
}
//=================================================================================================
void cGibbig::_activateProcess()
//-------------------------------------------------------------------------------------------------
{
    cTracer obTrace( "cGibbig::_activateProcess" );

    if( m_teGibbigAction == cGibbigAction::GA_DEFAULT )
    {
        if( QDateTime::currentDateTime() >= m_qdtExpiration )
        {
            gibbigAuthenticate();
        }
        _prepareProcess();
    }
}
//=================================================================================================
void cGibbig::_prepareProcess()
//-------------------------------------------------------------------------------------------------
{
    cTracer obTrace( "cGibbig::_prepareProcess" );

    if( !g_poPrefs->isGibbigEnabled() )
    {
        g_obLogger(cSeverity::DEBUG) << "GIBBIG DISABLED" << EOM;
        return;
    }

    if( m_teGibbigAction != cGibbigAction::GA_DEFAULT )
    {
        g_obLogger(cSeverity::DEBUG) << "GIBBIG ACTION IN PROGRESS" << EOM;
        return;
    }

    QString      qsQuery    = "SELECT gibbigMessageId, gibbigMessageType, gibbigMessage FROM "
                              "gibbigMessages, gibbigMessageTypes WHERE "
                              "gibbigMessages.gibbigMessageTypeId=gibbigMessageTypes.gibbigMessageTypeId AND "
                              "gibbigMessages.active=1 AND "
                              "gibbigMessages.archive='NEW' "
                              "LIMIT 1 ";
    QSqlQuery   *poQuery    = g_poDB->executeQTQuery( qsQuery );
    QString      qsType;
    QString      qsMessage;

    g_obLogger(cSeverity::DEBUG) << "GIBBIG Number of records: " << poQuery->size() << EOM;

    if( poQuery->size() < 1 )
    {
        m_teGibbigAction = cGibbigAction::GA_DEFAULT;
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
        m_teGibbigAction = cGibbigAction::GA_DEFAULT;
        return;
    }
}
//=================================================================================================
bool cGibbig::_getResult()
//-------------------------------------------------------------------------------------------------
{
    bool    bRet        = false;
    QString qsResults   = m_qsMessage.mid( 13, m_qsMessage.length()-16 );

/*    g_obLogger(cSeverity::DEBUG) << "GIBBIG result: "
                                 << qsResults
                                 << EOM;*/

    QStringList qslResults = qsResults.split( "},{" );

/*    g_obLogger(cSeverity::DEBUG) << "GIBBIG results count: "
                                 << qslResults.count()
                                 << EOM;*/

    for( int i=0; i<qslResults.count(); i++ )
    {
/*        g_obLogger(cSeverity::DEBUG) << "GIBBIG response: "
                                     << qslResults.at(i)
                                     << EOM;*/
        if( _getResponseStatus( qslResults.at(i) ) )
        {
            bRet = true;
            m_qslResponseIds << _getResponseId( qslResults.at(i) );
        }
    }

    return bRet;
}
//=================================================================================================
bool cGibbig::_getResponseStatus( QString p_qsResponse )
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
QString cGibbig::_getResponseId( QString p_qsResponse )
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
