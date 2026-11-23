//====================================================================================
//
// Belenus WebApp alkalmazas (c) Bikfalvi Tamas
//
//====================================================================================

#pragma once

#include <QTranslator>

class Logger;

// A Belenus nyelvi beállítását olvassa, és a WebApp saját fordítóját tartja életben.
class LanguageManager {
public:
    explicit LanguageManager(Logger &logger);
    void install();

private:
    QString selectedLanguage() const;

    Logger &logger;
    QTranslator translator;
};
