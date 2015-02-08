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

#include <vector>

using namespace std;

#define HTTP_STATUS_DEFAULT         -1
#define HTTP_ERROR_INVALID_TOKEN    -2
#define HTTP_ERROR_WRONG_TOKEN      -3
#define HTTP_ERROR_MD5_MISMATCH     -4
#define HTTP_ERROR_INVALID_STUDIO   -5
#define HTTP_ERROR_SERVER_SQL       -6
#define HTTP_ERROR_UNKNOWN          -99

//====================================================================================
class cBlnsHttpAction
{
public:
    enum teBlnsHttpAction
    {
        HA_DEFAULT = 0,
        HA_AUTHENTICATE,
        HA_PCSENDDATA,
        HA_PCUPDATERECORD,
        HA_PROCESSQUEUE,
        HA_PROCESSFINISHED
    };

    static const char *toStr( teBlnsHttpAction p_enGA )
    {
        switch( p_enGA )
        {
            case HA_DEFAULT:            return "HTTPMSG_00";    break;
            case HA_AUTHENTICATE:       return "HTTPMSG_01";    break;
            case HA_PCSENDDATA:         return "HTTPMSG_02";    break;
            case HA_PCUPDATERECORD:     return "HTTPMSG_03";    break;
            case HA_PROCESSQUEUE:       return "HTTPMSG_04";    break;
            case HA_PROCESSFINISHED:    return "HTTPMSG_99";    break;
            default:                    return "HTTPMSGERR";
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

    void             setTimeout( const int p_inTimeout );
    void             sendPatientCardData( QString p_qsBarcode, QString p_qsPatientCardData, bool p_bSendNow = true );
    void             processWaitingCardData();
    void             checkHttpServerAvailability();
    QString          errorMessage();
    int              getNumberOfWaitingRecords();

protected:

    void             timerEvent( QTimerEvent *p_poEvent );

private:

    QHttp           *obHttp;
    QFile           *obFile;
    bool             m_httpRequestAborted;
    int              m_httpGetId;

    QString          m_qsError;
    int              m_inTimeout;
    int              m_inTimer;
    QString          m_qsToken;
    int              m_inHttpProcessStep;
    QString          m_qsBarcode;
    QString          m_qsCardData;
    QString          m_qsCardDataSendResponse;
    unsigned int     m_uiRecordId;

    cBlnsHttpAction::teBlnsHttpAction           m_teBlnsHttpProcess;
    vector<cBlnsHttpAction::teBlnsHttpAction>   m_vrHttpActions;

    bool            _downloadFile( QString p_qsFileName );
    void            _httpStartProcess();
    void            _httpExecuteProcess();
    void            _httpProcessResponse();
    void            _httpGetToken();
    void            _httpSendCardData();
    void            _readTokenFromFile();
    void            _readCardSendResponseFromFile();
    void            _sendProcessFinished();
    void            _readResponseFromFile();
    void            _updateProcessedRecord();

signals:

    void             signalErrorOccured();
    void             signalActionProcessed( QString p_qsInfo );
    void             signalStepProgress();
    void             signalHideProgress();

private slots:
    void            _slotHttpRequestFinished(int requestId, bool error);
    void            _slotUpdateDataReadProgress(int bytesRead, int totalBytes);
    void            _slotReadResponseHeader(const QHttpResponseHeader &responseHeader);
    void            _slotAuthenticationRequired(const QString &, quint16, QAuthenticator *);
#ifndef QT_NO_OPENSSL
    void            _slotSslErrors(const QList<QSslError> &errors);
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
