//====================================================================================
//
// Belenus WebApp alkalmazas (c) Bikfalvi Tamas
//
//====================================================================================

#include "logger.h"

#include <QCoreApplication>
#include <QDateTime>
#include <QDir>

// Az EXE melletti naplóhoz fűz hozzá. A hiba a felületen is megjeleníthető.
bool Logger::open()
{
    file.setFileName(QDir(QCoreApplication::applicationDirPath()).filePath(QStringLiteral("webapp.log")));
    if (file.open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Text))
        return true;
    lastError = file.errorString();
    return false;
}

// A level a naplózási szint, az event a hívó által összeállított eseményszöveg.
void Logger::write(const char *level, const QString &event)
{
    if (!file.isOpen())
        return;
    const auto data = QStringLiteral("%1 [%2] %3\n")
                          .arg(QDateTime::currentDateTime().toString(Qt::ISODateWithMs),
                               QString::fromLatin1(level), event).toUtf8();
    if (file.write(data) != data.size() || !file.flush()) {
        lastError = file.errorString();
        file.close();
    }
}

const QString &Logger::error() const
{
    return lastError;
}
