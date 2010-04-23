//====================================================================================
//
// Belenus Server Admin alkalmazas © Pagony Multimedia Studio Bt - 2010
//
//====================================================================================
//
// Filename    : bs_connection.h
// AppVersion  : 1.0
// FileVersion : 1.0
// Author      : Bikfalvi Tamas
//
//====================================================================================
// Interneten kereszuli kapcsolat a BelenusServer alkalmazassal
//====================================================================================

#ifndef BS_CONNECTION_H
#define BS_CONNECTION_H

//====================================================================================

#include <QString>
#include <QtNetwork>

//====================================================================================
class BelenusServerConnection : public QThread
{
    Q_OBJECT

public:
    QString      m_qsAction;
    QString      m_qsCommand;
    QString      m_qsParameter;

    BelenusServerConnection( QTcpSocket *p_pTcpSocket );
    ~BelenusServerConnection();

    void run();
    void init( QString p_qsClientSerial,
               QString p_qsHostAddress,
               QString p_qsHostPort );
    bool isConnectionActive();
    bool isSerialValid();

public slots:
    void msgReceive();
    void displayError(QAbstractSocket::SocketError socketError);

private:
    QTcpSocket  *m_ptcpSocket;
    QByteArray   m_obPwdHash;
    quint16      blockSize;

    bool         m_bConnectionInProgress;
    bool         m_bCheckRegistration;

    bool         m_bSerialValid;
    QString      m_qsHostAddress;
    QString      m_qsHostPort;
    QString      m_qsMessageIN;
    QString      m_qsMessageOUT;
    QString      m_qsMessageChecksumIN;
    QString      m_qsMessageChecksumOUT;
    int          m_nProcessStep;
    QString      m_qsClientSerial;

    void connectToServer();
    void disconnectFromServer();
    void checkClientRegistration();
    void sendMessage();
    void msgSend( QString qsMsg );
    bool checkMessageConsistency();
    QString getMessageBody();
    QString getMessageAction();
    void ProcessMessage();
    void ProcessClientAuthorization();

private slots:

};
//====================================================================================

#endif // BS_CONNECTION_H
