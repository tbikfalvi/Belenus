//====================================================================================
//
// Belenus WebApp alkalmazas (c) Bikfalvi Tamas
//
//====================================================================================

#include "applicationsettings.h"

#include "database.h"
#include "logger.h"

#include <QObject>
#include <QSqlError>
#include <QUrl>
#include <QVariant>

ApplicationSettings::ApplicationSettings(DatabaseManager &database, Logger &logger)
    : database(database), logger(logger)
{
}

ApplicationSettings::Lookup ApplicationSettings::readShowWindow(QString &value)
{
    auto query = database.executeSelect(QStringLiteral(
        "SELECT value FROM settings WHERE identifier = 'WEBAPP_ShowWindowOnStart'"));
    if (!query.isActive())
        return Lookup::Failed;
    if (!query.next())
        return query.lastError().isValid() ? Lookup::Failed : Lookup::Missing;
    value = query.value(0).toString();
    return Lookup::Found;
}

// Csak a sikeres, üres SELECT után hozható létre az alapértelmezett rekord.
bool ApplicationSettings::loadShowWindow()
{
    QString value;
    const auto lookup = readShowWindow(value);
    bool show = true;
    if (lookup == Lookup::Found && (value == QStringLiteral("0") || value == QStringLiteral("1"))) {
        show = value == QStringLiteral("1");
    } else if (lookup == Lookup::Missing) {
        const bool inserted = database.executeNonQuery(QStringLiteral(
            "INSERT INTO settings (identifier, value) VALUES ('WEBAPP_ShowWindowOnStart', '1')"));
        logger.write(inserted ? "INFO" : "ERROR",
                     inserted ? QStringLiteral("Created WEBAPP_ShowWindowOnStart with default value 1.")
                              : QStringLiteral("Could not create WEBAPP_ShowWindowOnStart. Showing the window."));
    } else {
        logger.write("WARN", lookup == Lookup::Failed
                                 ? QStringLiteral("Could not read WEBAPP_ShowWindowOnStart. Showing the window.")
                                 : QStringLiteral("Invalid WEBAPP_ShowWindowOnStart. Showing the window."));
    }
    database.close();
    return show;
}

// Csak a meglévő rekordot módosítja. A visszaolvasás a változatlan értéket is elfogadja.
bool ApplicationSettings::saveShowWindow(bool show)
{
    const auto desired = show ? QStringLiteral("1") : QStringLiteral("0");
    const bool updated = database.executeNonQuery(QStringLiteral(
        "UPDATE settings SET value = '%1' WHERE identifier = 'WEBAPP_ShowWindowOnStart'").arg(desired));
    QString value;
    const bool saved = updated && readShowWindow(value) == Lookup::Found && value == desired;
    database.close();
    logger.write(saved ? "INFO" : "ERROR", saved ? QStringLiteral("Saved WEBAPP_ShowWindowOnStart.")
                                                 : QStringLiteral("Could not save WEBAPP_ShowWindowOnStart."));
    return saved;
}

// A meglévő WebSync-beállításból a licenckéréssel azonos szabályok szerint képez HTTPS-alapcímet.
UiMessage ApplicationSettings::readServerAddress(DatabaseManager &database, QUrl &server)
{
    auto query = database.executeSelect(
        QStringLiteral("SELECT value FROM settings WHERE identifier = 'SERVER_Address'"));
    if (!query.isActive() || (!query.next() && query.lastError().isValid()))
        return UiMessage([error = query.lastError()] {
            return QObject::tr("Database error while reading the server address%1.")
                .arg(DatabaseManager::errorCodeSuffix(error));
        });

    if (query.isValid())
        server = QUrl(query.value(0).toString().trimmed(), QUrl::StrictMode);
    if (server.scheme() == QStringLiteral("http"))
        server.setScheme(QStringLiteral("https"));
    if (!server.isValid() || server.scheme() != QStringLiteral("https") || server.host().isEmpty() ||
        !server.userInfo().isEmpty() || server.hasQuery() || server.hasFragment())
        return UiMessage([] { return QObject::tr("Missing or invalid HTTPS server address (SERVER_Address)."); });
    return {};
}
