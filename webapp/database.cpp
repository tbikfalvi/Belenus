//====================================================================================
//
// Belenus WebApp alkalmazas (c) Bikfalvi Tamas
//
//====================================================================================

#include "database.h"

#include "logger.h"

#include <QObject>
#include <QRegularExpression>
#include <QSqlError>
#include <QStringList>

namespace {

constexpr auto connectionName = "webapp-mysql-probe";

// A felületen megjelenő hibaüzenetből eltávolítja a kapcsolati adatokat.
QString safeConnectionError(QString text, const QString &password)
{
    static const QRegularExpression details(
        QStringLiteral(R"([A-Za-z][A-Za-z0-9+.-]*://\S+|\b(?:dsn|driver|server|host|database|uid|user(?:name)?|pwd|password)\s*=.*)"),
        QRegularExpression::CaseInsensitiveOption);
    text.replace(details, QStringLiteral("[redacted]"));
    if (!password.isEmpty())
        text.replace(password, QStringLiteral("[redacted]"));
    return text;
}

} // namespace

DatabaseManager::DatabaseManager(Logger &logger)
    : logger(logger),
      connection(QSqlDatabase::addDatabase(QStringLiteral("QMYSQL"), connectionName))
{
    if (!connection.isValid())
        return;
    const auto localAccount = QStringLiteral("belenus");
    connection.setHostName(QStringLiteral("127.0.0.1"));
    connection.setPort(3306);
    connection.setDatabaseName(localAccount);
    connection.setUserName(localAccount);
    connection.setPassword(localAccount);
    connection.setConnectOptions(QStringLiteral("MYSQL_OPT_CONNECT_TIMEOUT=5;MYSQL_OPT_READ_TIMEOUT=5;"
                                                "MYSQL_OPT_SSL_VERIFY_SERVER_CERT=0"));
}

DatabaseManager::~DatabaseManager()
{
    close();
    connection = QSqlDatabase();
    QSqlDatabase::removeDatabase(connectionName);
}

bool DatabaseManager::driverAvailable() const
{
    return connection.isValid();
}

QString DatabaseManager::driverMessage() const
{
    if (driverAvailable())
        return QObject::tr("QMYSQL loaded.");
    const auto drivers = QSqlDatabase::drivers().join(QStringLiteral(", "));
    return QObject::tr("QMYSQL could not be loaded (%1). Available drivers: %2.")
        .arg(connection.lastError().text(), drivers);
}

// Megnyitja a helyi kapcsolatot, vagy visszaadja a technikai hiba típusát.
DatabaseManager::Status DatabaseManager::open()
{
    if (!driverAvailable()) {
        logger.write("ERROR", QStringLiteral("Database connection unavailable; QMYSQL did not load"));
        return Status::DriverUnavailable;
    }
    if (connection.isOpen())
        return Status::Connected;
    if (!connection.open()) {
        logger.write("ERROR", QStringLiteral("Database connection failed%1").arg(errorCodeSuffix(connection.lastError())));
        return Status::OpenFailed;
    }
    logger.write("INFO", QStringLiteral("Database connection opened"));
    return Status::Connected;
}

void DatabaseManager::close()
{
    if (connection.isOpen())
        connection.close();
}

// A felület részletes diagnosztikája. A naplóba csak numerikus hibakód kerül.
QString DatabaseManager::connectionError() const
{
    const auto error = connection.lastError();
    const auto password = connection.password();
    return QObject::tr("QMYSQL loaded, but the connection failed.\n"
                       "nativeErrorCode(): %1\n"
                       "driverText(): %2\n"
                       "databaseText(): %3")
        .arg(safeConnectionError(error.nativeErrorCode(), password),
             safeConnectionError(error.driverText(), password),
             safeConnectionError(error.databaseText(), password));
}

// A teljes SELECT utasítást hajtja végre. Az eredmény a next() hívással járható be.
// A kapcsolat a visszaadott lekérdezés teljes élettartama alatt nyitva marad.
QSqlQuery DatabaseManager::executeSelect(const QString &sql)
{
    return execute(sql);
}

// Egy teljes adatmódosító SQL utasítást hajt végre. Siker esetén true-t ad.
bool DatabaseManager::executeNonQuery(const QString &sql)
{
    return execute(sql).isActive();
}

// Az SQL szövegét és a nyers adatbázis-hibát nem írja a naplóba.
QSqlQuery DatabaseManager::execute(const QString &sql)
{
    if (open() != Status::Connected)
        return QSqlQuery();
    QSqlQuery query(connection);
    if (!query.exec(sql))
        logger.write("ERROR", QStringLiteral("Database query failed%1").arg(errorCodeSuffix(query.lastError())));
    return query;
}

QString DatabaseManager::errorCodeSuffix(const QSqlError &error)
{
    const auto code = error.nativeErrorCode();
    static const QRegularExpression numeric(QStringLiteral("^[0-9]+$"));
    return numeric.match(code).hasMatch() ? QObject::tr(" (MySQL error %1)").arg(code) : QString();
}
