#ifndef PREFERENCES_H
#define PREFERENCES_H

#include <QSettings>
#include <QString>

class Preferences
{
    typedef QMap<QString, QString> PreferencesList;

public:
    Preferences();
    virtual ~Preferences();

    void loadFromFile( const QString filename );
    void saveFile( const QString filename = "" );

    void setValue(QString key, QString value);
    QString value(QString key, QString def = "");

protected:
    PreferencesList _preferences;
    QString _filename;
};

#endif
