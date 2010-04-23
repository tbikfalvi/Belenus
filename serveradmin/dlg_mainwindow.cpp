//====================================================================================
//
// Belenus Server Admin alkalmazas © Pagony Multimedia Studio Bt - 2010
//
//====================================================================================
//
// Filename    : dlg_mainwindow.cpp
// AppVersion  : 1.0
// FileVersion : 1.0
// Author      : Bikfalvi Tamas
//
//====================================================================================
// Fo dialogus ablak
//====================================================================================

#include <QCryptographicHash>
#include <QMessageBox>
#include <QtNetwork>

//====================================================================================

#include "belenusserveradmin.h"
#include "../framework/belenusmessages.h"
#include "dlg_mainwindow.h"

#define STEP_START_ACTION               0
#define STEP_ESTABLISH_CONNECTION       1
#define STEP_AUTHORIZE_ADMIN            2
#define STEP_REGISTER_SERIAL            3

//====================================================================================
// cDlgMainWindow::cDlgMainWindow
//------------------------------------------------------------------------------------
cDlgMainWindow::cDlgMainWindow( QWidget *p_poParent ) : QDialog( p_poParent )
{
    setupUi( this );

    blockSize                   = 0;

    m_qsMessageIN               = "";
    m_qsMessageOUT              = "";
    m_qsMessageChecksumIN       = "";
    m_qsMessageChecksumOUT      = "";
    m_nProcessStep              = 0;

    m_bRegisterSerialOnServer   = false;

    pbEditSerial->setEnabled( false );
    pbDeleteSerial->setEnabled( false );
    pbRegisterSerial->setEnabled(false);
    pbConnect->setEnabled(false);
    pbRegisterSerialOnServer->setEnabled(false);
    pbDeleteSerialFromServer->setEnabled(false);
    pbSendCustom->setEnabled(false);

    ledAddress->setText( g_poPrefs->getServerAddress() );
    ledPort->setText( QString("%1").arg(g_poPrefs->getServerPort()) );

    FillListSerial();
    FillMessageCombo();

    m_ptcpSocket = new QTcpSocket(this);

    connect(m_ptcpSocket, SIGNAL(readyRead()),                         this, SLOT(msgReceive()) );
    connect(m_ptcpSocket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(displayError(QAbstractSocket::SocketError)) );
}
//====================================================================================
// cDlgMainWindow::~cDlgMainWindow
//------------------------------------------------------------------------------------
cDlgMainWindow::~cDlgMainWindow()
{
}
//====================================================================================
// cDlgMainWindow::FillListSerial
//------------------------------------------------------------------------------------
void cDlgMainWindow::FillListSerial()
{
    listSerialNumbers->clear();
    QSqlQuery *poQuery = g_poDB->executeQTQuery( QString( "SELECT Serial FROM licenses" ) );
    while( poQuery->next() )
    {
        listSerialNumbers->addItem( poQuery->value(0).toString() );
    }
}
//====================================================================================
// cDlgMainWindow::FillMessageCombo
//------------------------------------------------------------------------------------
void cDlgMainWindow::FillMessageCombo()
{
    cmbCustomMsgBody->addItem("<type text here or select from list>" );
    cmbCustomMsgBody->addItem("_ADMIN_REQUEST_LOGIN_" );
    cmbCustomMsgBody->addItem("_ADMIN_PASSWORD_" );
    cmbCustomMsgBody->addItem("_REGISTER_CLIENT_SERIAL_" );
    cmbCustomMsgBody->addItem("_DELETE_CLIENT_SERIAL_" );
    cmbCustomMsgBody->addItem("_CONNECTION_CLOSED_" );
    cmbCustomMsgBody->addItem("_ADMIN_AUTHORIZED_" );
    cmbCustomMsgBody->addItem("_CLIENT_SERIAL_REGISTERED_" );
    cmbCustomMsgBody->addItem("_CLIENT_SERIAL_DELETED_" );
    cmbCustomMsgBody->addItem("_CLIENT_AUTHORIZED_" );
    cmbCustomMsgBody->addItem("_READY_TO_RECEIVE_SQL_" );
    cmbCustomMsgBody->addItem("_SQL_RECEIVED_" );
    cmbCustomMsgBody->addItem("_SQL_STATEMENT_PROCESSED_" );
    cmbCustomMsgBody->addItem("_CONNECTION_REJECTED_" );
    cmbCustomMsgBody->addItem("_INVALID_CONNECTION_RECEIVED_" );
    cmbCustomMsgBody->addItem("_UNREGISTERED_ADMIN_" );
    cmbCustomMsgBody->addItem("_UNREGISTERED_CLIENT_" );
    cmbCustomMsgBody->addItem("_MESSAGE_LENGTH_INVALID_" );
    cmbCustomMsgBody->addItem("_MESSAGE_CHEKCSUM_MISMATCH_" );
    cmbCustomMsgBody->addItem("_CLIENT_SERIAL_ALREADY_REGISTERED_" );
    cmbCustomMsgBody->addItem("_UNABLE_TO_REGISTER_CLIENT_SERIAL_" );
    cmbCustomMsgBody->addItem("_UNABLE_TO_DELETE_CLIENT_SERIAL_" );
    cmbCustomMsgBody->addItem("_UNABLE_TO_PROCESS_SQL_STATEMENT_" );
}
//====================================================================================
// cDlgMainWindow::on_pbExitApplication_clicked
//------------------------------------------------------------------------------------
void cDlgMainWindow::on_pbExitApplication_clicked()
{
    exit(0);
}
//====================================================================================
// cDlgMainWindow::on_pbRegisterSerial_clicked
//------------------------------------------------------------------------------------
void cDlgMainWindow::on_pbRegisterSerial_clicked()
{
    QString m_qsSQLCommand;

    if( pbRegisterSerial->text().compare(tr("Register")) == 0 )
    {
        m_qsSQLCommand = QString( "INSERT INTO licenses (idLicenses, Serial, Country, Region, City, Zip, Address, Studio, Contact, Active) VALUES (NULL, '%1', '', '', '', '', '', '', '', '0');" ).arg( ledSerialNumber->text() );
    }
    else if( pbRegisterSerial->text().compare(tr("Update")) == 0 )
    {
        m_qsSQLCommand = QString( "UPDATE licenses SET Serial='%1' WHERE idLicenses=%2;" ).arg( ledSerialNumber->text() ).arg( m_nCurrentId );
    }

    if( ledSerialNumber->text().length() == 0 )
    {
        QMessageBox::critical( this, tr("Attention"), tr("Serial number is not defined.") );
        return;
    }
    else if( ledSerialNumber->text().length() < 10 )
    {
        QMessageBox::critical( this, tr("Attention"), tr("Serial number is too short.\nPlease set Serial number not less than 10 digits.") );
        return;
    }

    try
    {
        g_poDB->executeQTQuery( m_qsSQLCommand );
    }
    catch( cSevException &e )
    {
        g_obLogger << e.severity();
        g_obLogger << e.what();
        g_obLogger << cQTLogger::EOM;
    }
    pbRegisterSerial->setText( tr("Register") );
    ledSerialNumber->setText( "" );
    FillListSerial();
}
//====================================================================================
// cDlgMainWindow::on_ledAdminPassword_lostFocus
//------------------------------------------------------------------------------------
void cDlgMainWindow::on_ledAdminPassword_lostFocus()
{
    m_obPwdHash = QCryptographicHash::hash( ledAdminPassword->text().toAscii(), QCryptographicHash::Sha1 );
}
//====================================================================================
// cDlgMainWindow::on_pbEditSerial_clicked
//------------------------------------------------------------------------------------
void cDlgMainWindow::on_pbEditSerial_clicked()
{
    ledSerialNumber->setText( listSerialNumbers->currentItem()->text() );
    pbRegisterSerial->setText( tr("Update") );
}
//====================================================================================
// cDlgMainWindow::on_pbDeleteSerial_clicked
//------------------------------------------------------------------------------------
void cDlgMainWindow::on_pbDeleteSerial_clicked()
{
    try
    {
        g_poDB->executeQTQuery( QString( "DELETE FROM licenses WHERE serial='%1'" ).arg( listSerialNumbers->currentItem()->text() ) );
    }
    catch( cSevException &e )
    {
        g_obLogger << e.severity();
        g_obLogger << e.what();
        g_obLogger << cQTLogger::EOM;
    }
    FillListSerial();
    pbEditSerial->setEnabled( false );
    pbDeleteSerial->setEnabled( false );
    pbRegisterSerial->setText( tr("Register") );
    ledSerialNumber->setText( "" );
}
//====================================================================================
// cDlgMainWindow::on_listSerialNumbers_itemSelectionChanged
//------------------------------------------------------------------------------------
void cDlgMainWindow::on_listSerialNumbers_itemSelectionChanged()
{
    if( listSerialNumbers->currentItem()->text().length() == 0 )
    {
        pbEditSerial->setEnabled( false );
        pbDeleteSerial->setEnabled( false );
    }
    else
    {
        QSqlQuery *poQuery = g_poDB->executeQTQuery( QString( "SELECT licenseId, Country, Region, City, Zip, Address, Studio, Contact, Active FROM licenses WHERE Serial='%1'" ).arg( listSerialNumbers->currentItem()->text() ) );
        if( poQuery->size() == 1 )
        {
            poQuery->first();
            m_nCurrentId = poQuery->value( 0 ).toInt();
            QString qsCountry = poQuery->value(1).toString();
            QString qsRegion  = poQuery->value(2).toString();
            QString qsCity    = poQuery->value(3).toString();
            QString qsZip     = poQuery->value(4).toString();
            QString qsAddress = poQuery->value(5).toString();
            QString qsStudio  = poQuery->value(6).toString();
            QString qsContact = poQuery->value(7).toString();
            QString qsActive  = (poQuery->value(8).toBool()?tr("active"):tr("inactive"));
            if( poQuery->value(8).toBool() )
                lblSerialInfo->setText( QString( tr("%1\n%2, %3 - %4-%5\n%6\nContact: %7\nSerial currently %8") ).arg(qsStudio).arg(qsCountry).arg(qsRegion).arg(qsZip).arg(qsCity).arg(qsAddress).arg(qsContact).arg(qsActive) );
            else
                lblSerialInfo->setText( QString( tr("Serial currently %1") ).arg(qsActive) );
        }
        pbEditSerial->setEnabled( true );
        pbDeleteSerial->setEnabled( true );
    }
}
//====================================================================================
// cDlgMainWindow::on_listSerialNumbers_doubleClicked
//------------------------------------------------------------------------------------
void cDlgMainWindow::on_listSerialNumbers_doubleClicked(QModelIndex index)
{
    on_pbEditSerial_clicked();
}
//====================================================================================
// cDlgMainWindow::on_ledSerialNumber_textChanged
//------------------------------------------------------------------------------------
void cDlgMainWindow::on_ledSerialNumber_textChanged(QString )
{
    if( ledSerialNumber->text().length() > 0 )
        pbRegisterSerial->setEnabled(true);
    else
        pbRegisterSerial->setEnabled(false);
    EnableButtonsServer();
}
//====================================================================================
// cDlgMainWindow::on_pbEmptySerial_clicked
//------------------------------------------------------------------------------------
void cDlgMainWindow::on_pbEmptySerial_clicked()
{
    ledSerialNumber->setText( "" );
    pbRegisterSerial->setText( tr("Register") );
}
//====================================================================================
// cDlgMainWindow::on_ledAddress_textChanged
//------------------------------------------------------------------------------------
void cDlgMainWindow::on_ledAddress_textChanged(QString )
{
    pbConnect->setEnabled( EnableButtonConnect() );
}
//====================================================================================
// cDlgMainWindow::on_ledPort_textChanged
//------------------------------------------------------------------------------------
void cDlgMainWindow::on_ledPort_textChanged(QString )
{
    pbConnect->setEnabled( EnableButtonConnect() );
}
//====================================================================================
// cDlgMainWindow::EnableButtonConnect
//------------------------------------------------------------------------------------
bool cDlgMainWindow::EnableButtonConnect()
{
    if( ledAddress->text().length() > 4 && ledPort->text().length() > 0 )
        return true;
    else
        return false;
}
//====================================================================================
// cDlgMainWindow::on_pbConnect_clicked
//------------------------------------------------------------------------------------
void cDlgMainWindow::on_pbConnect_clicked()
{
    if( pbConnect->text().compare(tr("Connect")) == 0 )
    {
        pbConnect->setText(tr("Disconnect"));
        m_ptcpSocket->connectToHost( ledAddress->text(),
                                     ledPort->text().toInt());
        lblConnectionStatus->setText(tr("Connected"));
        EnableButtonsServer();
        pbSendCustom->setEnabled(true);
        listStatus->insertItem(0,tr("Connection opened for server."));
    }
    else if( pbConnect->text().compare(tr("Disconnect")) == 0 )
    {
        pbConnect->setText(tr("Connect"));
        m_ptcpSocket->abort();
        lblConnectionStatus->setText(tr("Not connected"));
        EnableButtonsServer();
        pbSendCustom->setEnabled(false);
        listStatus->insertItem(0,tr("Connection closed with server."));
        listStatus->insertItem(0,"");
    }
}
//====================================================================================
// cDlgMainWindow::msgReceive
//------------------------------------------------------------------------------------
void cDlgMainWindow::msgReceive()
{
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

    listStatus->insertItem(0,QString("IN<<")+m_qsMessageIN.mid(0,160).trimmed());

    if( checkMessageConsistency() )
    {
        on_pbConnect_clicked();
    }
    ProcessMessage();
}
//====================================================================================
// cDlgMainWindow::msgSend
//------------------------------------------------------------------------------------
void cDlgMainWindow::msgSend( QString qsMsg )
{
    listStatus->insertItem(0,QString("OUT>>")+qsMsg);

    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);

    QString strMsgOut = qsMsg.leftJustified(168,' ');

    QByteArray obPwdHash = QCryptographicHash::hash( strMsgOut.toAscii(), QCryptographicHash::Md5 );
    strMsgOut += QString( obPwdHash.toHex() );

    g_obLogger << cSeverity::DEBUG;
    g_obLogger << "Message sent";
    g_obLogger << cQTLogger::EOM;
    g_obLogger << cSeverity::DEBUG;
    g_obLogger << strMsgOut.toStdString();
    g_obLogger << cQTLogger::EOM;

    out.setVersion(QDataStream::Qt_4_0);
    out << (quint16)0;
    out << strMsgOut;
    out.device()->seek(0);
    out << (quint16)(block.size() - sizeof(quint16));

    m_ptcpSocket->write(block);
}
//====================================================================================
// cDlgMainWindow::sendMessage
//------------------------------------------------------------------------------------
void cDlgMainWindow::sendMessage()
{
    m_qsMessageOUT = m_qsAction + m_qsCommand;

    if( m_qsParameter.length() > 0 )
        m_qsMessageOUT += QString("#")+m_qsParameter;

    msgSend( m_qsMessageOUT );
}
//====================================================================================
// cDlgMainWindow::displayError
//------------------------------------------------------------------------------------
void cDlgMainWindow::displayError(QAbstractSocket::SocketError socketError)
{
    switch (socketError)
    {
        case QAbstractSocket::RemoteHostClosedError:
            listStatus->insertItem(0,tr("The host closed the connection."));
            break;
        case QAbstractSocket::HostNotFoundError:
            listStatus->insertItem(0,tr("Host was not found. Check address/port settings.") );
            break;
        case QAbstractSocket::ConnectionRefusedError:
            listStatus->insertItem(0,tr("Connection refused. Check address/port settings.") );
            break;
        default:
            listStatus->insertItem(0,tr("Error: %1.").arg(m_ptcpSocket->errorString()) );
            break;
    }
    on_pbConnect_clicked();
}
//====================================================================================
// cDlgMainWindow::on_pbRegisterSerialOnServer_clicked
//------------------------------------------------------------------------------------
void cDlgMainWindow::on_pbRegisterSerialOnServer_clicked()
{
    m_qsAction    = ACT_CONNECT;
    m_qsCommand   = REQ_ADMIN_REQUEST_LOGIN;
    m_qsParameter = "";
    sendMessage();
    m_bRegisterSerialOnServer = true;
    m_nProcessStep = STEP_ESTABLISH_CONNECTION;
}
//====================================================================================
// cDlgMainWindow::on_pbDeleteSerialFromServer_clicked
//------------------------------------------------------------------------------------
void cDlgMainWindow::on_pbDeleteSerialFromServer_clicked()
{

}
//====================================================================================
// cDlgMainWindow::EnableButtonsServer
//------------------------------------------------------------------------------------
void cDlgMainWindow::EnableButtonsServer()
{
    if( ledAdminPassword->text().length() > 0 &&
        ledSerialNumber->text().length() > 0 &&
        pbConnect->text().compare(tr("Disconnect")) == 0 )
    {
        pbRegisterSerialOnServer->setEnabled(true);
        pbDeleteSerialFromServer->setEnabled(true);
    }
    else
    {
        pbRegisterSerialOnServer->setEnabled(false);
        pbDeleteSerialFromServer->setEnabled(false);
    }
}
//====================================================================================
// cDlgMainWindow::on_ledAdminPassword_textChanged
//------------------------------------------------------------------------------------
void cDlgMainWindow::on_ledAdminPassword_textChanged(QString )
{
    EnableButtonsServer();
}
//====================================================================================
// cDlgMainWindow::checkMessageConsistency
//------------------------------------------------------------------------------------
bool cDlgMainWindow::checkMessageConsistency()
{
    bool isMessageCorrupt = false;

    m_obPwdHash = QCryptographicHash::hash( m_qsMessageIN.left(m_qsMessageIN.length()-32).toAscii(), QCryptographicHash::Md5 );
    m_qsMessageChecksumIN = QString( m_obPwdHash.toHex() );

    if( m_qsMessageIN.length() != 200 )
    {
        isMessageCorrupt = true;
        listStatus->insertItem(0,tr("Message with improper lenght received.") );
    }
    else if( m_qsMessageChecksumIN.compare( m_qsMessageIN.right(32) ) != 0 )
    {
        isMessageCorrupt = true;
        listStatus->insertItem(0,tr("Message with corrupt checksum received.") );
    }

    return isMessageCorrupt;
}
//====================================================================================
// cDlgMainWindow::getMessageBody
//------------------------------------------------------------------------------------
QString cDlgMainWindow::getMessageBody()
{
    QString qsTemp = m_qsMessageIN.mid(3,160).trimmed();

    if( qsTemp.right(8).compare("#MSGEND#") == 0 )
        qsTemp.chop(8);

    return qsTemp;
}
//====================================================================================
// cDlgMainWindow::getMessageAction
//------------------------------------------------------------------------------------
QString cDlgMainWindow::getMessageAction()
{
   return m_qsMessageIN.left(3);
}
//====================================================================================
// cDlgMainWindow::ProcessMessage
//------------------------------------------------------------------------------------
void cDlgMainWindow::ProcessMessage()
{
    if( m_bRegisterSerialOnServer ) RegisterSerialOnServer();
}
//====================================================================================
// cDlgMainWindow::RegisterSerialOnServer
//------------------------------------------------------------------------------------
void cDlgMainWindow::RegisterSerialOnServer()
{
    listStatus->insertItem(0,tr("Step: %1").arg(m_nProcessStep));
    //---------------------------------------------------------------------------------
    // Beerkezett valaszuzenet feldolgozasa
    //---------------------------------------------------------------------------------
    switch( m_nProcessStep )
    {
        case STEP_ESTABLISH_CONNECTION:
            if( getMessageAction().compare(ACT_REQUEST_DATA) == 0 &&
                getMessageBody().compare(REQ_ADMIN_PASSWORD) == 0 )
            {
                m_nProcessStep = STEP_AUTHORIZE_ADMIN;
            }
            break;
        case STEP_AUTHORIZE_ADMIN:
            if( getMessageAction().compare(ACT_SEND_INFORMATION) == 0 &&
                getMessageBody().compare(INF_ADMIN_AUTHORIZED) == 0 )
            {
                m_nProcessStep = STEP_REGISTER_SERIAL;
            }
            break;
        case STEP_REGISTER_SERIAL:
            if( getMessageAction().compare(ACT_SEND_INFORMATION) == 0 )
            {
                QStringList strMsgPart = getMessageBody().split("#");

                if( strMsgPart[0].compare(INF_CLIENT_SERIAL_REGISTERED) == 0 &&
                    strMsgPart[1].compare(ledSerialNumber->text()) == 0 )
                {
                    listStatus->insertItem(0,tr("Serial number successfully registered on server."));
                    m_bRegisterSerialOnServer = false;
                    m_nProcessStep = STEP_START_ACTION;
                }
            }
            break;
    }

    //---------------------------------------------------------------------------------
    // Kovetkezo lepes inditasa
    //---------------------------------------------------------------------------------
    switch( m_nProcessStep )
    {
        case STEP_AUTHORIZE_ADMIN:
            m_obPwdHash = QCryptographicHash::hash( ledAdminPassword->text().toAscii(), QCryptographicHash::Sha1 );
            m_qsAction    = ACT_SEND_PASSWORD;
            m_qsCommand   = QString(m_obPwdHash.toHex());
            m_qsParameter = "";
            sendMessage();
            break;
        case STEP_REGISTER_SERIAL:
            m_qsAction    = ACT_REQUEST_DATA;
            m_qsCommand   = REQ_REGISTER_CLIENT_SERIAL;
            m_qsParameter = ledSerialNumber->text();
            sendMessage();
            break;
    }
}
//====================================================================================
// cDlgMainWindow::on_pbSendCustom_clicked
//------------------------------------------------------------------------------------
void cDlgMainWindow::on_pbSendCustom_clicked()
{
    if( rbCON->isChecked() )
        m_qsAction = ACT_CONNECT;
    if( rbREQ->isChecked() )
        m_qsAction = ACT_REQUEST_DATA;
    if( rbINF->isChecked() )
        m_qsAction = ACT_SEND_INFORMATION;
    if( rbERR->isChecked() )
        m_qsAction = ACT_SEND_ERROR;
    if( rbPSW->isChecked() )
        m_qsAction = ACT_SEND_PASSWORD;
    if( rbARC->isChecked() )
        m_qsAction = ACT_START_ARCHIVE_PROCESS;
    if( rbSQL->isChecked() )
        m_qsAction = ACT_SEND_SQL_STATEMENT;
    if( rbPRC->isChecked() )
        m_qsAction = ACT_PROCESS_SQL_STATEMENT;
    if( rbBYE->isChecked() )
        m_qsAction = ACT_DISCONNECT;

    m_qsCommand = cmbCustomMsgBody->currentText();
    sendMessage();
}
