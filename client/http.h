//====================================================================================
//
// Belenus Kliens alkalmazas (c) Pagony Multimedia Studio Bt - 2014
//
//====================================================================================
//
// Filename    : http.h
// AppVersion  : 1.0
// FileVersion : 1.0
// Author      : Bikfalvi Tamas
//
//====================================================================================
// BlnsHttp rendszerrel kapcsolatos reszek implementalasa
//====================================================================================

#ifndef BLNS_HTTP_H
#define BLNS_HTTP_H

//====================================================================================

#include <QObject>
#include <QHttp>
#include <QFile>
#include <QAuthenticator>
#include <QDateTime>
#include <QStringList>

//====================================================================================
class cBlnsHttpAction
{
public:
    enum teBlnsHttpAction
    {
        HA_DEFAULT = 0,
        HA_AUTHENTICATE,
        HA_PCSENDDATA
    };

    static const char *toStr( teBlnsHttpAction p_enGA )
    {
        switch( p_enGA )
        {
            case HA_DEFAULT:        return "HTTPMSG_01 Unidentified ";              break;
            case HA_AUTHENTICATE:   return "HTTPMSG_02 Authenticate ";              break;
            case HA_PCSENDDATA:     return "HTTPMSG_03 PatientCardSendData ";       break;
            default:                return "INVALID_IDENTIFIER";
        }
    }
};

//====================================================================================
class cBlnsHttp : public QObject
{
    Q_OBJECT

public:

    cBlnsHttp();
    ~cBlnsHttp();

    void             setHost( const QString p_qsHost );
    void             setTimeout( const int p_inTimeout );
    void             sendPatientCardData( QString p_qsPatientCardData );

    void             checkHttpServerAvailability();

private:

    QHttp           *obHttp;
    QFile           *obFile;
    bool             m_httpRequestAborted;
    int              m_httpGetId;

    QString          m_qsHost;
    int              m_inTimeout;

    bool            _downloadFile( QString p_qsFileName );

signals:

    void             signalErrorOccured();
    void             signalActionProcessed( QString p_qsInfo );

private slots:
    void             _slotHttpRequestFinished(int requestId, bool error);
    void             _slotUpdateDataReadProgress(int bytesRead, int totalBytes);
    void             _slotReadResponseHeader(const QHttpResponseHeader &responseHeader);
    void             _slotAuthenticationRequired(const QString &, quint16, QAuthenticator *);
#ifndef QT_NO_OPENSSL
    void             _slotSslErrors(const QList<QSslError> &errors);
#endif

};
//====================================================================================

/*
class cBlnsHttp : public QObject
{
    Q_OBJECT

public:

    cBlnsHttp();
    ~cBlnsHttp();

    void    setHost( const QString p_qsHost );
    void    setPort( const QString p_qsPort );
    void    setUserName( const QString p_qsUserName );
    void    setPassword( const QString p_qsPassword );
    void    setTimeout( const int p_inTimeout );

    void    blnshttpAuthenticate();
    void    blnshttpPCTCreate( QString p_qsPatientCardType );
    void    blnshttpPCTModify( QString p_qsPatientCardType );
    void    blnshttpPCTDelete( QString p_qsPatientCardType );
    void    blnshttpPCRegister( QString p_qsPatientCard );
    void    blnshttpPCRefill( QString p_qsPatientCard );
    void    blnshttpPCUse( QString p_qsPatientCard );
    void    blnshttpPCDelete( QString p_qsPatientCard );

    void    blnshttpClearError()          {   m_bErrorOccured = false; m_qsError = "";    }
    bool    blnshttpIsErrorOccured()      {   return m_bErrorOccured;                     }
    QString blnshttpErrorStr()            {   return m_qsError;                           }

protected:

    void    timerEvent( QTimerEvent *p_poEvent );

protected slots:

    void    slotRestRequestFinished(QNetworkReply *p_gbReply);

signals:

    void    signalErrorOccured();
    void    signalActionProcessed( QString p_qsInfo );
    void    signalDebugMessage(QString p_qsMessage);
    void    signalUpdatePatientCard(QString p_qsPatientCard,QString p_qsBlnsHttpId);

private:

    QNetworkAccessManager           *m_httpManager;
    QNetworkRequest                  m_gbRequest;

    QString                          m_qsHost;
    QString                          m_qsPort;
    QString                          m_qsGbUserName;
    QString                          m_qsGbPassword;
    int                              m_inTimeout;

    QString                          m_qsMessage;
    QString                          m_qsInfo;
    QString                          m_qsError;
    int                              m_inTimer;

    QString                          m_qsToken;
    QDateTime                        m_qdtExpiration;
    QString                          m_qsPatientCard;
    QString                          m_qsPatientCardType;

    bool                             m_bErrorOccured;
    bool                             m_bAuthenticationInProgress;

    cBlnsHttpAction::teBlnsHttpAction    m_teBlnsHttpAction;

    unsigned int                     m_uiMessageId;

    QStringList                      m_qslResponseIds;

    void                            _processPCTCreate( QString p_qsPatientCardType );
    void                            _processPCTModify( QString p_qsPatientCardType );
    void                            _processPCTDelete( QString p_qsPatientCardType );
    void                            _processPCRegister( QString p_qsPatientCard );
    void                            _processPCRefill( QString p_qsPatientCard );
    void                            _processPCUse( QString p_qsPatientCard );
    void                            _processPCDelete( QString p_qsPatientCard );
    void                            _processMessage();
    void                            _getTokenExpFromMessage();
    void                            _sendPatientCardData();
    void                            _sendPatientCardTypeData();
    QString                         _getBarcode();
    QString                         _getUnits();
    void                            _activateProcess();
    void                            _prepareProcess();
    bool                            _getResult();
    bool                            _getResponseStatus( QString p_qsResponse );
    QString                         _getResponseId( QString p_qsResponse );

};
//====================================================================================
*/

#endif // BLNS_HTTP_H
