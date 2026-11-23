//====================================================================================
//
// Belenus WebApp alkalmazas (c) Bikfalvi Tamas
//
//====================================================================================

#pragma once

#include <QSqlDatabase>
#include <QSqlQuery>

class Logger;
class QSqlError;

// Egyetlen névvel regisztrált helyi QMYSQL-kapcsolatot birtokol.
// A feladatmodulok adják az SQL szövegét, a végrehajtást ez az osztály végzi.
class DatabaseManager {
public:
    enum class Status { Connected, DriverUnavailable, OpenFailed };

    explicit DatabaseManager(Logger &logger);
    ~DatabaseManager();
    DatabaseManager(const DatabaseManager &) = delete;
    DatabaseManager &operator=(const DatabaseManager &) = delete;

    // A parancssori próba csak a meghajtó betöltését ellenőrzi.
    bool driverAvailable() const;
    QString driverMessage() const;

    Status open();
    // A hívó előbb engedje el az összes, kapcsolatból kapott QSqlQuery objektumot.
    void close();
    QString connectionError() const;

    QSqlQuery executeSelect(const QString &sql);
    bool executeNonQuery(const QString &sql);

    static QString errorCodeSuffix(const QSqlError &error);

private:
    QSqlQuery execute(const QString &sql);

    Logger &logger;
    QSqlDatabase connection;
};
