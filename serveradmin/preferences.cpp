//====================================================================================
//
// Belenus Server alkalmazas © Pagony Multimedia Studio Bt - 2010
//
//====================================================================================
//
// Filename    : preferences.cpp
// AppVersion  : 1.0
// FileVersion : 1.0
// Author      : Ballok Peter
//
//====================================================================================
// Beallitasok kezeleseert felelos osztaly implementacioja
//====================================================================================

#include "preferences.h"
#include "belenusserveradmin.h"

//====================================================================================
// cPreferences::cPreferences
//------------------------------------------------------------------------------------
cPreferences::cPreferences()
{
    init();
}
//====================================================================================
// cPreferences::cPreferences
//------------------------------------------------------------------------------------
cPreferences::cPreferences( const QString &p_qsFileName )
{
    init();
    setFileName( p_qsFileName );
    loadConfFileSettings();
}
//====================================================================================
// cPreferences::~cPreferences
//------------------------------------------------------------------------------------
cPreferences::~cPreferences()
{
}
//====================================================================================
// cPreferences::init
//------------------------------------------------------------------------------------
void cPreferences::init()
{
    m_qsFileName                = "";
    m_qsVersion                 = "";
    m_qsLangFilePrefix          = "";
    m_qsLang                    = "";
    m_nWindowWidth              = 400;
    m_nWindowHeight             = 100;
    m_qsAddress                 = "";
    m_nPort                     = 4000;
    m_nMaxCorruptMessageCount   = 5;
}
//====================================================================================
// cPreferences::setFileName
//------------------------------------------------------------------------------------
void cPreferences::setFileName( const QString &p_qsFileName )
{
    m_qsFileName = p_qsFileName;
}
//====================================================================================
// cPreferences::getFileName
//------------------------------------------------------------------------------------
QString cPreferences::getFileName() const
{
    return m_qsFileName;
}
//====================================================================================
// cPreferences::setVersion
//------------------------------------------------------------------------------------
void cPreferences::setVersion( const QString &p_qsVersion, bool p_boSaveNow )
{
    m_qsVersion = p_qsVersion;

    if( p_boSaveNow )
    {
        QSettings  obPrefFile( m_qsFileName, QSettings::IniFormat );
        obPrefFile.setValue( QString::fromAscii( "Version" ), m_qsVersion );
    }
}
//====================================================================================
// cPreferences::getVersion
//------------------------------------------------------------------------------------
QString cPreferences::getVersion() const
{
    return m_qsVersion;
}
//====================================================================================
// cPreferences::setLangFilePrefix
//------------------------------------------------------------------------------------
void cPreferences::setLangFilePrefix( const QString &p_qsPrefix )
{
    m_qsLangFilePrefix = p_qsPrefix;
}
//====================================================================================
// cPreferences::getLangFilePrefix
//------------------------------------------------------------------------------------
QString cPreferences::getLangFilePrefix() const
{
    return m_qsLangFilePrefix;
}
//====================================================================================
// cPreferences::setLang
//------------------------------------------------------------------------------------
void cPreferences::setLang( const QString &p_qsLang, bool p_boSaveNow )
{
    m_qsLang = p_qsLang;

    if( p_boSaveNow )
    {
        QSettings  obPrefFile( m_qsFileName, QSettings::IniFormat );
        obPrefFile.setValue( QString::fromAscii( "Lang" ), m_qsLang );
    }
}
//====================================================================================
// cPreferences::getLang
//------------------------------------------------------------------------------------
QString cPreferences::getLang() const
{
    return m_qsLang;
}
//====================================================================================
// cPreferences::setLogLevels
//------------------------------------------------------------------------------------
void cPreferences::setLogLevels( const unsigned int p_uiConLevel,
                                 const unsigned int p_uiDBLevel,
                                 const unsigned int p_uiGUILevel,
                                 bool p_boSaveNow )
{
    g_obLogger.setMinSeverityLevels( (cSeverity::teSeverity)p_uiConLevel,
                                     (cSeverity::teSeverity)p_uiDBLevel,
                                     (cSeverity::teSeverity)p_uiGUILevel );

    if( p_boSaveNow )
    {
        QSettings  obPrefFile( m_qsFileName, QSettings::IniFormat );
        obPrefFile.setValue( QString::fromAscii( "LogLevels/ConsoleLogLevel" ), p_uiConLevel );
        obPrefFile.setValue( QString::fromAscii( "LogLevels/DBLogLevel" ), p_uiDBLevel );
        obPrefFile.setValue( QString::fromAscii( "LogLevels/GUILogLevel" ), p_uiGUILevel );
    }
}
//====================================================================================
// cPreferences::getLogLevels
//------------------------------------------------------------------------------------
void cPreferences::getLogLevels( unsigned int *p_poConLevel,
                                 unsigned int *p_poDBLevel,
                                 unsigned int *p_poGUILevel ) const
{
    cSeverity::teSeverity  enConLevel = cSeverity::DEBUG;
    cSeverity::teSeverity  enDBLevel  = cSeverity::DEBUG;
    cSeverity::teSeverity  enGUILevel = cSeverity::DEBUG;
    g_obLogger.getMinSeverityLevels( &enConLevel, &enDBLevel, &enGUILevel );

    if( p_poConLevel ) *p_poConLevel = enConLevel;
    if( p_poDBLevel )  *p_poDBLevel  = enDBLevel;
    if( p_poGUILevel ) *p_poGUILevel = enGUILevel;
}
//====================================================================================
// cPreferences::setDBAccess
//------------------------------------------------------------------------------------
void cPreferences::setDBAccess( const QString &p_qsHost, const QString &p_qsDB,
                                const QString &p_qsUser, const QString &p_qsPwd )
{
    g_poDB->setHostName( p_qsHost );
    g_poDB->setDatabaseName( p_qsDB );
    g_poDB->setUserName( p_qsUser );
    g_poDB->setPassword( p_qsPwd );
}
//====================================================================================
// cPreferences::getDBAccess
//------------------------------------------------------------------------------------
void cPreferences::getDBAccess( QString *p_poHost, QString *p_poDB,
                                QString *p_poUser, QString *p_poPwd ) const
{
    if( p_poHost ) *p_poHost = g_poDB->getHostName();
    if( p_poDB )   *p_poDB   = g_poDB->getDatabaseName();
    if( p_poUser ) *p_poUser = g_poDB->getUserName();
    if( p_poPwd )  *p_poPwd  = g_poDB->getPassword();
}
//====================================================================================
// cPreferences::setMainWindowSize
//------------------------------------------------------------------------------------
void cPreferences::setMainWindowSize( const int p_nWindowWidth,
                                      const int p_nWindowHeight,
                                      bool p_boSaveNow )
{
    m_nWindowWidth = p_nWindowWidth;
    m_nWindowHeight = p_nWindowHeight;

    if( p_boSaveNow )
    {
        QSettings  obPrefFile( m_qsFileName, QSettings::IniFormat );
        obPrefFile.setValue( QString::fromAscii( "MainWindowWidth" ), p_nWindowWidth );
        obPrefFile.setValue( QString::fromAscii( "MainWindowHeight" ), p_nWindowHeight );
    }
}
//====================================================================================
// cPreferences::getMainWindowSize
//------------------------------------------------------------------------------------
void cPreferences::getMainWindowSize( int *p_nWindowWidth,
                                      int *p_nWindowHeight )
{
    *p_nWindowWidth = m_nWindowWidth;
    *p_nWindowHeight = m_nWindowHeight;
}
//====================================================================================
// cPreferences::setServerAddress
//------------------------------------------------------------------------------------
void cPreferences::setServerAddress( const QString &p_qsAddress, bool p_boSaveNow )
{
    m_qsAddress = p_qsAddress;

    if( p_boSaveNow )
    {
        QSettings  obPrefFile( m_qsFileName, QSettings::IniFormat );
        obPrefFile.setValue( QString::fromAscii( "Server/Address" ), m_qsAddress );
    }
}
//====================================================================================
// cPreferences::getServerAddress
//------------------------------------------------------------------------------------
QString cPreferences::getServerAddress() const
{
    return m_qsAddress;
}
//====================================================================================
// cPreferences::setServerPort
//------------------------------------------------------------------------------------
void cPreferences::setServerPort( const int p_nPort,
                                  bool p_boSaveNow )
{
    m_nPort = p_nPort;

    if( p_boSaveNow )
    {
        QSettings  obPrefFile( m_qsFileName, QSettings::IniFormat );
        obPrefFile.setValue( QString::fromAscii( "Server/Port" ), m_nPort );
    }
}
//====================================================================================
// cPreferences::getServerPort
//------------------------------------------------------------------------------------
int cPreferences::getServerPort() const
{
    return m_nPort;
}
//====================================================================================
// cPreferences::setMaxCorruptMessageCount
//------------------------------------------------------------------------------------
void cPreferences::setMaxCorruptMessageCount( const int p_nMaxCorruptMessageCount,
                                              bool p_boSaveNow )
{
    m_nMaxCorruptMessageCount = p_nMaxCorruptMessageCount;

    if( p_boSaveNow )
    {
        QSettings  obPrefFile( m_qsFileName, QSettings::IniFormat );
        obPrefFile.setValue( QString::fromAscii( "Connection/MaxCorruptedMesssage" ), m_nMaxCorruptMessageCount );
    }
}
//====================================================================================
// cPreferences::getMaxCorruptMessageCount
//------------------------------------------------------------------------------------
int cPreferences::getMaxCorruptMessageCount() const
{
    return m_nMaxCorruptMessageCount;
}
//====================================================================================
// cPreferences::loadConfFileSettings
//------------------------------------------------------------------------------------
void cPreferences::loadConfFileSettings()
{
    QSettings obPrefFile( m_qsFileName, QSettings::IniFormat );

    if( obPrefFile.status() != QSettings::NoError )
    {
        g_obLogger << cSeverity::WARNING;
        g_obLogger << "Failed to load preferences from file: " << m_qsFileName.toStdString();
        g_obLogger << cQTLogger::EOM;
    }
    else
    {
        m_qsLang                    = obPrefFile.value( QString::fromAscii( "Lang" ), "uk" ).toString();
        m_nWindowWidth              = obPrefFile.value( QString::fromAscii( "MainWindowWidth" ), 400 ).toInt();
        m_nWindowHeight             = obPrefFile.value( QString::fromAscii( "MainWindowHeight" ), 100 ).toInt();
        m_qsAddress                 = obPrefFile.value( QString::fromAscii( "Server/Address" ), "0.0.0.0" ).toString();
        m_nPort                     = obPrefFile.value( QString::fromAscii( "Server/Port" ), 4000 ).toInt();
        m_nMaxCorruptMessageCount   = obPrefFile.value( QString::fromAscii( "Connection/MaxCorruptedMesssage" ), 5 ).toInt();

        unsigned int uiConsoleLevel = obPrefFile.value( QString::fromAscii( "LogLevels/ConsoleLogLevel" ), cSeverity::DEBUG ).toUInt();
        if( (uiConsoleLevel >= cSeverity::MAX) ||
            (uiConsoleLevel <= cSeverity::MIN) )
        {
            uiConsoleLevel = cSeverity::DEBUG;

            g_obLogger << cSeverity::WARNING;
            g_obLogger << "Invalid ConsoleLogLevel in preferences file: " << m_qsFileName.toStdString();
            g_obLogger << cQTLogger::EOM;
        }

        unsigned int uiDBLevel = obPrefFile.value( QString::fromAscii( "LogLevels/DBLogLevel" ), cSeverity::DEBUG ).toUInt();
        if( (uiDBLevel >= cSeverity::MAX) &&
            (uiDBLevel <= cSeverity::MIN) )
        {
            uiDBLevel = cSeverity::DEBUG;

            g_obLogger << cSeverity::WARNING;
            g_obLogger << "Invalid DBLogLevel in preferences file: " << m_qsFileName.toStdString();
            g_obLogger << cQTLogger::EOM;
        }

        unsigned int uiGUILevel = obPrefFile.value( QString::fromAscii( "LogLevels/GUILogLevel" ), cSeverity::DEBUG ).toUInt();
        if( (uiGUILevel >= cSeverity::MAX) &&
            (uiGUILevel <= cSeverity::MIN) )
        {
            uiGUILevel = cSeverity::DEBUG;

            g_obLogger << cSeverity::WARNING;
            g_obLogger << "Invalid GUILogLevel in preferences file: " << m_qsFileName.toStdString();
            g_obLogger << cQTLogger::EOM;
        }

        setLogLevels( uiConsoleLevel, uiDBLevel, uiGUILevel );
    }
}
//====================================================================================
// cPreferences::loadDBSettings
//------------------------------------------------------------------------------------
void cPreferences::loadDBSettings()
{
/*    QSqlQuery *poQuery = g_poDB->executeQTQuery( QString( "SELECT value FROM settings WHERE settingId = %1" ).arg( SYS_MAX_DEVICE_COUNT_ID ) );
    if( poQuery->size() == 1 )
    {
        poQuery->first();
        m_uiSWMaxPanelCount = poQuery->value( 0 ).toInt();
    }*/
}
//====================================================================================
// cPreferences::save
//------------------------------------------------------------------------------------
void cPreferences::save() const
{
    QSettings  obPrefFile( m_qsFileName, QSettings::IniFormat );

    if( m_qsLang != "" )     obPrefFile.setValue( QString::fromAscii( "Lang" ), m_qsLang );
    obPrefFile.setValue( QString::fromAscii( "MainWindowWidth" ), m_nWindowWidth );
    obPrefFile.setValue( QString::fromAscii( "MainWindowHeight" ), m_nWindowHeight );
    if( m_qsAddress != "" ) obPrefFile.setValue( QString::fromAscii( "Server/Address" ), m_qsAddress );
    obPrefFile.setValue( QString::fromAscii( "Server/Port" ), m_nPort );
    obPrefFile.setValue( QString::fromAscii( "Connection/MaxCorruptedMesssage" ), m_nMaxCorruptMessageCount );

    unsigned int  uiConLevel, uiDBLevel, uiGUILevel;
    getLogLevels( &uiConLevel, &uiDBLevel, &uiGUILevel );
    obPrefFile.setValue( QString::fromAscii( "LogLevels/ConsoleLogLevel" ), uiConLevel );
    obPrefFile.setValue( QString::fromAscii( "LogLevels/DBLogLevel" ), uiDBLevel );
    obPrefFile.setValue( QString::fromAscii( "LogLevels/GUILogLevel" ), uiGUILevel );

//    string  stQuery = QString( "UPDATE settings SET value=\"%1\" WHERE settingId=%2" ).arg( m_uiSWMaxPanelCount ).arg( SYS_MAX_DEVICE_COUNT_ID ).toStdString();
//    g_obLogger << cSeverity::DEBUG << stQuery << cQTLogger::EOM;
//    g_poDB->executeQuery( stQuery );
}
