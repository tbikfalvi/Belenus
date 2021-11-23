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

#define HTTP_STATUS_DEFAULT                          -1
#define HTTP_ERROR_INVALID_TOKEN                     -2
#define HTTP_ERROR_WRONG_TOKEN                       -3
#define HTTP_ERROR_MD5_MISMATCH                      -4
#define HTTP_ERROR_INVALID_STUDIO                    -5
#define HTTP_ERROR_SERVER_SQL                        -6
#define HTTP_ERROR_SHA1_MISMATCH                     -7
#define HTTP_ERROR_SHA1_NOT_RECEIVED                 -8
#define HTTP_ERROR_MISSING_STUDIOID                  -9
#define HTTP_ERROR_MISSING_COMMID                   -10
#define HTTP_ERROR_OBSOLETE_TOKEN                   -11
#define HTTP_ERROR_RESULT_NOT_SENT                  -12
#define HTTP_ERROR_TOKEN_REPEAT                     -13
#define HTTP_ERROR_TOKEN_EXPIRED                    -14
#define HTTP_ERROR_MISSING_MAIL_RECIP               -15
#define HTTP_ERROR_MISSING_MAIL_SUBJ                -16
#define HTTP_ERROR_MISSING_MAIL_BODY                -17
#define HTTP_ERROR_MISSING_MAIL_VAR_NAME            -18
#define HTTP_ERROR_MISSING_MAIL_VAR_BARCODE         -19
#define HTTP_ERROR_MISSING_MAIL_VAR_CARDINFO        -20
#define HTTP_ERROR_MISSING_MAIL_VAR_UNITCOUNT       -21
#define HTTP_ERROR_MISSING_MAIL_VAR_DATETIME        -22
#define HTTP_ERROR_INVALID_EMAIL_ADDRESS            -23
#define HTTP_ERROR_LICENCE_INVALID                  -24
#define HTTP_ERROR_LICENCE_ALREADY_REGISTERED       -25
#define HTTP_ERROR_LICENCE_INACTIVE                 -26
#define HTTP_ERROR_LICENCE_CLIENT_CODE_INVALID      -27
#define HTTP_ERROR_LICENCE_SERVER_CODE_INVALID      -28

#define HTTP_ERROR_INVALID_LICENCE_CODE             -70
#define HTTP_ERROR_INVALID_ANSWER                   -98
#define HTTP_ERROR_UNKNOWN                          -99

//====================================================================================
class cBlnsHttpAction
{
public:
    enum teBlnsHttpAction
    {
        HA_DEFAULT = 0,
        HA_AUTHENTICATE,            //  1
        HA_PCSENDDATA,              //  2
        HA_PCUPDATERECORD,          //  3
        HA_PCPROCESSQUEUE,          //  4
        HA_REQUESTDATA,             //  5
        HA_SENDREQUESTSFINISHED,    //  6
        HA_SENDMAILTOSERVER,        //  7
        HA_MAILPROCESSQUEUE,        //  8
        HA_UPDATEMAILRECORD,        //  9
        HA_LICENCE_REGISTER,        // 10
        HA_LICENCE_REACTIVATE,      // 11
        HA_LICENCE_CHECK,           // 12
        HA_LICENCE_CODE_VALIDATE,   // 13
        HA_PROCESSFINISHED          // 14
    };

    static const char *toStr( teBlnsHttpAction p_enGA )
    {
        switch( p_enGA )
        {
            case HA_DEFAULT:                    return "HTTPMSG_00";                                                    break;
            case HA_AUTHENTICATE:               return "HTTPMSG_01 Authentication with server";                         break;
            case HA_PCSENDDATA:                 return "HTTPMSG_02 Send patientcard data to server";                    break;
            case HA_PCUPDATERECORD:             return "HTTPMSG_03 Update patientcard record on server";                break;
            case HA_PCPROCESSQUEUE:             return "HTTPMSG_04 Process waiting patientcard queue";                  break;
            case HA_REQUESTDATA:                return "HTTPMSG_05 Get patientcard data sold online";                   break;
            case HA_SENDREQUESTSFINISHED:       return "HTTPMSG_06 Processing patientcards sold online finished";       break;
            case HA_SENDMAILTOSERVER:           return "HTTPMSG_07 Send waiting mail to server";                        break;
            case HA_MAILPROCESSQUEUE:           return "HTTPMSG_08 Process waiting mails queue";                        break;
            case HA_UPDATEMAILRECORD:           return "HTTPMSG_09 Update mail record";                                 break;
            case HA_LICENCE_REGISTER:           return "HTTPMSG_10 Register licence key";                               break;
            case HA_LICENCE_REACTIVATE:         return "HTTPMSG_11 Reactivate licence key";                             break;
            case HA_LICENCE_CHECK:              return "HTTPMSG_12 Check licence key validity";                         break;
            case HA_LICENCE_CODE_VALIDATE:      return "HTTPMSG_13 Check licence code integrity at licence provider";   break;
            case HA_PROCESSFINISHED:            return "HTTPMSG_99";                                                    break;
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
    void             processWaitingMails();

    void             registerLicenceKey( QString p_qsLicenceString, QString p_qsClientCode );
    void             reactivateLicenceKey( QString p_qsLicenceString, QString p_qsClientCode );
    void             validateLicenceKey( QString p_qsLicenceString, QString p_qsClientCode, QString p_qsServerCode );

    int              getNumberOfWaitingRecords();
    QString          errorMessage();
    QString          settingsInfo();

    unsigned int     licenceId() { return m_uiLicenceId; }
    QString          licenceServerCode() { return m_qsLicenceServerCode; }

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
    QString          m_qsDisplayMessage;
    int              m_nMailTypeId;
    QString          m_qsMailRecipients;
    QString          m_qsMailSubject;
    QString          m_qsMailText;
    QString          m_qsMailVarName;
    QString          m_qsMailVarBarcode;
    QString          m_qsMailVarCardInfo;
    QString          m_qsMailVarDateTime;
    QString          m_qsMailVarUnitCount;
    QString          m_qsMailSha1;
    QString          m_qsLicenceStringValidate;
    QString          m_qsLicenceClientCode;
    QString          m_qsLicenceServerCode;

    QDomDocument    *obResponseXML;

    cBlnsHttpAction::teBlnsHttpAction           m_teBlnsHttpProcess;
    vector<cBlnsHttpAction::teBlnsHttpAction>   m_vrHttpActions;

    bool            _downloadFile( QString p_qsFileName );
    void            _httpStartProcess();
    void            _httpExecuteProcess();
    void            _httpProcessResponse();
    void            _httpGetToken();
    void            _httpSendCardData();
    void            _httpSendMailToServer();
    void            _readTokenFromFile();
    void            _sendProcessFinished();
    void            _readPCResponseFromFile();
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
    void            _updateMailRecord();
    void            _readMailResponseFromFile();
    QString         _getNameForPatientCardType( unsigned int p_uiPatientCardTypeId );
    void            _httpRegisterLicence();
    void            _httpReactivateLicence();
//    void            _httpChangeLicence();
    void            _httpCheckLicence();
    void            _httpValidateLicenceIntegrity();
    bool            _processLicence();
    bool            _processLicenceIntegrity();

signals:

    void             signalErrorOccured();
    void             signalActionProcessed( QString p_qsInfo );
    void             signalStepProgress();
//    void             signalHideProgress();
    void             signalHttpProcessSuspended();
    void             signalHttpProcessDisabled();
    void             signalPatientCardUpdated( unsigned int p_uiPatientCardId, QString p_qsBarcode );
    void             signalDisplayNotification( QString p_qsMessage );
    void             signalHideProgress( QString p_qsInfo );

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
