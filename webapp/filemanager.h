//====================================================================================
//
// Belenus WebApp alkalmazas (c) Bikfalvi Tamas
//
//====================================================================================

#pragma once

#include <QString>

class QByteArray;

// Az EXE melletti diagnosztikai fájlok technikai kezelését végzi.
class FileManager {
public:
    QString applicationFilePath(const QString &name) const;
    bool save(const QString &path, const QByteArray &body, QString &error) const;
};
