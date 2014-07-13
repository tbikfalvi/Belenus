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
        GA_AUTHENTICATE1,
        GA_AUTHENTICATE2,
        GA_AUTHENTICATE3,
        GA_AUTHENTICATE4,
        GA_PCREGISTER,
        GA_PCREFILL,
        GA_PCUSE
    };

    static const char *toStr( teGibbigAction p_enGA )
    {
        switch( p_enGA )
        {
            case GA_DEFAULT:        return "Unidentified";              break;
            case GA_AUTHENTICATE1:  return "Authenticate";              break;
            case GA_AUTHENTICATE2:  return "Authenticate";              break;
            case GA_AUTHENTICATE3:  return "Authenticate";              break;
            case GA_AUTHENTICATE4:  return "Authenticate";              break;
            case GA_PCREGISTER:     return "PatientCardRegistration";   break;
            case GA_PCREFILL:       return "PatientCardRefill";         break;
            case GA_PCUSE:          return "PatientCardUsage";          break;
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

    void    gibbigAuthenticate( cGibbigAction::teGibbigAction p_teGibbigAction = cGibbigAction::GA_AUTHENTICATE1 );
    void    gibbigPCRegister( QString p_qsPatientCard );
    void    gibbigPCRefill( QString p_qsPatientCard );
    void    gibbigPCUse( QString p_qsPatientCard );

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

    bool                             m_bErrorOccured;
    bool                             m_bAuthenticationInProgress;

    cGibbigAction::teGibbigAction    m_teGibbigAction;

    void                            _processMessage();
    void                            _getTokenExpFromMessage();
    void                            _sendPatientCardData();
    QString                         _getBarcode();
    QString                         _getUnits();

};
//====================================================================================

#endif // GIBBIG_H
