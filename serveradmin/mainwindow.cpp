//====================================================================================
//
// Belenus Server Admin alkalmazas (c) Pagony Multimedia Studio Bt - 2010
//
//====================================================================================


#include <QHostAddress>
#include "../framework/qtlogger.h"
#include "mainwindow.h"
#include "preferences.h"
#include "../framework/logger/ConsoleWriter.h"
#include "../framework/logger/LogEmitter.h"

extern cQTLogger g_obLogger;
extern AdminPreferences g_prefs;



MainWindow::MainWindow( QWidget *p_poParent )
    :   QDialog( p_poParent )
{
    setupUi( this );

    LogEmitter *emitter = new LogEmitter(static_cast<cSeverity::teSeverity>(g_prefs.value("loglevel/gui").toInt()));
    connect(emitter, SIGNAL(logMessage(QString)), this, SLOT(onLogMessage(QString)));
    g_obLogger.attachWriter( "gui", emitter );
    g_obLogger.attachWriter( "console", new ConsoleWriter(static_cast<cSeverity::teSeverity>(g_prefs.value("loglevel/console").toInt())) );


    setWindowIcon( QIcon("./resources/belenus.ico") );
    pbExit->setIcon( QIcon("./resources/40x40_shutdown.gif") );

    iHost->setText( QString("%1:%2").arg(g_prefs.value("server/host")).arg(g_prefs.value("server/port")) );
    iUsername->setText("root");
    iPassword->setText("Korben5Dallas");

    logLevel->addItem("Debug", cSeverity::DEBUG);
    logLevel->addItem("Info", cSeverity::INFO);
    logLevel->addItem("Warning", cSeverity::WARNING);
    logLevel->addItem("Error", cSeverity::ERROR);

    _connection.moveToThread(&_connection);
    _connection.start();

    connect( &_connection, SIGNAL(disconnected()),   this, SLOT(disconnected()) );
    connect( &_connection, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(socketError(QAbstractSocket::SocketError)));
    connect( &_connection, SIGNAL(connected()), this, SLOT(connected()) );
    connect( &_connection, SIGNAL(sqlResultReady(int, SqlResult*)), this, SLOT(onSqlResult(int,SqlResult*)) );
    connect( &_connection, SIGNAL(registerKeyResponse(Result::ResultCode)), this, SLOT(onRegisterKeyResponse(Result::ResultCode)) );

    enableConnectionButton();

    g_obLogger(cSeverity::DEBUG) << "[MainWindow::MainWindow] constructed" << EOM;
}



MainWindow::~MainWindow()
{
    g_obLogger.detachWriter("gui");
}



void MainWindow::onLogMessage(QString msg)
{
    listStatus->addItem( msg );
    listStatus->scrollToBottom();
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
    g_obLogger(cSeverity::DEBUG) << "[MainWindow::on_bConnect_clicked] enter" << EOM;

    if ( _connection.isConnected() ) {
        g_obLogger(cSeverity::DEBUG) << "[MainWindow::on_bConnect_clicked] connection is valid. disconnecting" << EOM;
        _connection.abort();
    } else {
        int port = g_prefs.value("server/port").toInt();
        QString host;

        g_obLogger(cSeverity::DEBUG) << "[MainWindow::on_bConnect_clicked] port from config is " << port << EOM;

        int p = iHost->text().indexOf(':');
        if ( p>=0 ) {
            port = iHost->text().mid( iHost->text().indexOf(':')+1, iHost->text().length()-p-1 ).toInt();
            host = iHost->text().mid(0, p);

            if (port<=0 || port>65535)
                port = g_prefs.value("server/port").toInt();
        } else
            host = iHost->text();

        if ( host.isEmpty() ) {
            g_obLogger(cSeverity::INFO) << "Host is empty. aborting." << EOM;
            return;
        }

        g_obLogger(cSeverity::DEBUG) << "[MainWindow::on_bConnect_clicked] host="<< host << ", port=" << port << EOM;

        bConnect->setEnabled(false);
        bConnect->setText("Connecting");

        _connection.setCredentials(iUsername->text(), iPassword->text());
        _connection.connectTo( host, port );
    }
}


void MainWindow::connected()
{
    g_obLogger(cSeverity::DEBUG) << "[MainWindow::connected()] enter" << EOM;

    bConnect->setText("Disconnect");
    bConnect->setEnabled(true);

    g_obLogger(cSeverity::INFO) << "Connected to server" << EOM;
}



void MainWindow::disconnected()
{
    g_obLogger(cSeverity::DEBUG) << "[MainWindow::disconnected] called" << EOM;
    bConnect->setText("Connect");
    bConnect->setEnabled(true);
}



void MainWindow::socketError(QAbstractSocket::SocketError /*socketError*/)
{
    g_obLogger(cSeverity::DEBUG) << "[MainWindow::socketError] called" << EOM;
    bConnect->setText("Connect");
    bConnect->setEnabled(true);
}



void MainWindow::on_bRegister_clicked()
{
    if ( !_connection.isConnected() ) {
        g_obLogger(cSeverity::ERROR) << "Not connected to server" << EOM;
        return;
    }

    if ( !iNewKey->text().length() )
        return;

    g_obLogger(cSeverity::DEBUG) << "[MainWindow::on_pushButton_clicked] registering key " << iNewKey->text() << EOM;
    _connection.registerNewKey(iNewKey->text().toStdString().c_str());
}



void MainWindow::onSqlResult(int id, SqlResult *res)
{
    g_obLogger(cSeverity::DEBUG) << "[MainWindow::on_sqlResult] result with id " << id << " received. Rows=" << res->rowCount() << EOM;

    switch (id) {

    case AdminClientThread::Q_GET_KEYS:
        g_obLogger(cSeverity::INFO) << "Received license keys result. Rows=" << res->rowCount() << EOM;
        licenseKeys->setModel(res);
        break;

    case AdminClientThread::Q_GET_LOGS:
        g_obLogger(cSeverity::INFO) << "Received log result. Rows=" << res->rowCount() << EOM;
        logs->setModel(res);
        break;

    case AdminClientThread::Q_RESET_CODE2:
        if (res->isValid())
            g_obLogger(cSeverity::INFO) << "Code2 was successfully reseted" << EOM;
        else
            g_obLogger(cSeverity::INFO) << "Failed to reset code2" << EOM;
        break;

    case AdminClientThread::Q_REMOVE_LICENSE_KEY:
        if (res->isValid()) {
            g_obLogger(cSeverity::INFO) << "License key was successfully removed" << EOM;
            _connection.queryLicenseKeys();
        } else
            g_obLogger(cSeverity::INFO) << "Failed to remove license key" << EOM;
        break;

    default:
        g_obLogger(cSeverity::ERROR) << "Unknown query result received. Id is " << id << ", result has " << res->rowCount() << " rows" << EOM;
        break;
    }
}



void MainWindow::onRegisterKeyResponse(Result::ResultCode r)
{
    if ( r!=Result::OK ) {
        g_obLogger(cSeverity::INFO) << "Registering new key was not successful. Returned code = " << r << EOM;
    } else
        _connection.queryLicenseKeys();
}



void MainWindow::on_bResetCode2_clicked()
{
    if (!licenseKeys->model())
        return;

    QModelIndexList indexes = licenseKeys->selectionModel()->selection().indexes();
    if (indexes.count()<1)
        return;

    int clientId = licenseKeys->model()->index(indexes.at(0).row(), 0).data().toInt();
    g_obLogger(cSeverity::INFO) << "Reseting code2 for client #" << clientId << EOM;
    _connection.resetCode2( clientId );
}



void MainWindow::on_pushButton_clicked()
{
    // load the keys from server
    g_obLogger(cSeverity::INFO) << "Getting licenses from server" << EOM;
    _connection.queryLicenseKeys();
}



void MainWindow::on_bGetLogs_clicked()
{
    g_obLogger(cSeverity::INFO) << "Getting logs from server" << EOM;
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
    g_obLogger(cSeverity::INFO) << "Removing license of client #" << clientId << EOM;
    _connection.removeKey( clientId );
}



void MainWindow::on_pbExit_clicked()
{
    close();
}
