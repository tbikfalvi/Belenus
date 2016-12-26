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
#include <QDomDocument>

#include <vector>

using namespace std;

#define HTTP_STATUS_DEFAULT              -1
#define HTTP_ERROR_INVALID_TOKEN         -2
#define HTTP_ERROR_WRONG_TOKEN           -3
#define HTTP_ERROR_MD5_MISMATCH          -4
#define HTTP_ERROR_INVALID_STUDIO        -5
#define HTTP_ERROR_SERVER_SQL            -6
#define HTTP_ERROR_SHA1_MISMATCH         -7
#define HTTP_ERROR_SHA1_NOT_RECEIVED     -8
#define HTTP_ERROR_MISSING_STUDIOID      -9
#define HTTP_ERROR_MISSING_COMMID       -10
#define HTTP_ERROR_OBSOLETE_TOKEN       -11
#define HTTP_ERROR_RESULT_NOT_SENT      -12
#define HTTP_ERROR_UNKNOWN              -99

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
        HA_REQUESTDATA,
        HA_SENDREQUESTSFINISHED,
        HA_PROCESSFINISHED
    };

    static const char *toStr( teBlnsHttpAction p_enGA )
    {
        switch( p_enGA )
        {
            case HA_DEFAULT:                    return "HTTPMSG_00";                                                break;
            case HA_AUTHENTICATE:               return "HTTPMSG_01 Authentication with server";                     break;
            case HA_PCSENDDATA:                 return "HTTPMSG_02 Send patientcard data to server";                break;
            case HA_PCUPDATERECORD:             return "HTTPMSG_03 Update patientcard record on server";            break;
            case HA_PROCESSQUEUE:               return "HTTPMSG_04 Process waiting queue";                          break;
            case HA_REQUESTDATA:                return "HTTPMSG_05 Get patientcard data sold online";               break;
            case HA_SENDREQUESTSFINISHED:       return "HTTPMSG_06 Processing patientcards sold online finished";   break;
            case HA_PROCESSFINISHED:            return "HTTPMSG_99";                                                break;
            default:                            return "HTTPMSGERR";
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
    void             setServerAddress( QString p_qsServerAddress );
    void             setStudioLicence( unsigned int p_uiLicenceId, QString p_qsLicenceString );
    void             setOnlinePCType( unsigned int p_uiId, int p_nPrice, int p_nUnits, int p_nUnitTime );
    void             setOnlinePaymentMethod( unsigned int p_uiPaymentMethod );
    void             setCommunicationEnabled(bool p_bHttpEnabled = true );
    void             setCommunicationSuspended( bool p_bHttpSuspended = true );

    void             checkHttpServerAvailability();
    void             sendPatientCardData( QString p_qsBarcode, QString p_qsPatientCardData, bool p_bSendNow = true );
    void             processWaitingCardData();
    void             getPatientCardsSoldOnline();

    int              getNumberOfWaitingRecords();
    QString          errorMessage();
    QString          settingsInfo();

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
    unsigned int     m_uiRecordId;
    unsigned int     m_uiCommId;
    unsigned int     m_uiCommIdNew;

    QString          m_qsServerAddress;
    bool             m_bIsHttpEnabled;
    bool             m_bIsHttpSuspended;
    unsigned int     m_uiLicenceId;
    QString          m_qsLicenceString;
    unsigned int     m_uiPatientCardTypeId;
    int              m_nPatientCardTypePrice;
    int              m_nPatientCardTypeUnits;
    int              m_nPatientCardTypeUnitTime;
    int              m_uiLedgerTypeId;
    QString          m_qsLedgerComment;
    unsigned int     m_uiPaymentMethod;
    bool             m_bGetOnlinePCProcessed;
    QStringList      m_qslProcessedRecordIds;

    QDomDocument    *obResponseXML;

    cBlnsHttpAction::teBlnsHttpAction           m_teBlnsHttpProcess;
    vector<cBlnsHttpAction::teBlnsHttpAction>   m_vrHttpActions;

    bool            _downloadFile( QString p_qsFileName );
    void            _httpStartProcess();
    void            _httpExecuteProcess();
    void            _httpProcessResponse();
    void            _httpGetToken();
    void            _httpSendCardData();
    void            _readTokenFromFile();
    void            _sendProcessFinished();
    void            _readResponseFromFile();
    void            _updateProcessedRecord();
    void            _httpGetOnlineRecords();
    void            _httpConfirmRequestedData();
    bool            _processCommXML();
    bool            _processCommResponse( QByteArray p_qsResponse );
    bool            _processResponse();
    unsigned int    _saveGuest( QString p_qsName, QString p_qsUniqueId, QString p_qsEmail );
    unsigned int    _savePatientCard( QString p_qsBarcode, QString p_qsValidDateTo, QString p_qsUnitCount, unsigned int p_uiPatientId );
    unsigned int    _saveOnlineSell( unsigned int p_uiPatientCardId, QString p_qsBarcode, unsigned int p_uiPatientId, QString p_qsLedgerTime );
    void            _savePatientCardUnits( QString p_qsUnitCount, unsigned int p_uiPatientCardId, QString p_qsValidDateTo, unsigned int p_uiLedgerId );
    QString         _bytearrayToString( QString p_qsString );

signals:

    void             signalErrorOccured();
    void             signalActionProcessed( QString p_qsInfo );
    void             signalStepProgress();
    void             signalHideProgress();
    void             signalHttpProcessSuspended();
    void             signalHttpProcessDisabled();
    void             signalPatientCardUpdated( unsigned int p_uiPatientCardId, QString p_qsBarcode );

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
