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
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>

//====================================================================================
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
    void    gibbigSendPatientCard( QString p_qsBarcode );

    void    gibbigClearError()          {   m_bErrorOccured = false;    }
    bool    gibbigIsErrorOccured()      {   return m_bErrorOccured;     }

protected:

    void    timerEvent( QTimerEvent *p_poEvent );

protected slots:

    void    slotRestRequestFinished(QNetworkReply *p_gbReply);

private:

    QNetworkAccessManager   *m_gbRestManager;
    QNetworkRequest          m_gbRequest;

    QString                  m_qsHost;
    QString                  m_qsPort;
    QString                  m_qsGbUserName;
    QString                  m_qsGbPassword;
    int                      m_inTimeout;

    QString                  m_qsMessage;
    QString                  m_qsError;
    int                      m_inTimer;

    bool                     m_bErrorOccured;
    bool                     m_bAuthenticationInProgress;

};
//====================================================================================

#endif // GIBBIG_H
