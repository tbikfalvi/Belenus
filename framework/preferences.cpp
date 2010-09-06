
#include <QString>
#include <QStringList>
#include "preferences.h"



Preferences::Preferences(const QString filename)
{
    setFilename(filename);
}



void Preferences::setFilename(const QString file)
{
    _filename = file;
}


Preferences::~Preferences()
{
    if ( _filename!="" )
        saveFile();
}



void Preferences::loadFile()
{
    if ( _filename=="" )
        return;

    // create a qsettings
    QSettings settings(_filename, QSettings::IniFormat);

    // load everything from the file into the inner list
    QStringList list = settings.allKeys();
    QStringList::iterator it;
    for ( it = list.begin(); it!=list.end(); ++it )
        setValue(*it, settings.value(*it).toString());

    checkAndSetDefaults();
}



void Preferences::saveFile()
{
    // create an inifile and put everything from preferenceslist into it.
    QSettings settings(_filename, QSettings::IniFormat);
    if (!settings.isWritable())
        return;

    PreferencesList::iterator it;
    for ( it = _preferences.begin(); it!=_preferences.end(); ++it )
        settings.setValue(it.key(), it.value());

    settings.sync();
}



void Preferences::setValue(QString key, QString value)
{
    _preferences[key] = value;
}



QString Preferences::value(QString key, QString def)
{
    PreferencesList::const_iterator it = _preferences.find(key);
    if ( it==_preferences.end() )
        return def;
    return it.value();
}
