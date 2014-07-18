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

    m_bErrorOccured             = false;
    m_teGibbigAction            = cGibbigAction::GA_DEFAULT;

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
    cTracer obTrace( "cGibbig::cGibbig" );

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
void cGibbig::gibbigAuthenticate( cGibbigAction::teGibbigAction p_teGibbigAction )
//-------------------------------------------------------------------------------------------------
{
    cTracer obTrace( "cGibbig::gibbigAuthenticate" );

    gibbigClearError();

    QNetworkReply *gbReply;

    QByteArray  qbMessage( QString( "{\"username\":\"%1\",\"password\":\"%2\"}" ).arg(m_qsGbUserName).arg(m_qsGbPassword).toStdString().c_str() );

    m_teGibbigAction = p_teGibbigAction;
    m_gbRequest.setUrl( QUrl( QString("https://%1/unifiedid/rest/user/authenticate").arg(m_qsHost) ) );
    gbReply = m_gbRestManager->post( m_gbRequest, qbMessage );
    gbReply->ignoreSslErrors();
    m_inTimer = startTimer( m_inTimeout );
}
//=================================================================================================
void cGibbig::gibbigPCTCreate( QString p_qsPatientCardType )
//-------------------------------------------------------------------------------------------------
{

}
//=================================================================================================
void cGibbig::gibbigPCTModify( QString p_qsPatientCardType )
//-------------------------------------------------------------------------------------------------
{

}
//=================================================================================================
void cGibbig::gibbigPCTDelete( QString p_qsPatientCardType )
//-------------------------------------------------------------------------------------------------
{

}
//=================================================================================================
void cGibbig::gibbigPCRegister(QString p_qsPatientCard)
//-------------------------------------------------------------------------------------------------
{
    cTracer obTrace( "cGibbig::gibbigPCRegister" );

    gibbigClearError();

    m_qsPatientCard = p_qsPatientCard;

    if( QDateTime::currentDateTime() < m_qdtExpiration )
    {
        m_teGibbigAction = cGibbigAction::GA_PCREGISTER;
        _sendPatientCardData();
    }
    else
    {
        gibbigAuthenticate( cGibbigAction::GA_AUTHENTICATE2 );
    }
}
//=================================================================================================
void cGibbig::gibbigPCRefill(QString p_qsPatientCard)
//-------------------------------------------------------------------------------------------------
{
    cTracer obTrace( "cGibbig::" );

    gibbigClearError();

    m_qsPatientCard = p_qsPatientCard;

    if( QDateTime::currentDateTime() < m_qdtExpiration )
    {
        m_teGibbigAction = cGibbigAction::GA_PCREFILL;
        _sendPatientCardData();
    }
    else
    {
        gibbigAuthenticate( cGibbigAction::GA_AUTHENTICATE3 );
    }
}
//=================================================================================================
void cGibbig::gibbigPCUse(QString p_qsPatientCard)
//-------------------------------------------------------------------------------------------------
{
    cTracer obTrace( "cGibbig::gibbigPCUse" );

    gibbigClearError();

    m_qsPatientCard = p_qsPatientCard;

    if( QDateTime::currentDateTime() < m_qdtExpiration )
    {
        m_teGibbigAction = cGibbigAction::GA_PCUSE;
        _sendPatientCardData();
    }
    else
    {
        gibbigAuthenticate( cGibbigAction::GA_AUTHENTICATE4 );
    }
}
//=================================================================================================
void cGibbig::gibbigPCDelete(QString p_qsPatientCard)
//-------------------------------------------------------------------------------------------------
{

}
//=================================================================================================
void cGibbig::timerEvent(QTimerEvent *)
//-------------------------------------------------------------------------------------------------
{
    cTracer obTrace( "cGibbig::timerEvent" );

    killTimer( m_inTimer );
    m_inTimer = 0;

    m_qsError.append( tr("Timeout error occured during Gibbig communication after %1 milliseconds.\n").arg(m_inTimeout) );
    m_qsError.append( tr("%1 FAILED due to timeout error.").arg( cGibbigAction::toStr( m_teGibbigAction ) ) );
    m_teGibbigAction = cGibbigAction::GA_DEFAULT;
    m_bErrorOccured = true;
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
void cGibbig::_processMessage()
//-------------------------------------------------------------------------------------------------
{
    cTracer obTrace( "cGibbig::_processMessage" );

    switch( m_teGibbigAction )
    {
        case cGibbigAction::GA_AUTHENTICATE1:
        case cGibbigAction::GA_AUTHENTICATE2:
        case cGibbigAction::GA_AUTHENTICATE3:
        case cGibbigAction::GA_AUTHENTICATE4:
        {
            if( m_qsMessage.left(10).compare( "{\"token\":\"" ) == 0 )
            {
                _getTokenExpFromMessage();

                if( m_teGibbigAction == cGibbigAction::GA_AUTHENTICATE1 )
                {
                    m_teGibbigAction = cGibbigAction::GA_DEFAULT;
                    emit signalActionProcessed( QString("GBMSG_01 Authentication succeeded (%1)\n%2 %3").arg(m_qsMessage).arg(m_qsToken).arg(m_qdtExpiration.toString("yyyy-MM-dd hh:mm:ss")) );
                }
                else if( cGibbigAction::GA_AUTHENTICATE2 )
                {
                    m_teGibbigAction = cGibbigAction::GA_PCREGISTER;
                    _sendPatientCardData();
                }
                else if( cGibbigAction::GA_AUTHENTICATE3 )
                {
                    m_teGibbigAction = cGibbigAction::GA_PCREFILL;
                    _sendPatientCardData();
                }
                else if( cGibbigAction::GA_AUTHENTICATE4 )
                {
                    m_teGibbigAction = cGibbigAction::GA_PCUSE;
                    _sendPatientCardData();
                }
            }
            else
            {
                m_teGibbigAction = cGibbigAction::GA_DEFAULT;
                m_qsError.append( tr("Invalid format, token string not received. '%1'\n").arg(m_qsMessage) );
                m_bErrorOccured = true;
                emit signalErrorOccured();
            }
            break;
        }

        case cGibbigAction::GA_PCREGISTER:
        {
            m_teGibbigAction = cGibbigAction::GA_DEFAULT;
            emit signalActionProcessed( QString("GBMSG_02 Patientcard register succeeded\n%1").arg(m_qsMessage) );
            break;
        }

        case cGibbigAction::GA_PCREFILL:
        {
            m_teGibbigAction = cGibbigAction::GA_DEFAULT;
            emit signalActionProcessed( QString("GBMSG_03 Patientcard refill succeeded\n%1").arg(m_qsMessage) );
            break;
        }

        case cGibbigAction::GA_PCUSE:
        {
            m_teGibbigAction = cGibbigAction::GA_DEFAULT;
            emit signalActionProcessed( QString("GBMSG_04 Patientcard use succeeded\n%1").arg(m_qsMessage) );
            break;
        }

        default:
        {
            m_qsError.append( tr("Unexpected message received.\n'%1'\n").arg(m_qsMessage) );
            m_bErrorOccured = true;
        }
    }
}
//=================================================================================================
void cGibbig::_sendPatientCardData()
//-------------------------------------------------------------------------------------------------
{
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

//    SELECT unitTime, validDateTo, COUNT(active) FROM `patientcardunits` WHERE patientcardid=2874 and active=1 GROUP BY unitTime, validDateTo

    QStringList qslUnits = _getUnits().split('|');

    qsMessage.append( "[" );

    for( int i=0; i<qslUnits.count(); i++ )
    {
        QStringList qslUnitData = qslUnits.at(i).split('/');

        qsMessage.append( QString( "{\"amount\":%1,\"couponParameters\":["
                                   "{\"name\":\"UNIT_VALUE\",\"value\":\"%2\"},"
                                   "{\"name\":\"VALID\",\"value\":\"%3\"}]}" ).arg(qslUnitData.at(0)).arg(qslUnitData.at(1)).arg(qslUnitData.at(2)) );
        if( i<qslUnits.count()-1 )
        {
            qsMessage.append( "," );
        }
    }
    qsMessage.append( "]}" );

    emit signalDebugMessage( qsMessage );

    QNetworkReply  *gbReply;
    QByteArray      qbMessage( qsMessage.toStdString().c_str() );

    m_gbRequest.setUrl( QUrl( QString("https://%1/unifiedid/rest/vendor/coupons/sync/%2?token=%3").arg(m_qsHost).arg(qsBarcode).arg(m_qsToken) ) );
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
