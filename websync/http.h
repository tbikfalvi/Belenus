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
    void             setServerAddress( QString p_qsServerAddress );
    void             setStudioLicenceString( QString p_qsLicenceString );

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

    QString          m_qsServerAddress;
    bool             m_bIsHttpEnabled;
    bool             m_bIsHttpSuspended;
    unsigned int     m_uiLicenceId;
    QString          m_qsLicenceString;

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
    void             signalHttpProcessSuspended();
    void             signalHttpProcessDisabled();

private slots:
    void            _slotHttpRequestFinished(int requestId, bool error);
    void            _slotUpdateDataReadProgress(int bytesRead, int totalBytes);
    void            _slotReadResponseHeader(const QHttpResponseHeader &responseHeader);
    void            _slotAuthenticationRequired(const QString &, quint16, QAuthenticator *);
#ifndef QT_NO_OPENSSL
    void            _slotSslErrors(const QList<QSslError> &errors);
#endif

};

#endif // BLNS_HTTP_H
