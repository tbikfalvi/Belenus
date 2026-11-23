//====================================================================================
//
// Belenus WebApp alkalmazas (c) Bikfalvi Tamas
//
//====================================================================================

#pragma once

#include "database.h"

#include <QDialog>
#include <QCoreApplication>

#include <memory>
#include <optional>

class QEvent;
class QCloseEvent;
class QLabel;
class ApplicationSettings;
class ConnectionMonitor;
class LanguageManager;
class LicenceManager;
class Logger;
struct Licence;
namespace Ui { class WebAppWindow; }

// A Designer-felület vezérlőit a LicenceManager műveleteihez és jelzéseihez köti.
class MainWindow : public QDialog {
    Q_OBJECT

public:
    MainWindow(LicenceManager &licences, const Logger &logger, LanguageManager &languages,
               ApplicationSettings &settings, ConnectionMonitor &monitor, bool showAtStartup);
    ~MainWindow() override;
    void setTrayAvailable(bool available);
    void bringToFront();
    void quitApplication();

signals:
    void languageChanged();

protected:
    void changeEvent(QEvent *event) override;
    void closeEvent(QCloseEvent *event) override;
    void reject() override;

private:
    enum class ServerStatus { Unchecked, Connecting, Connected, Failed, NotStarted };

    void retranslateWindow();
    void syncLanguageSelection();
    static void showConnection(QLabel *light, QLabel *status, const char *icon, const QString &message);
    void showDatabaseResult(DatabaseManager::Status status, const UiMessage &details);
    void renderDatabaseStatus();
    void showServerResult(bool ok);
    void renderServerStatus();
    void setNetworkBusy(bool busy);
    void showOnlineMessage(const UiMessage &message);
    void resetLocalLicence();
    void showLocalLicence(const Licence &licence);
    static QString localLicenceStatus(const QString &status);

    std::unique_ptr<Ui::WebAppWindow> ui;
    LanguageManager &languages;
    QString logError;
    std::optional<DatabaseManager::Status> databaseStatus;
    UiMessage databaseDetails;
    ServerStatus serverStatus = ServerStatus::Unchecked;
    UiMessage serverDetails;
    UiMessage localMessage;
    UiMessage onlineMessage;
    bool trayAvailable = false;
    bool confirmingQuit = false;
    bool quitting = false;
};
