//====================================================================================
//
// Belenus Server alkalmazas © Pagony Multimedia Studio Bt - 2010
//
//====================================================================================
//
// Filename    : bs_thread.h
// AppVersion  : 1.0
// FileVersion : 1.0
// Author      : Bikfalvi Tamas
//
//====================================================================================
// Thread osztaly leirasa
//====================================================================================

#ifndef BS_THREAD_H
#define BS_THREAD_H

//====================================================================================
#include <QThread>
#include <QTcpSocket>
#include <QDataStream>
#include <QSqlQuery>

//====================================================================================
class BelenusThread : public QThread
{
   Q_OBJECT

public:
   BelenusThread(QTcpSocket *pTcpSocket, QObject *parent=0);
   ~BelenusThread();
   void run();

public slots:
   void msgReceive();
   void clientDisconnected();

signals:
   void error(QTcpSocket::SocketError socketError);

private:
   QTcpSocket  *m_pTcpSocket;             // thread sajat TCP Socket-jere mutato pointer
   quint16      blockSize;                // uzenet fogadasahoz szukseges valtozo
   QString      strMessage;               // fogadott uzenet
   int          nMsgCorruptCount;         // hibásan fogadott üzenetek száma

   bool         m_bClientAuthorized;
   bool         m_bAdminIdentified;
   bool         m_bPasswordRequested;
   bool         m_bAdminAuthorized;
   bool         m_bClientConnected;

   bool         m_bArchiveProcessActive;

   QString      m_sClientSerial;
   QString      m_sClientSerialEncrypted;
   unsigned int m_nClientId;

   QString      m_qsSQLCommand;
   QSqlQuery   *m_poQuery;
   QByteArray   m_obPwdHash;
   QString      m_qsMessageChecksum;

   void msgSend(QString str);
   void msgProcess();

   bool checkMessageConsistency();
   bool isClientAuthorized();
   void AuthorizeClient();
   void AuthorizeAdmin();

   bool isAdminAuthorized();
   bool isAdminIdentified();
   bool isPasswordRequested();

   void ProcessAdminMessage();
   void ProcessClientMessage();
   void ProcessCONMessage();
   void ProcessARCMessage();
   void ProcessSQLMessage();
   void ProcessPRCMessage();

   void ProcessBYEMessage();

   QString getMessageBody();
   QString getMessageAction();
};
//====================================================================================

#endif // BS_THREAD_H
