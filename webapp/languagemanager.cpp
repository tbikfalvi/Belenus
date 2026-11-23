//====================================================================================
//
// Belenus WebApp alkalmazas (c) Bikfalvi Tamas
//
//====================================================================================

#include "languagemanager.h"

#include "logger.h"

#include <QCoreApplication>
#include <QDir>
#include <QFile>
#include <QXmlStreamReader>

LanguageManager::LanguageManager(Logger &logger) : logger(logger)
{
}

// A függvény csak olvassa a konfigurációt. Egyetlen, pontosan "yes" értékű bejegyzést fogad el.
QString LanguageManager::selectedLanguage() const
{
    const auto path = QDir::cleanPath(QDir(QCoreApplication::applicationDirPath())
                                        .filePath(QStringLiteral("../Belenus/lang/languages.inf")));
    QFile file(path);
    if (!file.open(QIODevice::ReadOnly)) {
        logger.write("WARN", QStringLiteral("Cannot read language configuration %1: %2. Using English.")
                                 .arg(path, file.errorString()));
        return QStringLiteral("en");
    }

    QXmlStreamReader xml(&file);
    if (!xml.readNextStartElement() || xml.qualifiedName() != QStringLiteral("languages")) {
        logger.write("WARN", QStringLiteral("Invalid language configuration root in %1. Using English.").arg(path));
        return QStringLiteral("en");
    }

    int activeCount = 0;
    QString language;
    while (xml.readNextStartElement()) {
        const auto attributes = xml.attributes();
        if (xml.qualifiedName() == QStringLiteral("language") &&
            attributes.value(QStringLiteral("current")) == QStringLiteral("yes")) {
            ++activeCount;
            language = attributes.value(QStringLiteral("shortname")).toString();
        }
        xml.skipCurrentElement();
    }

    // A gyökérelem mögötti hibás tartalmat is észleljük, mielőtt elfogadnánk a nyelvet.
    while (!xml.atEnd())
        xml.readNext();
    if (xml.hasError() || file.error() != QFileDevice::NoError) {
        logger.write("WARN", QStringLiteral("Invalid or unreadable language configuration %1 (line %2). Using English.")
                                 .arg(path).arg(xml.lineNumber()));
        return QStringLiteral("en");
    }
    if (activeCount != 1 || language.isEmpty()) {
        logger.write("WARN", QStringLiteral("No unambiguous active language in %1. Using English.").arg(path));
        return QStringLiteral("en");
    }
    if (language == QStringLiteral("hu") || language == QStringLiteral("en"))
        return language;

    logger.write("WARN", QStringLiteral("Unsupported active language in %1. Using English.").arg(path));
    return QStringLiteral("en");
}

// A qmake által beágyazott katalógust a főablak létrehozása előtt telepíti.
void LanguageManager::install()
{
    const auto language = selectedLanguage();
    const auto catalog = QStringLiteral(":/i18n/webapp_%1.qm").arg(language);
    if (translator.load(catalog) && QCoreApplication::installTranslator(&translator))
        return;

    // A forrásszövegek angolok, ezért hibás katalógus mellett is használható a felület.
    logger.write("WARN", QStringLiteral("Could not install translation %1. Using English source text.").arg(catalog));
}
