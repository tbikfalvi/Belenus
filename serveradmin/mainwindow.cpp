//====================================================================================
//
// Belenus Server Admin alkalmazas © Pagony Multimedia Studio Bt - 2010
//
//====================================================================================


#include <QMessageBox>
#include "../framework/qtlogger.h"
#include "mainwindow.h"
#include "preferences.h"


extern cQTLogger g_obLogger;
extern AdminPreferences g_prefs;



MainWindow::MainWindow( QWidget *p_poParent )
    :   QDialog( p_poParent )
{
    setupUi( this );

    iHost->setText( QString("%1:%2").arg(g_prefs.value("server/host")).arg(g_prefs.value("server/port")) );
    iUsername->setText("root");
    iPassword->setText("Korben5Dallas");

    logLevel->addItem("Debug", cSeverity::DEBUG);
    logLevel->addItem("Info", cSeverity::INFO);
    logLevel->addItem("Warning", cSeverity::WARNING);
    logLevel->addItem("Error", cSeverity::ERROR);

    enableConnectionButton();

    connect( &_connection, SIGNAL(disconnected()),   this, SLOT(disconnected()) );
    connect( &_connection, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(socketError(QAbstractSocket::SocketError)));
    connect( &_connection, SIGNAL(connected()), this, SLOT(connected()) );
    connect( &_connection, SIGNAL(sqlResultReady(int, SqlResult*)), this, SLOT(on_sqlResult(int,SqlResult*)) );

    g_obLogger(cSeverity::DEBUG) << "[MainWindow::MainWindow] constructed" << cQTLogger::EOM;
}



MainWindow::~MainWindow()
{
}



void MainWindow::log(QString msg)
{
    listStatus->addItem( tr("[%1] %2").arg(QTime::currentTime().toString(Qt::TextDate)).arg(msg) );
}



void MainWindow::enableConnectionButton()
{
    bConnect->setEnabled( !iHost->text().isEmpty()
                          && !iUsername->text().isEmpty()
                          && !iPassword->text().isEmpty() );
}



void MainWindow::on_iHost_textChanged(QString )
{
    enableConnectionButton();
}



void MainWindow::on_iUsername_textChanged(QString )
{
    enableConnectionButton();
}



void MainWindow::on_iPassword_textChanged(QString )
{
    enableConnectionButton();
}



void MainWindow::on_bConnect_clicked()
{
    g_obLogger(cSeverity::DEBUG) << "[MainWindow::on_bConnect_clicked] enter" << cQTLogger::EOM;

    if ( _connection.isConnected() ) {
        g_obLogger(cSeverity::DEBUG) << "[MainWindow::on_bConnect_clicked] connection is valid. disconnecting" << cQTLogger::EOM;
        _connection.abortConnection();
    } else {
        int port = g_prefs.value("server/port").toInt();
        QString host;

        g_obLogger(cSeverity::DEBUG) << "[MainWindow::on_bConnect_clicked] host is " << host.toStdString() << ", port from config is " << port << cQTLogger::EOM;

        int p = iHost->text().indexOf(':');
        if ( p>=0 ) {
            port = iHost->text().mid( iHost->text().indexOf(':')+1, iHost->text().length()-p-1 ).toInt();
            host = iHost->text().mid(0, p);

            if (port<=0 || port>65535)
                port = g_prefs.value("server/port").toInt();
        } else
            host = iHost->text();

        if ( host.isEmpty() ) {
            g_obLogger(cSeverity::INFO) << "[MainWindow::on_bConnect_clicked] host is empty. aborting" << cQTLogger::EOM;
            return;
        }

        g_obLogger(cSeverity::DEBUG) << "[MainWindow::on_bConnect_clicked] host="<< host.toStdString() << ", port=" << port << cQTLogger::EOM;

        bConnect->setEnabled(false);
        bConnect->setText("Connecting");

        _connection.setCredentials(iUsername->text(), iPassword->text());
        _connection.connectTo( QHostAddress(host), port );
    }
}


void MainWindow::connected()
{
    g_obLogger(cSeverity::DEBUG) << "[MainWindow::connected()] enter" << cQTLogger::EOM;

    bConnect->setText("Disconnect");
    bConnect->setEnabled(true);

    log("Connected to server");
}



void MainWindow::disconnected()
{
    g_obLogger << cSeverity::DEBUG << "[MainWindow::disconnected] called" << cQTLogger::EOM;
    bConnect->setText("Connect");
    bConnect->setEnabled(true);
    log("Disconnected. Invalid credentials?");
}



void MainWindow::socketError(QAbstractSocket::SocketError socketError)
{
    QString err;
    switch (socketError) {
        case 0: err = "Connection refused"; break;
        case 1: err = "Remote host closed the connection"; break;
        case 2: err = "Host not found"; break;
        case 3: err = "Socket access error"; break;
        case 4: err = "Socket resource error"; break;
        case 5: err = "Socket timeout"; break;
        case 7: err = "Network error"; break;
        default:err = "Unkown error"; break;
    }

    g_obLogger << cSeverity::DEBUG << "[MainWindow::socketError] " << socketError << ": " << err.toStdString() << cQTLogger::EOM;
    bConnect->setText("Connect");
    bConnect->setEnabled(true);
    log( tr("Socket error: %1").arg(err) );
}



void MainWindow::on_bRegister_clicked()
{
    if ( !_connection.isConnected() ) {
        g_obLogger(cSeverity::ERROR) << "Not connected to server" << cQTLogger::EOM;
        return;
    }

    if ( !iNewKey->text().length() )
        return;

    g_obLogger(cSeverity::DEBUG) << "[MainWindow::on_pushButton_clicked] registering key " << iNewKey->text().toStdString() << cQTLogger::EOM;
    _connection.registerNewKey(iNewKey->text().toStdString().c_str());
}



void MainWindow::on_sqlResult(int id, SqlResult *res)
{
    g_obLogger(cSeverity::DEBUG) << "[MainWindow::on_sqlResult] result with id " << id << " received. Rows=" << res->rowCount() << cQTLogger::EOM;

    switch (id) {

    case AdminClientThread::Q_GET_KEYS:
        log(tr("Received license keys result. Rows=%1").arg(res->rowCount()));
        licenseKeys->setModel(res);
        break;

    case AdminClientThread::Q_GET_LOGS:
        log(tr("Received logs result. Rows=%1").arg(res->rowCount()));
        logs->setModel(res);
        break;

    case AdminClientThread::Q_RESET_CODE2:
        if (res->isValid())
            log("Code2 was successfully reseted");
        else
            log("Failed to reset code2");
        break;

    case AdminClientThread::Q_REMOVE_LICENSE_KEY:
        if (res->isValid())
            log("License key was successfully removed");
        else
            log("Failed to remove license key");
        break;

    default:
        log(tr("Unknown query result received. Id is %1, result has %1 rows").arg(id).arg(res->rowCount()));
        break;
    }
}



void MainWindow::on_bResetCode2_clicked()
{
    if (!licenseKeys->model())
        return;

    QModelIndexList indexes = licenseKeys->selectionModel()->selection().indexes();
    if (indexes.count()<1)
        return;

    int clientId = licenseKeys->model()->index(indexes.at(0).row(), 0).data().toInt();
    g_obLogger(cSeverity::INFO) << "[MainWindow::on_bResetCode2_clicked] Reseting code2 for client " << clientId << cQTLogger::EOM;
    log("Resetting code 2");
    _connection.resetCode2( clientId );
}



void MainWindow::on_pushButton_clicked()
{
    // load the keys from server
    g_obLogger(cSeverity::DEBUG) << "[MainWindow::on_pushButton_clicked()] getting licenses from server" << cQTLogger::EOM;
    log("Getting license keys");
    _connection.queryLicenseKeys();
}



void MainWindow::on_bGetLogs_clicked()
{
    g_obLogger(cSeverity::DEBUG) << "[MainWindow::on_bGetLogs_clicked()] getting logs from server" << cQTLogger::EOM;
    log("Getting logs");
    _connection.queryLogs( static_cast<cSeverity::teSeverity>(logLevel->itemData(logLevel->currentIndex()).toInt()), logCount->value());
}



void MainWindow::on_bRemoveKey_clicked()
{
    if (!licenseKeys->model())
        return;

    QModelIndexList indexes = licenseKeys->selectionModel()->selection().indexes();
    if (indexes.count()<1)
        return;

    int clientId = licenseKeys->model()->index(indexes.at(0).row(), 0).data().toInt();
    g_obLogger(cSeverity::INFO) << "[MainWindow::on_bRemoveKey_clicked] Removing license on " << clientId << cQTLogger::EOM;
    log("Removing key");
    _connection.removeKey( clientId );
}
