#include "preferences.h"
#include "belenus.h"

cPreferences::cPreferences()
{
    init();
}

cPreferences::cPreferences( const QString &p_qsFileName )
{
    init();
    setFileName( p_qsFileName );
    loadConfFileSettings();
}

cPreferences::~cPreferences()
{
}

void cPreferences::init()
{
    m_qsFileName          = "";
    m_qsVersion           = "";
    m_qsLangFilePrefix    = "";
    m_qsLang              = "";
    m_qsLastUser          = "";
    m_uiPanelsPerRow      = 0;
    m_uiPanelCount        = 0;
    m_uiMainWindowLeft    = 0;
    m_uiMainWindowTop     = 0;
    m_uiMainWindowWidth   = 0;
    m_uiMainWindowHeight  = 0;
    m_uiLicenceId         = 0;
    m_qsClientSerial      = "";
    m_qsServerAddress     = "";
    m_qsServerPort        = "";
    m_qsMainBackground    = "";
    m_inCommunicationPort = 1;
    m_inBarcodeLength     = 12;
    m_qsBarcodePrefix     = "";
}

void cPreferences::setFileName( const QString &p_qsFileName )
{
    m_qsFileName = p_qsFileName;
}

QString cPreferences::getFileName() const
{
    return m_qsFileName;
}

void cPreferences::setVersion( const QString &p_qsVersion, bool p_boSaveNow )
{
    m_qsVersion = p_qsVersion;

    if( p_boSaveNow )
    {
        QSettings  obPrefFile( m_qsFileName, QSettings::IniFormat );
        obPrefFile.setValue( QString::fromAscii( "Version" ), m_qsVersion );
    }
}

QString cPreferences::getVersion() const
{
    return m_qsVersion;
}

void cPreferences::setLangFilePrefix( const QString &p_qsPrefix )
{
    m_qsLangFilePrefix = p_qsPrefix;
}

QString cPreferences::getLangFilePrefix() const
{
    return m_qsLangFilePrefix;
}

void cPreferences::setLang( const QString &p_qsLang, bool p_boSaveNow )
{
    m_qsLang = p_qsLang;

    if( p_boSaveNow )
    {
        QSettings  obPrefFile( m_qsFileName, QSettings::IniFormat );
        obPrefFile.setValue( QString::fromAscii( "Lang" ), m_qsLang );
    }
}

QString cPreferences::getLang() const
{
    return m_qsLang;
}

void cPreferences::setLastUser( const QString &p_qsLastUser, bool p_boSaveNow )
{
    m_qsLastUser = p_qsLastUser;

    if( p_boSaveNow )
    {
        QSettings  obPrefFile( m_qsFileName, QSettings::IniFormat );
        obPrefFile.setValue( QString::fromAscii( "LastUser" ), m_qsLastUser );
    }
}

QString cPreferences::getLastUser() const
{
    return m_qsLastUser;
}

void cPreferences::setPanelsPerRow( const unsigned int p_uiPanelsPerRow,
                                    bool p_boSaveNow )
{
    m_uiPanelsPerRow = p_uiPanelsPerRow;

    if( p_boSaveNow )
    {
        QSettings  obPrefFile( m_qsFileName, QSettings::IniFormat );
        obPrefFile.setValue( QString::fromAscii( "PanelsPerRow" ), m_uiPanelsPerRow );
    }
}

unsigned int cPreferences::getPanelsPerRow() const
{
    return m_uiPanelsPerRow;
}

unsigned int cPreferences::getPanelCount() const
{
    return m_uiPanelCount;
}

void cPreferences::setMainWindowSizePos( const unsigned int p_uiMainWindowLeft,
                                         const unsigned int p_uiMainWindowTop,
                                         const unsigned int p_uiMainWindowWidth,
                                         const unsigned int p_uiMainWindowHeight,
                                         bool p_boSaveNow )
{
    m_uiMainWindowLeft   = p_uiMainWindowLeft;
    m_uiMainWindowTop    = p_uiMainWindowTop;
    m_uiMainWindowWidth  = p_uiMainWindowWidth;
    m_uiMainWindowHeight = p_uiMainWindowHeight;

    if( p_boSaveNow )
    {
        QSettings  obPrefFile( m_qsFileName, QSettings::IniFormat );
        obPrefFile.setValue( QString::fromAscii( "UserInterface/MainWindowLeft" ), m_uiMainWindowLeft );
        obPrefFile.setValue( QString::fromAscii( "UserInterface/MainWindowTop" ), m_uiMainWindowTop );
        obPrefFile.setValue( QString::fromAscii( "UserInterface/MainWindowWidth" ), m_uiMainWindowWidth );
        obPrefFile.setValue( QString::fromAscii( "UserInterface/MainWindowHeight" ), m_uiMainWindowHeight );
    }
}

unsigned int cPreferences::getMainWindowLeft() const
{
    return m_uiMainWindowLeft;
}

unsigned int cPreferences::getMainWindowTop() const
{
    return m_uiMainWindowTop;
}

unsigned int cPreferences::getMainWindowWidth() const
{
    return m_uiMainWindowWidth;
}

unsigned int cPreferences::getMainWindowHeight() const
{
    return m_uiMainWindowHeight;
}

void cPreferences::setMainBackground( const QString &p_qsColor, bool p_boSaveNow )
{
    m_qsMainBackground = p_qsColor;

    if( p_boSaveNow )
    {
        QSettings  obPrefFile( m_qsFileName, QSettings::IniFormat );
        obPrefFile.setValue( QString::fromAscii( "UserInterface/MainBackground" ), m_qsMainBackground );
    }
}

QString cPreferences::getMainBackground() const
{
    return m_qsMainBackground;
}

unsigned int cPreferences::getLicenceId() const
{
    return m_uiLicenceId;
}

QString cPreferences::getClientSerial() const
{
    return m_qsClientSerial;
}

void cPreferences::setServerAddress( const QString &p_qsServerAddress, bool p_boSaveNow )
{
    m_qsServerAddress = p_qsServerAddress;

    if( p_boSaveNow )
    {
        QSettings  obPrefFile( m_qsFileName, QSettings::IniFormat );
        obPrefFile.setValue( QString::fromAscii( "Server/Address" ), m_qsServerAddress );
    }
}

QString cPreferences::getServerAddress() const
{
    return m_qsServerAddress;
}

void cPreferences::setServerPort( const QString &p_qsServerPort, bool p_boSaveNow )
{
    m_qsServerPort = p_qsServerPort;

    if( p_boSaveNow )
    {
        QSettings  obPrefFile( m_qsFileName, QSettings::IniFormat );
        obPrefFile.setValue( QString::fromAscii( "Server/Port" ), m_qsServerPort );
    }
}

QString cPreferences::getServerPort() const
{
    return m_qsServerPort;
}

void cPreferences::setCommunicationPort( const int p_inPortNumber, bool p_boSaveNow )
{
    m_inCommunicationPort = p_inPortNumber;

    if( p_boSaveNow )
    {
        QSettings  obPrefFile( m_qsFileName, QSettings::IniFormat );
        obPrefFile.setValue( QString::fromAscii( "Hardware/ComPort" ), m_inCommunicationPort );
    }
}

int cPreferences::getCommunicationPort() const
{
    return m_inCommunicationPort;
}

void cPreferences::setBarcodeLength( const int p_inBarcodeLength, bool p_boSaveNow )
{
    m_inBarcodeLength = p_inBarcodeLength;

    if( p_boSaveNow )
    {
        QSettings  obPrefFile( m_qsFileName, QSettings::IniFormat );
        obPrefFile.setValue( QString::fromAscii( "BarcodeLength" ), m_inBarcodeLength );
    }
}

int cPreferences::getBarcodeLength() const
{
    return m_inBarcodeLength;
}

void cPreferences::setBarcodePrefix( const QString &p_qsPrefix, bool p_boSaveNow )
{
    m_qsBarcodePrefix = p_qsPrefix;
    m_qsBarcodePrefix.truncate( m_inBarcodeLength - 1 );

    if( p_boSaveNow )
    {
        QSettings  obPrefFile( m_qsFileName, QSettings::IniFormat );
        obPrefFile.setValue( QString::fromAscii( "BarcodePrefix" ), m_qsBarcodePrefix );
    }
}

QString cPreferences::getBarcodePrefix() const
{
    return m_qsBarcodePrefix;
}

void cPreferences::setCurrencyShort( const QString &p_qsCurrencyShort, bool p_boSaveNow )
{
    m_qsCurrencyShort = p_qsCurrencyShort;

    if( p_boSaveNow )
    {
        QSettings  obPrefFile( m_qsFileName, QSettings::IniFormat );
        obPrefFile.setValue( QString::fromAscii( "Currency/Short" ), m_qsCurrencyShort );
    }
}

QString cPreferences::getCurrencyShort() const
{
    return m_qsCurrencyShort;
}

void cPreferences::setCurrencyLong( const QString &p_qsCurrencyLong, bool p_boSaveNow )
{
    m_qsCurrencyLong = p_qsCurrencyLong;

    if( p_boSaveNow )
    {
        QSettings  obPrefFile( m_qsFileName, QSettings::IniFormat );
        obPrefFile.setValue( QString::fromAscii( "Currency/Long" ), m_qsCurrencyLong );
    }
}

QString cPreferences::getCurrencyLong() const
{
    return m_qsCurrencyLong;
}

void cPreferences::setCurrencySeparator( const QString &p_qsCurrencySeparator, bool p_boSaveNow )
{
    m_qsCurrencySeparator = p_qsCurrencySeparator;

    if( p_boSaveNow )
    {
        QSettings  obPrefFile( m_qsFileName, QSettings::IniFormat );
        obPrefFile.setValue( QString::fromAscii( "Currency/Separator" ), m_qsCurrencySeparator );
    }
}

QString cPreferences::getCurrencySeparator() const
{
    return m_qsCurrencySeparator;
}

void cPreferences::setMaxTreatLength( const unsigned int p_uiMaxTreatLength, bool p_boSaveNow )
{
    m_uiMaxTreatLength = p_uiMaxTreatLength;

    if( p_boSaveNow )
    {
        QSettings  obPrefFile( m_qsFileName, QSettings::IniFormat );
        obPrefFile.setValue( QString::fromAscii( "Device/MaxTreatLength" ), m_uiMaxTreatLength );
    }
}

unsigned int cPreferences::getMaxTreatLength() const
{
    return m_uiMaxTreatLength;
}

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

void cPreferences::setDBAccess( const QString &p_qsHost, const QString &p_qsDB,
                                const QString &p_qsUser, const QString &p_qsPwd )
{
    g_poDB->setHostName( p_qsHost );
    g_poDB->setDatabaseName( p_qsDB );
    g_poDB->setUserName( p_qsUser );
    g_poDB->setPassword( p_qsPwd );
}

void cPreferences::getDBAccess( QString *p_poHost, QString *p_poDB,
                                QString *p_poUser, QString *p_poPwd ) const
{
    if( p_poHost ) *p_poHost = g_poDB->getHostName();
    if( p_poDB )   *p_poDB   = g_poDB->getDatabaseName();
    if( p_poUser ) *p_poUser = g_poDB->getUserName();
    if( p_poPwd )  *p_poPwd  = g_poDB->getPassword();
}

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
        m_qsLang              = obPrefFile.value( QString::fromAscii( "Lang" ), "uk" ).toString();
        m_qsLastUser          = obPrefFile.value( QString::fromAscii( "LastUser" ), "" ).toString();
        m_uiPanelsPerRow      = obPrefFile.value( QString::fromAscii( "PanelsPerRow" ), 1 ).toUInt();
        m_inBarcodeLength     = obPrefFile.value( QString::fromAscii( "BarcodeLength" ), "1" ).toInt();
        m_qsBarcodePrefix     = obPrefFile.value( QString::fromAscii( "BarcodePrefix" ), "" ).toString();
        bool boIsANumber = false;
        m_qsBarcodePrefix.toInt( &boIsANumber );
        if( !boIsANumber ) m_qsBarcodePrefix = "";
        m_qsBarcodePrefix.truncate( m_inBarcodeLength - 1 );  //Make sure there is at least one free character to be filled in a Barcode

        m_uiMainWindowLeft    = obPrefFile.value( QString::fromAscii( "UserInterface/MainWindowLeft" ),   0    ).toUInt();
        m_uiMainWindowTop     = obPrefFile.value( QString::fromAscii( "UserInterface/MainWindowTop" ),    0    ).toUInt();
        m_uiMainWindowWidth   = obPrefFile.value( QString::fromAscii( "UserInterface/MainWindowWidth" ),  1024 ).toUInt();
        m_uiMainWindowHeight  = obPrefFile.value( QString::fromAscii( "UserInterface/MainWindowHeight" ), 768  ).toUInt();
        m_qsMainBackground    = obPrefFile.value( QString::fromAscii( "UserInterface/MainBackground" ), "#000000"  ).toString();

        m_qsServerAddress     = obPrefFile.value( QString::fromAscii( "Server/Address" ), "0.0.0.0" ).toString();
        m_qsServerPort        = obPrefFile.value( QString::fromAscii( "Server/Port" ), "1000" ).toString();

        m_inCommunicationPort = obPrefFile.value( QString::fromAscii( "Hardware/ComPort" ), "1" ).toInt();

        m_qsCurrencyShort     = obPrefFile.value( QString::fromAscii( "Currency/Short" ), "Ft." ).toString();
        m_qsCurrencyLong      = obPrefFile.value( QString::fromAscii( "Currency/Long" ), "Forint" ).toString();
        m_qsCurrencySeparator = obPrefFile.value( QString::fromAscii( "Currency/Separator" ), "," ).toString();

        m_uiMaxTreatLength    = obPrefFile.value( QString::fromAscii( "Device/MaxTreatLength" ), 100 ).toUInt();

        unsigned int uiConsoleLevel = obPrefFile.value( QString::fromAscii( "LogLevels/ConsoleLogLevel" ), cSeverity::WARNING ).toUInt();
        if( (uiConsoleLevel >= cSeverity::MAX) ||
            (uiConsoleLevel <= cSeverity::MIN) )
        {
            uiConsoleLevel = cSeverity::DEBUG;

            g_obLogger << cSeverity::WARNING;
            g_obLogger << "Invalid ConsoleLogLevel in preferences file: " << m_qsFileName.toStdString();
            g_obLogger << cQTLogger::EOM;
        }

        unsigned int uiDBLevel = obPrefFile.value( QString::fromAscii( "LogLevels/DBLogLevel" ), cSeverity::INFO ).toUInt();
        if( (uiDBLevel >= cSeverity::MAX) &&
            (uiDBLevel <= cSeverity::MIN) )
        {
            uiDBLevel = cSeverity::DEBUG;

            g_obLogger << cSeverity::WARNING;
            g_obLogger << "Invalid DBLogLevel in preferences file: " << m_qsFileName.toStdString();
            g_obLogger << cQTLogger::EOM;
        }

        unsigned int uiGUILevel = obPrefFile.value( QString::fromAscii( "LogLevels/GUILogLevel" ), cSeverity::WARNING ).toUInt();
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

void cPreferences::loadDBSettings() throw (cSevException)
{
    QSqlQuery *poQuery = NULL;
    try
    {
        poQuery = g_poDB->executeQTQuery( QString( "SELECT COUNT(*) AS panelCount FROM panels WHERE active=1" ) );
        poQuery->first();
        m_uiPanelCount = poQuery->value( 0 ).toInt();

        delete poQuery;
        poQuery = NULL;

        poQuery = g_poDB->executeQTQuery( QString( "SELECT licenceId, serial FROM licences WHERE active=1 ORDER BY licenceId DESC" ) );
        if( poQuery->first() )
        {
            m_uiLicenceId    = poQuery->value( 0 ).toInt();
            m_qsClientSerial = poQuery->value( 1 ).toString();
        }
    }
    catch( cSevException &e )
    {
        if( poQuery ) delete poQuery;

        g_obLogger << e.severity();
        g_obLogger << e.what();
        g_obLogger << cQTLogger::EOM;
    }

}

void cPreferences::save() const throw (cSevException)
{
    QSettings  obPrefFile( m_qsFileName, QSettings::IniFormat );

    if( m_qsLang != "" )     obPrefFile.setValue( QString::fromAscii( "Lang" ), m_qsLang );
    if( m_qsLastUser != "" ) obPrefFile.setValue( QString::fromAscii( "LastUser" ), m_qsLastUser );
    obPrefFile.setValue( QString::fromAscii( "PanelsPerRow" ), m_uiPanelsPerRow );
    obPrefFile.setValue( QString::fromAscii( "BarcodeLength" ), m_inBarcodeLength );
    obPrefFile.setValue( QString::fromAscii( "BarcodePrefix" ), m_qsBarcodePrefix );

    unsigned int  uiConLevel, uiDBLevel, uiGUILevel;
    getLogLevels( &uiConLevel, &uiDBLevel, &uiGUILevel );
    obPrefFile.setValue( QString::fromAscii( "LogLevels/ConsoleLogLevel" ), uiConLevel );
    obPrefFile.setValue( QString::fromAscii( "LogLevels/DBLogLevel" ), uiDBLevel );
    obPrefFile.setValue( QString::fromAscii( "LogLevels/GUILogLevel" ), uiGUILevel );

    obPrefFile.setValue( QString::fromAscii( "Server/Address" ), m_qsServerAddress );
    obPrefFile.setValue( QString::fromAscii( "Server/Port" ), m_qsServerPort );

    obPrefFile.setValue( QString::fromAscii( "Hardware/ComPort" ), m_inCommunicationPort );

    obPrefFile.setValue( QString::fromAscii( "UserInterface/MainBackground" ), m_qsMainBackground );

    obPrefFile.setValue( QString::fromAscii( "Currency/Short" ), m_qsCurrencyShort );
    obPrefFile.setValue( QString::fromAscii( "Currency/Long" ), m_qsCurrencyLong );
    obPrefFile.setValue( QString::fromAscii( "Currency/Separator" ), m_qsCurrencySeparator );

    obPrefFile.setValue( QString::fromAscii( "Device/MaxTreatLength" ), m_uiMaxTreatLength );
}

unsigned int cPreferences::postponedPatients() const
{
    return m_uiPostponedPatients;
}

void cPreferences::setPostponedPatients( const unsigned int p_uiPostponedPatients )
{
    m_uiPostponedPatients = p_uiPostponedPatients;
}

unsigned int cPreferences::postponedAttendances() const
{
    return m_uiPostponedAttendances;
}

void cPreferences::setPostponedAttendances( const unsigned int p_uiPostponedAttendances )
{
    m_uiPostponedAttendances = p_uiPostponedAttendances;
}
