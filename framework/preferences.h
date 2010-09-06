#ifndef PREFERENCES_H
#define PREFERENCES_H

#include <QSettings>
#include <QString>

class Preferences
{
    typedef QMap<QString, QString> PreferencesList;

public:
    Preferences(const QString filename = "");
    virtual ~Preferences();

    void setFilename(const QString filename);
    void loadFile();
    void saveFile();

    void setValue(QString key, QString value);
    QString value(QString key, QString def = "");

protected:
    virtual void checkAndSetDefaults() = 0;

    PreferencesList _preferences;
    QString _filename;
};

#endif
