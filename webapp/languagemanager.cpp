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
    if (!setLanguage(selectedLanguage()))
        setLanguage(QStringLiteral("en"));
}

QString LanguageManager::currentLanguage() const
{
    return activeLanguage;
}

// A futás közbeni választás csak memóriában él. A régi fordítót sikeres betöltésig megtartjuk.
bool LanguageManager::setLanguage(const QString &language)
{
    if (language != QStringLiteral("en") && language != QStringLiteral("hu"))
        return false;
    if (translator && language == activeLanguage)
        return true;

    const auto catalog = QStringLiteral(":/i18n/webapp_%1.qm").arg(language);
    auto next = std::make_unique<QTranslator>();
    const auto previousLanguage = activeLanguage;
    activeLanguage = language;
    if (next->load(catalog) && QCoreApplication::installTranslator(next.get())) {
        if (translator)
            QCoreApplication::removeTranslator(translator.get());
        translator = std::move(next);
        return true;
    }

    logger.write("WARN", QStringLiteral("Could not install translation %1.").arg(catalog));
    if (language == QStringLiteral("en")) {
        // Angol katalógus nélkül is az angol forrásszövegek jelennek meg.
        if (translator)
            QCoreApplication::removeTranslator(translator.get());
        translator.reset();
        return true;
    }
    activeLanguage = previousLanguage;
    return false;
}
