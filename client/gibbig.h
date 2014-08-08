//====================================================================================
//
// Belenus Kliens alkalmazas (c) Pagony Multimedia Studio Bt - 2014
//
//====================================================================================
//
// Filename    : gibbig.h
// AppVersion  : 1.0
// FileVersion : 1.0
// Author      : Bikfalvi Tamas
//
//====================================================================================
// Gibbig rendszerrel kapcsolatos reszek implementalasa
//====================================================================================

#ifndef GIBBIG_H
#define GIBBIG_H

//====================================================================================

#include <QObject>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkRequest>
#include <QtNetwork/QNetworkReply>
#include <QDateTime>

//====================================================================================
class cGibbigAction
{
public:
    enum teGibbigAction
    {
        GA_DEFAULT = 0,
        GA_AUTHENTICATE,
        GA_PCTCREATE,
        GA_PCTMODIFY,
        GA_PCTDELETE,
        GA_PCREGISTER,
        GA_PCREFILL,
        GA_PCUSE,
        GA_PCDELETE
    };

    static const char *toStr( teGibbigAction p_enGA )
    {
        switch( p_enGA )
        {
            case GA_DEFAULT:        return "GBMSG_01 Unidentified ";              break;
            case GA_AUTHENTICATE:   return "GBMSG_02 Authenticate ";              break;
            case GA_PCTCREATE:      return "GBMSG_03 PatientCardTypeCreate ";     break;
            case GA_PCTMODIFY:      return "GBMSG_04 PatientCardTypeModify ";     break;
            case GA_PCTDELETE:      return "GBMSG_05 PatientCardTypeDelete ";     break;
            case GA_PCREGISTER:     return "GBMSG_06 PatientCardRegistration ";   break;
            case GA_PCREFILL:       return "GBMSG_07 PatientCardRefill ";         break;
            case GA_PCUSE:          return "GBMSG_08 PatientCardUsage ";          break;
            case GA_PCDELETE:       return "GBMSG_09 PatientCardDelete ";         break;
            default:                return "INVALID";
        }
    }
};


class cGibbig : public QObject
{
    Q_OBJECT

public:

    cGibbig();
    ~cGibbig();

    void    setHost( const QString p_qsHost );
    void    setPort( const QString p_qsPort );
    void    setUserName( const QString p_qsUserName );
    void    setPassword( const QString p_qsPassword );
    void    setTimeout( const int p_inTimeout );

    void    gibbigAuthenticate();
    void    gibbigPCTCreate( QString p_qsPatientCardType );
    void    gibbigPCTModify( QString p_qsPatientCardType );
    void    gibbigPCTDelete( QString p_qsPatientCardType );
    void    gibbigPCRegister( QString p_qsPatientCard );
    void    gibbigPCRefill( QString p_qsPatientCard );
    void    gibbigPCUse( QString p_qsPatientCard );
    void    gibbigPCDelete( QString p_qsPatientCard );

    void    gibbigClearError()          {   m_bErrorOccured = false; m_qsError = "";    }
    bool    gibbigIsErrorOccured()      {   return m_bErrorOccured;                     }
    QString gibbigErrorStr()            {   return m_qsError;                           }

protected:

    void    timerEvent( QTimerEvent *p_poEvent );

protected slots:

    void    slotRestRequestFinished(QNetworkReply *p_gbReply);

signals:

    void    signalErrorOccured();
    void    signalActionProcessed( QString p_qsInfo );
    void    signalDebugMessage(QString p_qsMessage);

private:

    QNetworkAccessManager           *m_gbRestManager;
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

    cGibbigAction::teGibbigAction    m_teGibbigAction;

    unsigned int                     m_uiMessageId;

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

};
//====================================================================================

#endif // GIBBIG_H
