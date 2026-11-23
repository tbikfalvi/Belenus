//====================================================================================
//
// Belenus WebApp alkalmazas (c) Bikfalvi Tamas
//
//====================================================================================

#include "mainwindow.h"

#include "licencemanager.h"
#include "logger.h"
#include "ui_mainwindow.h"

#include <QCoreApplication>
#include <QPixmap>
#include <QTimer>

// A meglévő Designer-ablakot építi fel, majd beköti a műveleteket és visszajelzéseket.
MainWindow::MainWindow(LicenceManager &licences, const Logger &logger)
    : ui(std::make_unique<Ui::WebAppWindow>())
{
    ui->setupUi(this);
    setWindowTitle(QStringLiteral("Belenus WebApp - v%1").arg(QCoreApplication::applicationVersion()));
    ui->logStatus->setVisible(!logger.error().isEmpty());
    if (!logger.error().isEmpty())
        ui->logStatus->setText(tr("Cannot open or write to webapp.log: %1").arg(logger.error()));

    const auto refresh = [this, &licences] {
        resetLocalLicence();
        licences.refreshLocalLicence();
    };
    connect(ui->exitButton, &QPushButton::clicked, this, &QDialog::close);
    connect(ui->databaseRefresh, &QPushButton::clicked, this, refresh);
    connect(ui->licenceRefresh, &QPushButton::clicked, this, refresh);
    connect(ui->serverRefresh, &QPushButton::clicked, &licences, &LicenceManager::checkServer);
    connect(ui->onlineButton, &QPushButton::clicked, this, [this, &licences] {
        resetLocalLicence();
        ui->onlineStatus->clear();
        licences.checkOnline();
    });

    connect(&licences, &LicenceManager::databaseStatusChanged, this, &MainWindow::showDatabaseResult);
    connect(&licences, &LicenceManager::localLicenceRead, this, &MainWindow::showLocalLicence);
    connect(&licences, &LicenceManager::localLicenceReadFailed, ui->licenceStatus, &QLabel::setText);
    connect(&licences, &LicenceManager::serverAddressChanged, ui->serverAddress, &QLineEdit::setText);
    connect(&licences, &LicenceManager::serverCheckNotStarted, this, [this](const QString &message) {
        showConnection(ui->serverLight, ui->serverStatus, "yellow", tr("The server check did not start."));
        ui->serverStatus->setToolTip(message);
    });
    connect(&licences, &LicenceManager::serverTokenResult, this, &MainWindow::showServerResult);
    connect(&licences, &LicenceManager::networkBusyChanged, this, &MainWindow::setNetworkBusy);
    connect(&licences, &LicenceManager::onlineStarted, this, [this] {
        ui->onlineStatus->setText(tr("Checking licence online…"));
    });
    connect(&licences, &LicenceManager::onlineFinished, ui->onlineStatus, &QLabel::setText);
    QTimer::singleShot(0, this, refresh);
}

MainWindow::~MainWindow() = default;

// Kicseréli a jelző ikonját és feliratát, egyben törli a korábbi hibaleírást.
void MainWindow::showConnection(QLabel *light, QLabel *status, const char *icon, const QString &message)
{
    light->setPixmap(QPixmap(QStringLiteral(":/icons/status_%1.png").arg(QString::fromLatin1(icon))));
    status->setText(message);
    status->setToolTip({});
}

void MainWindow::showDatabaseResult(DatabaseManager::Status status, const QString &details)
{
    switch (status) {
    case DatabaseManager::Status::Connected:
        showConnection(ui->databaseLight, ui->databaseStatus, "green", tr("Connection succeeded."));
        break;
    case DatabaseManager::Status::DriverUnavailable:
        showConnection(ui->databaseLight, ui->databaseStatus, "red", tr("The QMYSQL driver could not be loaded."));
        break;
    case DatabaseManager::Status::OpenFailed:
        showConnection(ui->databaseLight, ui->databaseStatus, "red", tr("Connection failed."));
        ui->databaseStatus->setToolTip(details);
        break;
    }
}

// A szerverjelző a tokenkérés technikai eredményét mutatja, a licenckód ettől külön jelenik meg.
void MainWindow::showServerResult(bool ok)
{
    showConnection(ui->serverLight, ui->serverStatus, ok ? "green" : "red",
                   ok ? tr("HTTPS connection established.") : tr("Token request failed."));
}

void MainWindow::setNetworkBusy(bool busy)
{
    ui->serverRefresh->setEnabled(!busy);
    ui->onlineButton->setEnabled(!busy);
    if (busy)
        showConnection(ui->serverLight, ui->serverStatus, "yellow", tr("Connecting…"));
}

void MainWindow::resetLocalLicence()
{
    ui->localSerial->clear();
    ui->localDate->clear();
    ui->localServerCode->clear();
    ui->localClientCode->clear();
    ui->licenceStatus->setText(tr("Not loaded yet."));
}

// A betöltött rekord mezőit jeleníti meg, és külön jelzi az üres kulcsot.
void MainWindow::showLocalLicence(const Licence &licence)
{
    ui->localSerial->setText(licence.serial);
    ui->localDate->setText(licence.checkedAt);
    ui->localServerCode->setText(licence.serverCode);
    ui->localClientCode->setText(licence.clientCode);
    ui->licenceStatus->setText(licence.serial.trimmed().isEmpty()
                                   ? tr("The local licence key is empty.") : localLicenceStatus(licence.status));
}

// A tárolt azonosítókat érintetlenül hagyja, csak az ismert állapotok feliratát fordítja.
QString MainWindow::localLicenceStatus(const QString &status)
{
    if (status == QStringLiteral("VALIDATED"))
        return tr("Validated");
    if (status == QStringLiteral("UNVALIDATED"))
        return tr("Not validated");
    if (status == QStringLiteral("UNREGISTERED"))
        return tr("Not registered");
    if (status == QStringLiteral("INVALID"))
        return tr("Expired / Invalid");
    if (status == QStringLiteral("DEMO"))
        return tr("Demo");
    return status.isEmpty() ? tr("Unknown") : status;
}
