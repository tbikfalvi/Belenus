//====================================================================================
//
// Belenus WebApp alkalmazas (c) Bikfalvi Tamas
//
//====================================================================================

#include "filemanager.h"

#include <QCoreApplication>
#include <QDir>
#include <QSaveFile>

QString FileManager::applicationFilePath(const QString &name) const
{
    return QDir(QCoreApplication::applicationDirPath()).filePath(name);
}

// A path célfájlt a body változatlan bájtjaival cseréli le.
// Sikertelen mentéskor false értéket és az error paraméterben fájlhibát ad vissza.
bool FileManager::save(const QString &path, const QByteArray &body, QString &error) const
{
    QSaveFile file(path);
    if (file.open(QIODevice::WriteOnly) && file.write(body) == body.size() && file.commit())
        return true;
    error = file.errorString();
    return false;
}
