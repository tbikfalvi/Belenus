

#include "preferences.h"



Preferences::Preferences()
{
}



Preferences::~Preferences()
{
    if ( _filename!="" )
        saveFile();
}



void Preferences::loadFromFile( const QString filename )
{
}



void Preferences::saveFile( const QString filename )
{
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
