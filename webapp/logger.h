//====================================================================================
//
// Belenus WebApp alkalmazas (c) Bikfalvi Tamas
//
//====================================================================================

#pragma once

#include <QFile>

// Az alkalmazás közös fájlnaplója. A hívók csak naplózható eseményadatokat adnak át.
class Logger {
public:
    bool open();
    void write(const char *level, const QString &event);
    const QString &error() const;

private:
    QFile file;
    QString lastError;
};
