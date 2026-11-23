//====================================================================================
//
// Belenus WebApp alkalmazas (c) Bikfalvi Tamas
//
//====================================================================================

#include "mainwindow.h"

#include "applicationsettings.h"
#include "connectionmonitor.h"
#include "languagemanager.h"
#include "licencemanager.h"
#include "logger.h"
#include "ui_mainwindow.h"

#include <QCoreApplication>
#include <QCloseEvent>
#include <QEvent>
#include <QPixmap>
#include <QMessageBox>
#include <QScopedValueRollback>
#include <QSignalBlocker>
#include <QTimer>
#include <QVariant>

// A meglévő Designer-ablakot építi fel, majd beköti a műveleteket és visszajelzéseket.
MainWindow::MainWindow(LicenceManager &licences, const Logger &logger, LanguageManager &languages,
                       ApplicationSettings &settings, ConnectionMonitor &monitor, bool showAtStartup)
    : ui(std::make_unique<Ui::WebAppWindow>()), languages(languages), logError(logger.error())
{
    ui->setupUi(this);
    ui->showAtStartup->setChecked(showAtStartup);
    // Csak a mentés gomb ír az adatbázisba; az aktuális ablak láthatósága nem változik.
    connect(ui->saveSettings, &QPushButton::clicked, this, [this, &settings] {
        const bool saved = settings.saveShowWindow(ui->showAtStartup->isChecked());
        QMessageBox result(saved ? QMessageBox::Information : QMessageBox::Warning,
                           tr("Startup settings"),
                           saved ? tr("The startup setting was saved.") : tr("Could not save the startup setting."),
                           QMessageBox::NoButton, this);
        result.addButton(tr("OK"), QMessageBox::AcceptRole);
        result.exec();
    });
    ui->language->addItem(QStringLiteral("English"), QStringLiteral("en"));
    ui->language->addItem(QStringLiteral("Magyar"), QStringLiteral("hu"));
    retranslateWindow();
    // A lista csak előkészíti a választást. A fordítót a mellette lévő gomb aktiválja.
    connect(ui->reloadLanguage, &QPushButton::clicked, this, [this] {
        const auto language = ui->language->currentData().toString();
        if (language.isEmpty() || language == this->languages.currentLanguage())
            return;
        if (!this->languages.setLanguage(language))
            syncLanguageSelection();
    });

    const auto refresh = [this, &licences] {
        resetLocalLicence();
        licences.refreshLocalLicence();
    };
    connect(ui->exitButton, &QPushButton::clicked, this, &MainWindow::quitApplication);
    connect(ui->databaseRefresh, &QPushButton::clicked, this, refresh);
    connect(ui->licenceRefresh, &QPushButton::clicked, this, refresh);
    connect(ui->serverRefresh, &QPushButton::clicked, &licences, &LicenceManager::checkServer);
    connect(ui->onlineButton, &QPushButton::clicked, this, [this, &licences] {
        resetLocalLicence();
        showOnlineMessage({});
        licences.checkOnline();
    });

    connect(&licences, &LicenceManager::databaseStatusChanged, this, &MainWindow::showDatabaseResult);
    connect(&licences, &LicenceManager::localLicenceRead, this, &MainWindow::showLocalLicence);
    connect(&licences, &LicenceManager::localLicenceReadFailed, this, [this](const UiMessage &message) {
        localMessage = message;
        ui->licenceStatus->setText(localMessage.text());
    });
    connect(&licences, &LicenceManager::serverAddressChanged, ui->serverAddress, &QLineEdit::setText);
    const auto serverUnavailable = [this](const UiMessage &message) {
        serverStatus = ServerStatus::NotStarted;
        serverDetails = message;
        renderServerStatus();
    };
    connect(&licences, &LicenceManager::serverCheckNotStarted, this, serverUnavailable);
    connect(&licences, &LicenceManager::serverTokenResult, this, &MainWindow::showServerResult);
    connect(&licences, &LicenceManager::networkBusyChanged, this, &MainWindow::setNetworkBusy);
    connect(&licences, &LicenceManager::onlineStarted, this, [this] {
        showOnlineMessage(UiMessage([] { return tr("Checking licence online…"); }));
    });
    connect(&licences, &LicenceManager::onlineFinished, this, &MainWindow::showOnlineMessage);
    connect(&monitor, &ConnectionMonitor::databaseChecked, this, &MainWindow::showDatabaseResult);
    connect(&monitor, &ConnectionMonitor::serverChecked, this, &MainWindow::showServerResult);
    connect(&monitor, &ConnectionMonitor::serverUnavailable, this, serverUnavailable);
    connect(&monitor, &ConnectionMonitor::probeBusyChanged, this, [this](bool busy) {
        ui->serverRefresh->setEnabled(!busy);
        ui->onlineButton->setEnabled(!busy);
    });
    QTimer::singleShot(0, this, refresh);
}

MainWindow::~MainWindow() = default;

void MainWindow::setTrayAvailable(bool available)
{
    trayAvailable = available;
}

void MainWindow::bringToFront()
{
    auto state = windowState();
    state.setFlag(Qt::WindowMinimized, false);
    setWindowState(state);
    show();
    raise();
    activateWindow();
}

// A valódi kilépés a tálcamenüből és az ablak Kilépés gombjáról is ide érkezik.
void MainWindow::quitApplication()
{
    if (quitting || confirmingQuit)
        return;

    // A tálcamenüből is ugyanaz az egy megerősítő ablak nyílhat meg.
    const QScopedValueRollback<bool> confirmation(confirmingQuit, true);
    QMessageBox prompt(QMessageBox::Warning, tr("Exit application"),
                       tr("Are you sure you want to close the application? Synchronization will stop if you exit."),
                       QMessageBox::NoButton, this);
    const auto *exit = prompt.addButton(tr("Exit"), QMessageBox::AcceptRole);
    auto *cancel = prompt.addButton(tr("Cancel"), QMessageBox::RejectRole);
    prompt.setDefaultButton(cancel);
    prompt.setEscapeButton(cancel);
    prompt.exec();
    if (prompt.clickedButton() != exit)
        return;

    quitting = true;
    QCoreApplication::quit();
}

// Tálcaikon nélkül csak jóváhagyott kilépéskor zárjuk be az ablakot.
void MainWindow::closeEvent(QCloseEvent *event)
{
    if (quitting) {
        event->accept();
        return;
    }
    event->ignore();
    if (trayAvailable)
        hide();
    else
        QTimer::singleShot(0, this, &MainWindow::quitApplication);
}

void MainWindow::reject()
{
    close();
}

void MainWindow::changeEvent(QEvent *event)
{
    QDialog::changeEvent(event);
    if (event->type() == QEvent::LanguageChange) {
        retranslateWindow();
        emit languageChanged();
    }
}

void MainWindow::syncLanguageSelection()
{
    const QSignalBlocker blocker(ui->language);
    ui->language->setCurrentIndex(ui->language->findData(languages.currentLanguage()));
}

// A Designer feliratai után az aktuális állapotokat is újrarajzolja, adatlekérés nélkül.
void MainWindow::retranslateWindow()
{
    ui->retranslateUi(this);
    setWindowTitle(QStringLiteral("Belenus WebApp - v%1").arg(QCoreApplication::applicationVersion()));
    syncLanguageSelection();
    ui->logStatus->setVisible(!logError.isEmpty());
    if (!logError.isEmpty())
        ui->logStatus->setText(tr("Cannot open or write to webapp.log: %1").arg(logError));
    renderDatabaseStatus();
    renderServerStatus();
    if (!localMessage.isEmpty())
        ui->licenceStatus->setText(localMessage.text());
    if (!onlineMessage.isEmpty())
        ui->onlineStatus->setText(onlineMessage.text());
}

// Kicseréli a jelző ikonját és feliratát, egyben törli a korábbi hibaleírást.
void MainWindow::showConnection(QLabel *light, QLabel *status, const char *icon, const QString &message)
{
    light->setPixmap(QPixmap(QStringLiteral(":/icons/status_%1.png").arg(QString::fromLatin1(icon))));
    status->setText(message);
    status->setToolTip({});
}

void MainWindow::showDatabaseResult(DatabaseManager::Status status, const UiMessage &details)
{
    databaseStatus = status;
    databaseDetails = details;
    renderDatabaseStatus();
}

void MainWindow::renderDatabaseStatus()
{
    if (!databaseStatus)
        return;
    switch (*databaseStatus) {
    case DatabaseManager::Status::Connected:
        showConnection(ui->databaseLight, ui->databaseStatus, "green", tr("Connection succeeded."));
        break;
    case DatabaseManager::Status::DriverUnavailable:
        showConnection(ui->databaseLight, ui->databaseStatus, "red", tr("The QMYSQL driver could not be loaded."));
        break;
    case DatabaseManager::Status::OpenFailed:
        showConnection(ui->databaseLight, ui->databaseStatus, "red", tr("Connection failed."));
        ui->databaseStatus->setToolTip(databaseDetails.text());
        break;
    }
}

// A szerverjelző a tokenkérés technikai eredményét mutatja, a licenckód ettől külön jelenik meg.
void MainWindow::showServerResult(bool ok)
{
    serverStatus = ok ? ServerStatus::Connected : ServerStatus::Failed;
    renderServerStatus();
}

void MainWindow::renderServerStatus()
{
    switch (serverStatus) {
    case ServerStatus::Unchecked:
        break;
    case ServerStatus::Connecting:
        showConnection(ui->serverLight, ui->serverStatus, "yellow", tr("Connecting…"));
        break;
    case ServerStatus::Connected:
        showConnection(ui->serverLight, ui->serverStatus, "green", tr("HTTPS connection established."));
        break;
    case ServerStatus::Failed:
        showConnection(ui->serverLight, ui->serverStatus, "red", tr("Token request failed."));
        break;
    case ServerStatus::NotStarted:
        showConnection(ui->serverLight, ui->serverStatus, "yellow", tr("The server check did not start."));
        ui->serverStatus->setToolTip(serverDetails.text());
        break;
    }
}

void MainWindow::setNetworkBusy(bool busy)
{
    ui->serverRefresh->setEnabled(!busy);
    ui->onlineButton->setEnabled(!busy);
    if (busy) {
        serverStatus = ServerStatus::Connecting;
        renderServerStatus();
    }
}

void MainWindow::showOnlineMessage(const UiMessage &message)
{
    onlineMessage = message;
    ui->onlineStatus->setText(onlineMessage.text());
}

void MainWindow::resetLocalLicence()
{
    ui->localSerial->clear();
    ui->localDate->clear();
    ui->localServerCode->clear();
    ui->localClientCode->clear();
    localMessage = {};
    ui->licenceStatus->setText(tr("Not loaded yet."));
}

// A betöltött rekord mezőit jeleníti meg, és külön jelzi az üres kulcsot.
void MainWindow::showLocalLicence(const Licence &licence)
{
    ui->localSerial->setText(licence.serial);
    ui->localDate->setText(licence.checkedAt);
    ui->localServerCode->setText(licence.serverCode);
    ui->localClientCode->setText(licence.clientCode);
    localMessage = UiMessage([emptyKey = licence.serial.trimmed().isEmpty(), status = licence.status] {
        return emptyKey ? tr("The local licence key is empty.") : localLicenceStatus(status);
    });
    ui->licenceStatus->setText(localMessage.text());
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
