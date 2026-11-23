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

class QLabel;
class LicenceManager;
class Logger;
struct Licence;
namespace Ui { class WebAppWindow; }

// A Designer-felület vezérlőit a LicenceManager műveleteihez és jelzéseihez köti.
class MainWindow : public QDialog {
    Q_DECLARE_TR_FUNCTIONS(MainWindow)

public:
    MainWindow(LicenceManager &licences, const Logger &logger);
    ~MainWindow() override;

private:
    static void showConnection(QLabel *light, QLabel *status, const char *icon, const QString &message);
    void showDatabaseResult(DatabaseManager::Status status, const QString &details);
    void showServerResult(bool ok);
    void setNetworkBusy(bool busy);
    void resetLocalLicence();
    void showLocalLicence(const Licence &licence);
    static QString localLicenceStatus(const QString &status);

    std::unique_ptr<Ui::WebAppWindow> ui;
};
