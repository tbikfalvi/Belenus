//====================================================================================
//
// Belenus WebApp alkalmazas (c) Bikfalvi Tamas
//
//====================================================================================

#pragma once

#include "uimessage.h"

class DatabaseManager;
class Logger;
class QUrl;

// A WebApp indulási beállítását a meglévő settings táblában kezeli.
class ApplicationSettings {
public:
    ApplicationSettings(DatabaseManager &database, Logger &logger);
    bool loadShowWindow();
    bool saveShowWindow(bool show);

    // A licencellenőrzés és a kapcsolatfigyelés ugyanazt a szerverbeállítást olvassa.
    static UiMessage readServerAddress(DatabaseManager &database, QUrl &server);

private:
    enum class Lookup { Found, Missing, Failed };
    Lookup readShowWindow(QString &value);

    DatabaseManager &database;
    Logger &logger;
};
