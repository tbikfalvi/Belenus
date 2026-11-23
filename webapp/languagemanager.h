//====================================================================================
//
// Belenus WebApp alkalmazas (c) Bikfalvi Tamas
//
//====================================================================================

#pragma once

#include <QTranslator>

#include <memory>

class Logger;

// A Belenus nyelvi beállítását olvassa, és a WebApp saját fordítóját tartja életben.
class LanguageManager {
public:
    explicit LanguageManager(Logger &logger);
    void install();
    bool setLanguage(const QString &language);
    QString currentLanguage() const;

private:
    QString selectedLanguage() const;

    Logger &logger;
    std::unique_ptr<QTranslator> translator;
    QString activeLanguage = QStringLiteral("en");
};
