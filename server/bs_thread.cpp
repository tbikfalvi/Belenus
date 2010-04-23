//====================================================================================
//
// Belenus Server alkalmazas © Pagony Multimedia Studio Bt - 2010
//
//====================================================================================
//
// Filename    : bs_thread.cpp
// AppVersion  : 1.0
// FileVersion : 1.0
// Author      : Bikfalvi Tamas
//
//====================================================================================
// __TO_BE_COMPLETED__  ott ahol meg csinalni kell valamit
//====================================================================================

#include <QMessageBox>
#include <QCryptographicHash>
#include <QRegExp>

#include "belenusserver.h"
#include "../framework/belenusmessages.h"
#include "bs_thread.h"

//====================================================================================

#define REGISTER_IDENTIFIER             "7c01fcbe9cab6ae14c98c76cf943a7b2be6a7922"

//====================================================================================
// BelenusThread::BelenusThread
//------------------------------------------------------------------------------------
// Konstruktor fuggveny. Kommunikacio inditasakor szukseges parameterek inicializalasa
// Socket osztalyra mutato pointer atvetele a szulo foosztalytol
//------------------------------------------------------------------------------------
BelenusThread::BelenusThread(QTcpSocket *pTcpSocket, QObject *parent) : QThread(parent)
{
   m_pTcpSocket         = pTcpSocket;
   blockSize            = 0;
   nMsgCorruptCount     = 0;

   m_bClientAuthorized  = false;
   m_bClientConnected   = false;
   m_bAdminIdentified   = false;
   m_bPasswordRequested = false;
   m_bAdminAuthorized   = false;

   g_nConnectionCount++;
   g_msgLabel->setText( QString(tr("There are %1 client(s) connected to the server")).arg(g_nConnectionCount) );
   msgSend( QString("INF_CONNECTION_RECEIVED_") );
}
//====================================================================================
// BelenusThread::BelenusThread
//------------------------------------------------------------------------------------
// Destruktor fuggveny. Kommunikacio, megnyitott kapcsolatok lezarasa. Memoriaterulet
// felszabaditasa
//------------------------------------------------------------------------------------
BelenusThread::~BelenusThread()
{
    if( m_bClientConnected )
    {
        g_obLogger << cSeverity::INFO;
        g_obLogger << "Client disconnected - Serial [" << m_sClientSerial.toStdString() << "] ";
        g_obLogger << "Id [" << m_nClientId << "] ";
        g_obLogger << cQTLogger::EOM;
    }
    g_nConnectionCount--;
    g_msgLabel->setText( QString(tr("There are %1 client(s) connected to the server")).arg(g_nConnectionCount) );
}
//====================================================================================
// BelenusThread::run
//------------------------------------------------------------------------------------
// Thread inditasakor elsokent elindulo fuggveny, esemenyek, uzenetek regisztralasa
//------------------------------------------------------------------------------------
void BelenusThread::run()
{
   connect( m_pTcpSocket, SIGNAL(readyRead()),      this, SLOT(msgReceive())         );
   connect( m_pTcpSocket, SIGNAL(disconnected()),   this, SLOT(clientDisconnected()) );

   exec();
}
//====================================================================================
// BelenusThread::msgReceive
//------------------------------------------------------------------------------------
// Meglevo TCP Socket kapcsolaton erkezo uzenet fogadasa es atadasa az uzenet
// feldolgozo fuggvenynek
//------------------------------------------------------------------------------------
void BelenusThread::msgReceive()
{
   QDataStream in(m_pTcpSocket);
   in.setVersion(QDataStream::Qt_4_0);
   strMessage = "";

   if (blockSize == 0)
   {
      if( m_pTcpSocket->bytesAvailable() < (int)sizeof(quint16) )
         return;
      in >> blockSize;
   }

   if (m_pTcpSocket->bytesAvailable() < blockSize)
      return;

   in >> strMessage;
   blockSize = 0;

   g_obLogger << cSeverity::DEBUG;
   g_obLogger << "MsgIN  [" << strMessage.toStdString() << "] ";
   g_obLogger << cQTLogger::EOM;

   // Uzenet ellenorzese           Az uzenet serult, vagy nem korrekt
   if( checkMessageConsistency() ) return;

   // Kuldo vizsgalata         Kuldo meg nem volt ellenorizve
   if( !isClientAuthorized() ) AuthorizeClient();

   // Uzenet feldolgozasa
   msgProcess();
}
//====================================================================================
// BelenusThread::clientDisconnected
//------------------------------------------------------------------------------------
// Kliens lekapcsolodasakor meghivott fuggveny. Thread lezarasa.
//------------------------------------------------------------------------------------
void BelenusThread::clientDisconnected()
{
   quit();
}
//====================================================================================
// BelenusThread::msgSend
//------------------------------------------------------------------------------------
// Meglevo TCP Socket kapcsolaton parameterben megadott uzenet elkuldese
//------------------------------------------------------------------------------------
// Parameter:
//          IN strMsg - Kikuldendo uzenet
//------------------------------------------------------------------------------------
void BelenusThread::msgSend(QString strMsg)
{
   QByteArray block;
   QDataStream out(&block, QIODevice::WriteOnly);

   QString strMsgOut = strMsg.leftJustified(168,' ');

   QByteArray obPwdHash = QCryptographicHash::hash( strMsgOut.toAscii(), QCryptographicHash::Md5 );
   strMsgOut += QString( obPwdHash.toHex() );

   out.setVersion(QDataStream::Qt_4_0);
   out << (quint16)0;
   out << strMsgOut;
   out.device()->seek(0);
   out << (quint16)(block.size() - sizeof(quint16));

   g_obLogger << cSeverity::DEBUG;
   g_obLogger << "MsgOUT [" << strMsgOut.toStdString() << "] ";
   g_obLogger << cQTLogger::EOM;

   m_pTcpSocket->write(block);
}
//====================================================================================
// BelenusThread::msgProcess
//------------------------------------------------------------------------------------
// Kapott uzenet feldolgozasa a kornyezeti parameterek alapjan
//------------------------------------------------------------------------------------
void BelenusThread::msgProcess()
{
   if( !isClientAuthorized() )
   {
      msgSend( QString("ERR_UNREGISTERED_CLIENT_") );
      quit();
   }
   else
   {
      if( isAdminIdentified() )
      {
         if( !isAdminAuthorized() )
         {
            if( !isPasswordRequested() )
            {
               m_bPasswordRequested = true;
               msgSend( QString("REQ_ADMIN_PASSWORD_") );
            }
            else
            {
               AuthorizeAdmin();
            }
         }
         else
         {
            ProcessAdminMessage();
         }
      }
      else
      {
         ProcessClientMessage();
      }
   }
}
//====================================================================================
// BelenusThread::checkMessageConsistency
//------------------------------------------------------------------------------------
// Ellenorzi a beerkezett uzenetet, nem serult-e. Ha igen, egy belso szamlalot
// megnovel. Adott ertek elerese utan checksum error hibauzenetet kuld a kliens-nek
// es megszakitja a kommunikaciot
//------------------------------------------------------------------------------------
bool BelenusThread::checkMessageConsistency()
{
    bool isMessageCorrupt = false;

    m_obPwdHash = QCryptographicHash::hash( strMessage.left(strMessage.length()-32).toAscii(), QCryptographicHash::Md5 );
    m_qsMessageChecksum = QString( m_obPwdHash.toHex() );

    if( strMessage.length() != 200 )
    {
        isMessageCorrupt = true;
        msgSend( QString("ERR_MESSAGE_LENGTH_INVALID_") );
    }
    else if( m_qsMessageChecksum.compare( strMessage.right(32) ) != 0 )
    {
        isMessageCorrupt = true;
        msgSend( QString("ERR_MESSAGE_CHEKCSUM_MISMATCH_") );
    }

    if( isMessageCorrupt )
    {
        nMsgCorruptCount++;
    }

    if( nMsgCorruptCount > g_poPrefs->getMaxCorruptMessageCount() )
    {
        quit();
    }

    return isMessageCorrupt;
}
//====================================================================================
// BelenusThread::isClientAuthorized
//------------------------------------------------------------------------------------
// Megmondja, hogy a TCP kapcsolat kliens alkalmazasa regisztralt Belenus kliens-e
// Visszateresi ertek TRUE, ha igen, FALSE ha nem. A visszateresi erteket az
// AuthorizeClient() fuggveny allitja be az elso bejovo uzenet alapjan.
//------------------------------------------------------------------------------------
bool BelenusThread::isClientAuthorized()
{
   return m_bClientAuthorized;
}
//====================================================================================
// BelenusThread::AuthorizeClient
//------------------------------------------------------------------------------------
// Eldonti, hogy a TCP kapcsolatban levo kliens regisztralt Belenus kliens alkalmazas,
// vagy valaki mas. Ha nem regisztralt kliens, a belso valtozo erteke FALSE marad.
// Ez a fgv. csak a kapcsolat letrejottekor hivodik meg.
//------------------------------------------------------------------------------------
void BelenusThread::AuthorizeClient()
{
    // Elso uzenetnek 'CON'-al kell kezdodnie
    if( !strMessage.startsWith("CON") )
    {
        msgSend( QString("ERR_CONNECTION_REJECTED_") );
        quit();
        return;
    }
    // Kliens azonosító kiolvasása és ellenorzése az adatbázisban
    m_obPwdHash = QCryptographicHash::hash( getMessageBody().toAscii(), QCryptographicHash::Sha1 );
    m_qsSQLCommand = QString( "SELECT clientId FROM clients WHERE code='" )+QString(m_obPwdHash.toHex())+QString( "'" );
    m_poQuery = g_poDB->executeQTQuery( m_qsSQLCommand );
    if( m_poQuery->size() == 1 )
    {
        m_bClientAuthorized = true;
        m_poQuery->first();
        m_sClientSerial = getMessageBody();
        m_sClientSerialEncrypted = QString(m_obPwdHash.toHex());
        m_nClientId = m_poQuery->value( 0 ).toUInt();

        g_obLogger << cSeverity::INFO;
        g_obLogger << "Client connected - Serial [" << m_sClientSerial.toStdString() << "] ";
        g_obLogger << "Id [" << m_nClientId << "] ";
        g_obLogger << cQTLogger::EOM;

        m_qsSQLCommand = QString("UPDATE clients set lastLogin=NOW() WHERE code='")+m_sClientSerialEncrypted+QString( "'" );
        g_poDB->executeQTQuery( m_qsSQLCommand );
    }

   // Akar sikerult a kliens kiolvasasa, akar nem, admin kapcsolodas ellenorzese
   if( getMessageBody().compare( "_ADMIN_REQUEST_LOGIN_" ) == 0 )
   {
      m_bClientAuthorized = true;
      m_bAdminIdentified = true;
   }
}
//====================================================================================
// BelenusThread::isAdminAuthorized
//------------------------------------------------------------------------------------
bool BelenusThread::isAdminAuthorized()
{
   return m_bAdminAuthorized;
}
//====================================================================================
// BelenusThread::getMessageBody
//------------------------------------------------------------------------------------
QString BelenusThread::getMessageBody()
{
    QString qsTemp = strMessage.mid(3,160).trimmed();

    if( qsTemp.right(8).compare("#MSGEND#") == 0 )
        qsTemp.chop(8);

    return qsTemp;
}
//====================================================================================
// BelenusThread::getMessageAction
//------------------------------------------------------------------------------------
QString BelenusThread::getMessageAction()
{
   return strMessage.left(3);
}
//====================================================================================
// BelenusThread::isAdminIdentified
//------------------------------------------------------------------------------------
bool BelenusThread::isAdminIdentified()
{
   return m_bAdminIdentified;
}
//====================================================================================
// BelenusThread::isPasswordRequested
//------------------------------------------------------------------------------------
bool BelenusThread::isPasswordRequested()
{
   return m_bPasswordRequested;
}
//====================================================================================
// BelenusThread::isPasswordRequested
//------------------------------------------------------------------------------------
void BelenusThread::AuthorizeAdmin()
{
   if( getMessageAction().compare("PSW") == 0 &&
       getMessageBody().compare( REGISTER_IDENTIFIER ) == 0 )
   {
      m_bAdminAuthorized = true;
      msgSend( QString("INF_ADMIN_AUTHORIZED_") );
   }
   else
   {
      msgSend( QString("ERR_UNREGISTERED_ADMIN_") );
      quit();
   }
}
//====================================================================================
// BelenusThread::ProcessAdminMessage
//------------------------------------------------------------------------------------
void BelenusThread::ProcessAdminMessage()
{
    QStringList strMsgPart = getMessageBody().split("#");

    //---------------------------------------------------------------------------------
    // Kliens regisztráció
    //---------------------------------------------------------------------------------
    if( getMessageAction().compare("REQ") == 0 && strMsgPart[0].compare( REQ_REGISTER_CLIENT_SERIAL ) == 0 )
    {
        m_obPwdHash = QCryptographicHash::hash( strMsgPart[1].toAscii(), QCryptographicHash::Sha1 );
        m_qsSQLCommand = QString( "SELECT clientId FROM clients WHERE code='" )+QString(m_obPwdHash.toHex())+QString( "'" );
        m_poQuery = g_poDB->executeQTQuery( m_qsSQLCommand );
        if( m_poQuery->size() == 1 )
        {
            msgSend( QString("ERR_CLIENT_SERIAL_ALREADY_REGISTERED_#")+strMsgPart[1] );
            return;
        }
        try
        {
            m_qsSQLCommand = QString( "INSERT INTO clients (clientId, code, dateCreated, lastLogin) VALUES (NULL, '%1', NOW(), NOW());" ).arg( QString(m_obPwdHash.toHex()) );
            g_poDB->executeQTQuery( m_qsSQLCommand );
            msgSend( QString("INF_CLIENT_SERIAL_REGISTERED_#")+strMsgPart[1] );
        }
        catch( cSevException &e )
        {
            g_obLogger << e.severity();
            g_obLogger << e.what();
            g_obLogger << cQTLogger::EOM;
            msgSend( QString("ERR_UNABLE_TO_REGISTER_CLIENT_SERIAL_#")+strMsgPart[1] );
        }
    }
    //---------------------------------------------------------------------------------
    // Kliens törlés
    //---------------------------------------------------------------------------------
    if( getMessageAction().compare("REQ") == 0 && strMsgPart[0].compare( REQ_DELETE_CLIENT_SERIAL ) == 0 )
    {
        m_obPwdHash = QCryptographicHash::hash( strMsgPart[1].toAscii(), QCryptographicHash::Sha1 );
        try
        {
            m_qsSQLCommand = QString( "DELETE from clients WHERE code='%1'" ).arg( QString(m_obPwdHash.toHex()) );
            g_poDB->executeQTQuery( m_qsSQLCommand );
            msgSend( QString("INF_CLIENT_SERIAL_DELETED_#")+strMsgPart[1] );
        }
        catch( cSevException &e )
        {
            g_obLogger << e.severity();
            g_obLogger << e.what();
            g_obLogger << cQTLogger::EOM;
            msgSend( QString("ERR_UNABLE_TO_DELETE_CLIENT_SERIAL_#")+strMsgPart[1] );
        }
    }
}
//====================================================================================
// BelenusThread::ProcessClientMessage
//------------------------------------------------------------------------------------
void BelenusThread::ProcessClientMessage()
{
    //---------------------------------------------------------------------------------
    // Kliens kapcsolódás
    //---------------------------------------------------------------------------------
    if( getMessageAction().compare("CON") == 0 ) ProcessCONMessage();

    //---------------------------------------------------------------------------------
    // Archiválásra adatküldés megkezdése
    //---------------------------------------------------------------------------------
    else if( getMessageAction().compare("ARC") == 0 ) ProcessARCMessage();

    //---------------------------------------------------------------------------------
    // SQL szöveg fogadása
    //---------------------------------------------------------------------------------
    else if( getMessageAction().compare("SQL") == 0 ) ProcessSQLMessage();

    //---------------------------------------------------------------------------------
    // SQL parancs végrehajtása
    //---------------------------------------------------------------------------------
    else if( getMessageAction().compare("PRC") == 0 ) ProcessPRCMessage();

    //---------------------------------------------------------------------------------
    // Kapcsolat lezárása
    //---------------------------------------------------------------------------------
    else if( getMessageAction().compare("BYE") == 0 ) ProcessBYEMessage();
}
//====================================================================================
// BelenusThread::ProcessCONMessage
//------------------------------------------------------------------------------------
void BelenusThread::ProcessCONMessage()
{
    // m_sClientSerial and m_sClientSerialEncrypted már ki vannak töltve
    // a csatlakozott kliens serial számával
    m_qsSQLCommand              = "";
    m_bArchiveProcessActive     = false;

    if( !m_bClientConnected )
    {
        // válasz a kliensnek, hogy sikeresen csatlakozott
        m_bClientConnected = true;
        msgSend( QString("INF_CLIENT_AUTHORIZED_#")+m_sClientSerial );
    }
    else
    {
        // a csatlakozás már megtörtént, hibaüzenet küldése, kapcsolat lezárása
        msgSend( QString("ERR_INVALID_CONNECTION_RECEIVED_") );
        quit();
    }
}
//====================================================================================
// BelenusThread::ProcessARCMessage
//------------------------------------------------------------------------------------
void BelenusThread::ProcessARCMessage()
{
    m_bArchiveProcessActive     = true;
    msgSend( QString("INF_READY_TO_RECEIVE_SQL_") );
}
//====================================================================================
// BelenusThread::ProcessSQLMessage
//------------------------------------------------------------------------------------
void BelenusThread::ProcessSQLMessage()
{
    m_qsSQLCommand += getMessageBody();
    msgSend( QString("INF_SQL_RECEIVED_#")+m_qsMessageChecksum );
}
//====================================================================================
// BelenusThread::ProcessPRCMessage
//------------------------------------------------------------------------------------
void BelenusThread::ProcessPRCMessage()
{
    //---------------------------------------------------------------------------------
    // Kliens archiválni akar, SQL parancs végrehajtása
    //---------------------------------------------------------------------------------
    if( m_bArchiveProcessActive )
    {
        m_bArchiveProcessActive = false;
        try
        {
            g_poDB->executeQTQuery( m_qsSQLCommand );
            msgSend( QString("INF_SQL_STATEMENT_PROCESSED_") );
        }
        catch( cSevException &e )
        {
            g_obLogger << e.severity();
            g_obLogger << e.what();
            g_obLogger << cQTLogger::EOM;
            msgSend( QString("ERR_UNABLE_TO_PROCESS_SQL_STATEMENT_") );
        }
        m_qsSQLCommand = "";
    }
}
//====================================================================================
// BelenusThread::ProcessBYEMessage
//------------------------------------------------------------------------------------
void BelenusThread::ProcessBYEMessage()
{
    m_bClientConnected = false;
    msgSend( QString("INF_CONNECTION_CLOSED_") );
    g_obLogger << cSeverity::INFO;
    g_obLogger << "Client closed the connection - Serial [" << m_sClientSerial.toStdString() << "] ";
    g_obLogger << "Id [" << m_nClientId << "] ";
    g_obLogger << cQTLogger::EOM;
    quit();
}
