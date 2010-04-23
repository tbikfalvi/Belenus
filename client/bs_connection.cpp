//====================================================================================
//
// Belenus Server Admin alkalmazas © Pagony Multimedia Studio Bt - 2010
//
//====================================================================================
//
// Filename    : bs_connection.cpp
// AppVersion  : 1.0
// FileVersion : 1.0
// Author      : Bikfalvi Tamas
//
//====================================================================================
// Interneten kereszuli kapcsolat a BelenusServer alkalmazassal
//====================================================================================

#include <QCryptographicHash>
#include <QMessageBox>
#include <QtNetwork>

//====================================================================================

#include "bs_connection.h"
#include "../framework/belenusmessages.h"
#include "belenus.h"

#define STEP_START_ACTION               0
#define STEP_CONNECT_SERVER             1
#define STEP_WAIT_AUTHORIZATION         2

//====================================================================================
// BelenusServerConnection::BelenusServerConnection
//------------------------------------------------------------------------------------
BelenusServerConnection::BelenusServerConnection( QTcpSocket *p_pTcpSocket ) : QThread()
{
    /*g_obLogger << cSeverity::DEBUG;
    g_obLogger << "BSC BelenusServerConnection::BelenusServerConnection";
    g_obLogger << cQTLogger::EOM;*/

    m_ptcpSocket                = NULL;

    blockSize                   = 0;

    m_bConnectionInProgress     = false;
    m_bCheckRegistration        = true;

    m_bSerialValid              = false;

    m_qsMessageIN               = "";
    m_qsMessageOUT              = "";
    m_qsMessageChecksumIN       = "";
    m_qsMessageChecksumOUT      = "";

    m_nProcessStep              = 0;

    m_qsAction                  = "";
    m_qsCommand                 = "";
    m_qsParameter               = "";

    m_qsClientSerial            = "";

    m_ptcpSocket = p_pTcpSocket;
}
//====================================================================================
// BelenusServerConnection::~BelenusServerConnection
//------------------------------------------------------------------------------------
BelenusServerConnection::~BelenusServerConnection()
{
    delete m_ptcpSocket;
}
//====================================================================================
// BelenusServerConnection::run
//------------------------------------------------------------------------------------
void BelenusServerConnection::run()
{
    /*g_obLogger << cSeverity::DEBUG;
    g_obLogger << "BSC BelenusServerConnection::run";
    g_obLogger << cQTLogger::EOM;*/

    connect(m_ptcpSocket, SIGNAL(readyRead()),                         this, SLOT(msgReceive()) );
    connect(m_ptcpSocket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(displayError(QAbstractSocket::SocketError)) );

    exec();
}
//====================================================================================
// BelenusServerConnection::init
//------------------------------------------------------------------------------------
void BelenusServerConnection::init( QString p_qsClientSerial,
                                    QString p_qsHostAddress,
                                    QString p_qsHostPort )
{
    g_obLogger << cSeverity::DEBUG;
    g_obLogger << "BSC BelenusServerConnection::init";
    g_obLogger << cQTLogger::EOM;

    m_qsClientSerial    = p_qsClientSerial;
    m_qsHostAddress     = p_qsHostAddress;
    m_qsHostPort        = p_qsHostPort;

    checkClientRegistration();
}
//====================================================================================
// BelenusServerConnection::isConnectionActive
//------------------------------------------------------------------------------------
bool BelenusServerConnection::isConnectionActive()
{
    return m_bConnectionInProgress;
}
//====================================================================================
// BelenusServerConnection::connectToServer
//------------------------------------------------------------------------------------
void BelenusServerConnection::connectToServer()
{
    g_obLogger << cSeverity::DEBUG;
    g_obLogger << "BSC BelenusServerConnection::connectToServer";
    g_obLogger << cQTLogger::EOM;

    m_ptcpSocket->connectToHost( m_qsHostAddress, m_qsHostPort.toInt() );
}
//====================================================================================
// BelenusServerConnection::disconnectFromServer
//------------------------------------------------------------------------------------
void BelenusServerConnection::disconnectFromServer()
{
    g_obLogger << cSeverity::DEBUG;
    g_obLogger << "BSC BelenusServerConnection::disconnectFromServer";
    g_obLogger << cQTLogger::EOM;

    m_ptcpSocket->disconnectFromHost();
    m_bConnectionInProgress = false;
}
//====================================================================================
// BelenusServerConnection::isSerialValid
//------------------------------------------------------------------------------------
bool BelenusServerConnection::isSerialValid()
{
    return m_bSerialValid;
}
//====================================================================================
// cDlgMainWindow::sendMessage
//------------------------------------------------------------------------------------
void BelenusServerConnection::sendMessage()
{
    g_obLogger << cSeverity::DEBUG;
    g_obLogger << "BSC BelenusServerConnection::sendMessage";
    g_obLogger << cQTLogger::EOM;

    m_qsMessageOUT = m_qsAction + m_qsCommand;

    if( m_qsParameter.length() > 0 )
        m_qsMessageOUT += QString("#")+m_qsParameter;

    g_obLogger << cSeverity::DEBUG;
    g_obLogger << "BSC " << m_qsMessageOUT.toStdString();
    g_obLogger << cQTLogger::EOM;

    msgSend( m_qsMessageOUT );
}
//====================================================================================
// BelenusServerConnection::msgReceive
//------------------------------------------------------------------------------------
void BelenusServerConnection::msgReceive()
{
    g_obLogger << cSeverity::DEBUG;
    g_obLogger << "BSC BelenusServerConnection::msgReceive";
    g_obLogger << cQTLogger::EOM;

    QDataStream in(m_ptcpSocket);
    in.setVersion(QDataStream::Qt_4_0);
    m_qsMessageIN = "";

    if (blockSize == 0)
    {
        if (m_ptcpSocket->bytesAvailable() < (int)sizeof(quint16))
            return;
        in >> blockSize;
    }

    if (m_ptcpSocket->bytesAvailable() < blockSize)
        return;

    in >> m_qsMessageIN;
    blockSize = 0;

    g_obLogger << cSeverity::DEBUG;
    g_obLogger << "MsgIN  [" << m_qsMessageIN.toStdString() << "] ";
    g_obLogger << cQTLogger::EOM;

    if( checkMessageConsistency() )
    {
        disconnectFromServer();
    }

    ProcessMessage();
}
//====================================================================================
// BelenusServerConnection::checkClientRegistration
//------------------------------------------------------------------------------------
void BelenusServerConnection::checkClientRegistration()
{
    connectToServer();
    sleep(1);
    m_bConnectionInProgress = true;
    m_bCheckRegistration    = true;
    m_nProcessStep          = STEP_CONNECT_SERVER;
    ProcessClientAuthorization();
}
//====================================================================================
// BelenusServerConnection::msgSend
//------------------------------------------------------------------------------------
void BelenusServerConnection::msgSend( QString qsMsg )
{
    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);

    QString strMsgOut = qsMsg.leftJustified(168,' ');

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

    m_ptcpSocket->write(block);
}
//====================================================================================
// BelenusServerConnection::displayError
//------------------------------------------------------------------------------------
void BelenusServerConnection::displayError(QAbstractSocket::SocketError socketError)
{
    switch (socketError)
    {
        case QAbstractSocket::RemoteHostClosedError:
            g_obLogger << cSeverity::ERROR;
            g_obLogger << "BSC " << INF_CONNECTION_CLOSED;
            g_obLogger << cQTLogger::EOM;
            break;
        case QAbstractSocket::HostNotFoundError:
            g_obLogger << cSeverity::ERROR;
            g_obLogger << "BSC " << ERR_HOST_NOT_FOUND;
            g_obLogger << cQTLogger::EOM;
            break;
        case QAbstractSocket::ConnectionRefusedError:
            g_obLogger << cSeverity::ERROR;
            g_obLogger << "BSC " << ERR_CONNECTION_REJECTED;
            g_obLogger << cQTLogger::EOM;
            break;
        default:
            g_obLogger << cSeverity::ERROR;
            g_obLogger << "BSC " << (tr("Error: %1.").arg(m_ptcpSocket->errorString())).toStdString();
            g_obLogger << cQTLogger::EOM;
            break;
    }
    m_ptcpSocket->abort();
}
//====================================================================================
// BelenusServerConnection::checkMessageConsistency
//------------------------------------------------------------------------------------
bool BelenusServerConnection::checkMessageConsistency()
{
    bool isMessageCorrupt = false;

    m_obPwdHash = QCryptographicHash::hash( m_qsMessageIN.left(m_qsMessageIN.length()-32).toAscii(), QCryptographicHash::Md5 );
    m_qsMessageChecksumIN = QString( m_obPwdHash.toHex() );

    if( m_qsMessageIN.length() != 200 )
    {
        isMessageCorrupt = true;
        g_obLogger << cSeverity::ERROR;
        g_obLogger << "BSC " << ERR_MESSAGE_LENGTH_INVALID;
        g_obLogger << cQTLogger::EOM;
    }
    else if( m_qsMessageChecksumIN.compare( m_qsMessageIN.right(32) ) != 0 )
    {
        isMessageCorrupt = true;
        g_obLogger << cSeverity::ERROR;
        g_obLogger << "BSC " << ERR_MESSAGE_CHEKCSUM_MISMATCH;
        g_obLogger << cQTLogger::EOM;
    }

    return isMessageCorrupt;
}
//====================================================================================
// BelenusServerConnection::getMessageBody
//------------------------------------------------------------------------------------
QString BelenusServerConnection::getMessageBody()
{
    QString qsTemp = m_qsMessageIN.mid(3,160).trimmed();

    if( qsTemp.right(8).compare("#MSGEND#") == 0 )
        qsTemp.chop(8);

    return qsTemp;
}
//====================================================================================
// BelenusServerConnection::getMessageAction
//------------------------------------------------------------------------------------
QString BelenusServerConnection::getMessageAction()
{
   return m_qsMessageIN.left(3);
}
//====================================================================================
// BelenusServerConnection::ProcessMessage
//------------------------------------------------------------------------------------
void BelenusServerConnection::ProcessMessage()
{
    if( m_bCheckRegistration ) ProcessClientAuthorization();
}
//====================================================================================
// BelenusServerConnection::ProcessClientAuthorization
//------------------------------------------------------------------------------------
void BelenusServerConnection::ProcessClientAuthorization()
{
    g_obLogger << cSeverity::DEBUG;
    g_obLogger << "BSC BelenusServerConnection::ProcessClientAuthorization";
    g_obLogger << cQTLogger::EOM;

    //---------------------------------------------------------------------------------
    // Beerkezett valaszuzenet feldolgozasa
    //---------------------------------------------------------------------------------
    switch( m_nProcessStep )
    {
        case STEP_WAIT_AUTHORIZATION:
            g_obLogger << cSeverity::DEBUG;
            g_obLogger << "BSC Step Authorization";
            g_obLogger << cQTLogger::EOM;
            if( getMessageAction().compare(ACT_SEND_INFORMATION) == 0 )
            {
                QStringList strMsgPart = getMessageBody().split("#");

                if( strMsgPart[0].compare(INF_CLIENT_AUTHORIZED) == 0 &&
                    strMsgPart[1].compare(m_qsClientSerial) == 0 )
                {
                    m_bSerialValid = true;
                }
            }
            m_bCheckRegistration    = false;
            m_nProcessStep          = STEP_START_ACTION;
            disconnectFromServer();
            break;
    }

    //---------------------------------------------------------------------------------
    // Kovetkezo lepes inditasa
    //---------------------------------------------------------------------------------
    switch( m_nProcessStep )
    {
        case STEP_CONNECT_SERVER:
            g_obLogger << cSeverity::DEBUG;
            g_obLogger << "BSC Step Connect";
            g_obLogger << cQTLogger::EOM;
            m_qsAction    = ACT_CONNECT;
            m_qsCommand   = m_qsClientSerial;
            m_qsParameter = "";
            sendMessage();
            m_nProcessStep = STEP_WAIT_AUTHORIZATION;
            break;
    }
}
//====================================================================================
