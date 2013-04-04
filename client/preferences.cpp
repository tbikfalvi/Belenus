//====================================================================================
//
// Belenus Kliens alkalmazas (c) Pagony Multimedia Studio Bt - 2010
//
//====================================================================================
//
// Filename    : preferences.cpp
// AppVersion  : 1.0
// FileVersion : 1.0
// Author      : Ballok Peter, Bikfalvi Tamas, Kovacs Gabor
//
//====================================================================================
// Alkalmazas beallitasok allomanya.
//====================================================================================

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
    processComponentID();
}

cPreferences::~cPreferences()
{
}

void cPreferences::init()
{
    m_qsFileName            = "";
    m_qsVersion             = "";
    m_qsLangFilePrefix      = "";
    m_qsLang                = "";
    m_qsLastUser            = "";
    m_uiPanelsPerRow        = 0;
    m_uiPanelCount          = 0;
    m_uiMainWindowLeft      = 0;
    m_uiMainWindowTop       = 0;
    m_uiMainWindowWidth     = 0;
    m_uiMainWindowHeight    = 0;
    m_uiLicenceId           = 0;
    m_qsServerAddress       = "";
    m_qsServerPort          = "";
    m_qsMainBackground      = "";
    m_inCommunicationPort   = 1;
    m_inBarcodeLength       = 12;
    m_qsBarcodePrefix       = "";

    m_bCassaAutoClose       = false;
    m_bCassaAutoWithdrawal  = false;

    m_nPatientCardLostPrice         = 0;
    m_nPatientCardLostPriceVat      = 0;
    m_nPatientCardPartnerPrice      = 0;
    m_nPatientCardPartnerPriceVat   = 0;
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

void cPreferences::setComponents( const unsigned int p_uiComponent, bool p_boSaveNow )
{
    m_uiComponent = p_uiComponent;

    if( p_boSaveNow )
    {
        QSettings  obPrefFile( m_qsFileName, QSettings::IniFormat );
        obPrefFile.setValue( QString::fromAscii( "PanelSystemID" ), m_uiComponent );
    }
}

unsigned int cPreferences::getComponents() const
{
    return m_uiComponent;
}

void cPreferences::processComponentID()
{
    m_bComponentSensolite   = ( m_uiComponent & CONST_COMPONENT_SENSOLITE ? true : false );
    m_bComponentKiwiSun     = ( m_uiComponent & CONST_COMPONENT_KIWISUN   ? true : false );
    m_bComponentDatabase    = ( m_uiComponent & CONST_COMPONENT_DATABASE  ? true : false );
    m_bComponentHardware    = ( m_uiComponent & CONST_COMPONENT_HARDWARE  ? true : false );
    m_bComponentInternet    = ( m_uiComponent & CONST_COMPONENT_INTERNET  ? true : false );
    m_bComponentClient      = ( m_uiComponent & CONST_COMPONENT_CLIENT    ? true : false );
    m_bComponentViewer      = ( m_uiComponent & CONST_COMPONENT_VIEWER    ? true : false );
}

bool cPreferences::isComponentSensoliteInstalled()
{
    return m_bComponentSensolite;
}

bool cPreferences::isComponentKiwiSunInstalled()
{
    return m_bComponentKiwiSun;
}

bool cPreferences::isComponentDatabaseInstalled()
{
    return m_bComponentDatabase;
}

bool cPreferences::isComponentHardwareInstalled()
{
    return m_bComponentHardware;
}

bool cPreferences::isComponentInternetInstalled()
{
    return m_bComponentInternet;
}

bool cPreferences::isComponentClientInstalled()
{
    return m_bComponentClient;
}

bool cPreferences::isComponentViewerInstalled()
{
    return m_bComponentViewer;
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

void cPreferences::setLicenceId( const int licenceId )
{
    m_uiLicenceId = licenceId;
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

void cPreferences::setBarcodeLengthDifferent( const bool p_bBarcodeLengthDifferent, bool p_boSaveNow )
{
    m_bBarcodeLengthDifferent = p_bBarcodeLengthDifferent;

    if( p_boSaveNow )
    {
        QSettings  obPrefFile( m_qsFileName, QSettings::IniFormat );
        obPrefFile.setValue( QString::fromAscii( "CardProductBarcodeLengthDifferent" ), m_bBarcodeLengthDifferent );
    }
}

bool cPreferences::isBarcodeLengthDifferent() const
{
    return m_bBarcodeLengthDifferent;
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

void cPreferences::setCurrencyDecimalSeparator( const QString &p_qsCurrencyDecimalSeparator, bool p_boSaveNow )
{
    m_qsCurrencyDecimalSeparator = p_qsCurrencyDecimalSeparator;

    if( p_boSaveNow )
    {
        QSettings  obPrefFile( m_qsFileName, QSettings::IniFormat );
        obPrefFile.setValue( QString::fromAscii( "Currency/Decimal" ), m_qsCurrencyDecimalSeparator );
    }
}

QString cPreferences::getCurrencyDecimalSeparator() const
{
    return m_qsCurrencyDecimalSeparator;
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

void cPreferences::setDeviceUseVAT( const int p_inVAT, bool p_boSaveNow )
{
    m_inDeviceUseVAT = p_inVAT;

    if( p_boSaveNow )
    {
        QSettings  obPrefFile( m_qsFileName, QSettings::IniFormat );
        obPrefFile.setValue( QString::fromAscii( "Device/VAT" ), m_inDeviceUseVAT );
    }
}

int cPreferences::getDeviceUseVAT() const
{
    return m_inDeviceUseVAT;
}

void cPreferences::setZipLength( const int p_inZip, bool p_boSaveNow )
{
    m_inZipLength = p_inZip;

    if( p_boSaveNow )
    {
        QSettings  obPrefFile( m_qsFileName, QSettings::IniFormat );
        obPrefFile.setValue( QString::fromAscii( "ZipLength" ), m_inZipLength );
    }
}

int cPreferences::getZipLength() const
{
    return m_inZipLength;
}


void cPreferences::setCassaAutoClose( const bool p_bCassaAutoClose, bool p_boSaveNow )
{
    m_bCassaAutoClose = p_bCassaAutoClose;

    if( p_boSaveNow )
    {
        QSettings  obPrefFile( m_qsFileName, QSettings::IniFormat );
        obPrefFile.setValue( QString::fromAscii( "CassaAutoClose" ), m_bCassaAutoClose );
    }
}

bool cPreferences::getCassaAutoClose() const
{
    return m_bCassaAutoClose;
}

void cPreferences::setCassaAutoWithdrawal( const bool p_bCassaAutoWithdrawal, bool p_boSaveNow )
{
    m_bCassaAutoWithdrawal = p_bCassaAutoWithdrawal;

    if( p_boSaveNow )
    {
        QSettings  obPrefFile( m_qsFileName, QSettings::IniFormat );
        obPrefFile.setValue( QString::fromAscii( "CassaAutoWithdrawal" ), m_bCassaAutoWithdrawal );
    }
}

bool cPreferences::getCassaAutoWithdrawal() const
{
    return m_bCassaAutoWithdrawal;
}

void cPreferences::setDefaultCountry( const QString &p_qsDefaultCountry, bool p_boSaveNow )
{
    m_qsDefaultCountry = p_qsDefaultCountry;

    if( p_boSaveNow )
    {
        QSettings  obPrefFile( m_qsFileName, QSettings::IniFormat );
        obPrefFile.setValue( QString::fromAscii( "DefaultCountry" ), m_qsDefaultCountry );
    }
}

QString cPreferences::getDefaultCountry() const
{
    return m_qsDefaultCountry;
}

void cPreferences::setDBAutoArchive( const bool p_bDBAutoArchive, bool p_boSaveNow )
{
    m_bDBAutoArchive = p_bDBAutoArchive;

    if( p_boSaveNow )
    {
        QSettings  obPrefFile( m_qsFileName, QSettings::IniFormat );
        obPrefFile.setValue( QString::fromAscii( "DBAutoSynchronization" ), m_bDBAutoArchive );
    }
}

bool cPreferences::getDBAutoArchive() const
{
    return m_bDBAutoArchive;
}
void cPreferences::setDBGlobalAutoSynchronize( const bool p_bDBGlobalAutoSynchronize, bool p_boSaveNow )
{
    m_bDBGlobalAutoSynchronize = p_bDBGlobalAutoSynchronize;

    if( p_boSaveNow )
    {
        QSettings  obPrefFile( m_qsFileName, QSettings::IniFormat );
        obPrefFile.setValue( QString::fromAscii( "DBGlobalAutoSynchronization" ), m_bDBGlobalAutoSynchronize );
    }
}

bool cPreferences::getDBGlobalAutoSynchronize() const
{
    return m_bDBGlobalAutoSynchronize;
}

void cPreferences::setSecondaryWindowVisibility( const bool p_bIsSecondaryWindowVisible, bool p_boSaveNow )
{
    m_bIsSecondaryWindowVisible = p_bIsSecondaryWindowVisible;

    if( p_boSaveNow )
    {
        QSettings  obPrefFile( m_qsFileName, QSettings::IniFormat );
        obPrefFile.setValue( QString::fromAscii( "IsSecondaryWindowVisible" ), m_bIsSecondaryWindowVisible );
    }
}

bool cPreferences::isSecondaryWindowVisible() const
{
    return m_bIsSecondaryWindowVisible;
}

void cPreferences::setSecondaryWindowPosition( const QPoint &p_qpPosition, bool p_boSaveNow )
{
    m_qpSecondaryPosition = p_qpPosition;

    if( p_boSaveNow )
    {
        QSettings  obPrefFile( m_qsFileName, QSettings::IniFormat );
        obPrefFile.setValue( QString::fromAscii( "SecondaryWindow/Left" ), m_qpSecondaryPosition.x() );
        obPrefFile.setValue( QString::fromAscii( "SecondaryWindow/Top" ), m_qpSecondaryPosition.y() );
    }
}

QPoint cPreferences::secondaryWindowPosition() const
{
    return m_qpSecondaryPosition;
}

void cPreferences::setSecondaryWindowSize( const QSize &p_qsSize, bool p_boSaveNow )
{
    m_qsSecondarySize = p_qsSize;

    if( p_boSaveNow )
    {
        QSettings  obPrefFile( m_qsFileName, QSettings::IniFormat );
        obPrefFile.setValue( QString::fromAscii( "SecondaryWindow/Width" ), m_qsSecondarySize.width() );
        obPrefFile.setValue( QString::fromAscii( "SecondaryWindow/Height" ), m_qsSecondarySize.height() );
    }
}

QSize cPreferences::secondaryWindowSize() const
{
    return m_qsSecondarySize;
}

void cPreferences::setSecondaryBackground( const QString &p_qsColor, bool p_boSaveNow )
{
    m_qsSecondaryBackground = p_qsColor;

    if( p_boSaveNow )
    {
        QSettings  obPrefFile( m_qsFileName, QSettings::IniFormat );
        obPrefFile.setValue( QString::fromAscii( "SecondaryWindow/Background" ), m_qsSecondaryBackground );
    }
}

QString cPreferences::getSecondaryBackground() const
{
    return m_qsSecondaryBackground;
}

void cPreferences::setPatientCardLostPrice( const int p_inPrice )
{
    m_nPatientCardLostPrice = p_inPrice;
}

int cPreferences::getPatientCardLostPrice() const
{
    return m_nPatientCardLostPrice;
}

void cPreferences::setPatientCardLostPriceVat(const int p_inVat )
{
    m_nPatientCardLostPriceVat = p_inVat;
}

int cPreferences::getPatientCardLostPriceVat() const
{
    return m_nPatientCardLostPriceVat;
}

void cPreferences::setPatientCardPartnerPrice( const int p_inPrice )
{
    m_nPatientCardPartnerPrice = p_inPrice;
}

int cPreferences::getPatientCardPartnerPrice() const
{
    return m_nPatientCardPartnerPrice;
}

void cPreferences::setPatientCardPartnerPriceVat(const int p_inVat )
{
    m_nPatientCardPartnerPriceVat = p_inVat;
}

int cPreferences::getPatientCardPartnerPriceVat() const
{
    return m_nPatientCardPartnerPriceVat;
}

void cPreferences::setLogLevels( const unsigned int p_uiConLevel,
                                 const unsigned int p_uiDBLevel,
                                 const unsigned int p_uiGUILevel,
                                 const unsigned int p_uiFileLevel,
                                 bool p_boSaveNow )
{
    g_obLogger.setMinimumSeverity("console", (cSeverity::teSeverity)p_uiConLevel );
    g_obLogger.setMinimumSeverity("db", (cSeverity::teSeverity)p_uiDBLevel );
    g_obLogger.setMinimumSeverity("gui", (cSeverity::teSeverity)p_uiGUILevel );
    g_obLogger.setMinimumSeverity("file", (cSeverity::teSeverity)p_uiFileLevel );

    if( p_boSaveNow )
    {
        QSettings  obPrefFile( m_qsFileName, QSettings::IniFormat );
        obPrefFile.setValue( QString::fromAscii( "LogLevels/ConsoleLogLevel" ), p_uiConLevel );
        obPrefFile.setValue( QString::fromAscii( "LogLevels/DBLogLevel" ), p_uiDBLevel );
        obPrefFile.setValue( QString::fromAscii( "LogLevels/GUILogLevel" ), p_uiGUILevel );
        obPrefFile.setValue( QString::fromAscii( "LogLevels/FileLogLevel" ), p_uiFileLevel );
    }
}

void cPreferences::getLogLevels( unsigned int *p_poConLevel,
                                 unsigned int *p_poDBLevel,
                                 unsigned int *p_poGUILevel,
                                 unsigned int *p_poFileLevel) const
{    
    if( p_poConLevel )  *p_poConLevel   = g_obLogger.getMinimumSeverity( "console" );
    if( p_poDBLevel )   *p_poDBLevel    = g_obLogger.getMinimumSeverity( "db" );
    if( p_poGUILevel )  *p_poGUILevel   = g_obLogger.getMinimumSeverity( "gui" );
    if( p_poFileLevel ) *p_poFileLevel  = g_obLogger.getMinimumSeverity( "file" );
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
        g_obLogger(cSeverity::WARNING) << "Failed to load preferences from file: " << m_qsFileName << EOM;
    }
    else
    {
        m_qsLang                    = obPrefFile.value( QString::fromAscii( "Lang" ), "uk" ).toString();
        m_qsLastUser                = obPrefFile.value( QString::fromAscii( "LastUser" ), "" ).toString();
        m_uiPanelsPerRow            = obPrefFile.value( QString::fromAscii( "PanelsPerRow" ), 1 ).toUInt();
        m_inBarcodeLength           = obPrefFile.value( QString::fromAscii( "BarcodeLength" ), "1" ).toInt();
        m_qsBarcodePrefix           = obPrefFile.value( QString::fromAscii( "BarcodePrefix" ), "" ).toString();
        m_bBarcodeLengthDifferent = obPrefFile.value( QString::fromAscii( "CardProductBarcodeLengthDifferent" ), true ).toBool();
        m_bCassaAutoClose           = obPrefFile.value( QString::fromAscii( "CassaAutoClose" ), false ).toBool();
        m_bCassaAutoWithdrawal      = obPrefFile.value( QString::fromAscii( "CassaAutoWithdrawal" ), false ).toBool();
        m_qsDefaultCountry          = obPrefFile.value( QString::fromAscii( "DefaultCountry" ), "" ).toString();
        m_inZipLength               = obPrefFile.value( QString::fromAscii( "ZipLength" ), 4 ).toInt();
        m_bDBAutoArchive            = obPrefFile.value( QString::fromAscii( "DBAutoSynchronization" ), false ).toBool();
        m_bDBGlobalAutoSynchronize  = obPrefFile.value( QString::fromAscii( "DBGlobalAutoSynchronization" ), false ).toBool();
        m_uiComponent               = obPrefFile.value( QString::fromAscii( "PanelSystemID" ), 0 ).toUInt();
        m_bIsSecondaryWindowVisible = obPrefFile.value( QString::fromAscii( "IsSecondaryWindowVisible" ), false ).toBool();
        int nLeft                   = obPrefFile.value( QString::fromAscii( "SecondaryWindow/Left" ), "10" ).toInt();
        int nTop                    = obPrefFile.value( QString::fromAscii( "SecondaryWindow/Top" ), "10" ).toInt();
        int nWidth                  = obPrefFile.value( QString::fromAscii( "SecondaryWindow/Width" ), "600" ).toInt();
        int nHeight                 = obPrefFile.value( QString::fromAscii( "SecondaryWindow/Height" ), "400" ).toInt();
        m_qsSecondaryBackground     = obPrefFile.value( QString::fromAscii( "SecondaryWindow/Background" ), "#000000"  ).toString();

        m_qpSecondaryPosition = QPoint( nLeft, nTop );
        m_qsSecondarySize = QSize( nWidth, nHeight );

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
        m_qsCurrencyDecimalSeparator   = obPrefFile.value( QString::fromAscii( "Currency/Decimal" ), "," ).toString();

        m_uiMaxTreatLength    = obPrefFile.value( QString::fromAscii( "Device/MaxTreatLength" ), 100 ).toUInt();
        m_inDeviceUseVAT      = obPrefFile.value( QString::fromAscii( "Device/VAT" ), 25 ).toInt();

        m_nPatientCardLostPrice     = obPrefFile.value( QString::fromAscii( "PatientCard/PriceLost" ), 0 ).toUInt();
        m_nPatientCardLostPriceVat  = obPrefFile.value( QString::fromAscii( "PatientCard/PriceLostVat" ), 0 ).toUInt();
        m_nPatientCardPartnerPrice     = obPrefFile.value( QString::fromAscii( "PatientCard/PricePartner" ), 0 ).toUInt();
        m_nPatientCardPartnerPriceVat  = obPrefFile.value( QString::fromAscii( "PatientCard/PricePartnerVat" ), 0 ).toUInt();

        unsigned int uiConsoleLevel = obPrefFile.value( QString::fromAscii( "LogLevels/ConsoleLogLevel" ), cSeverity::WARNING ).toUInt();
        if( (uiConsoleLevel >= cSeverity::MAX) ||
            (uiConsoleLevel <= cSeverity::MIN) )
        {
            uiConsoleLevel = cSeverity::DEBUG;
            g_obLogger(cSeverity::WARNING) << "Invalid ConsoleLogLevel in preferences file: " << m_qsFileName << EOM;
        }

        unsigned int uiDBLevel = obPrefFile.value( QString::fromAscii( "LogLevels/DBLogLevel" ), cSeverity::INFO ).toUInt();
        if( (uiDBLevel >= cSeverity::MAX) &&
            (uiDBLevel <= cSeverity::MIN) )
        {
            uiDBLevel = cSeverity::DEBUG;

            g_obLogger(cSeverity::WARNING) << "Invalid DBLogLevel in preferences file: " << m_qsFileName << EOM;
        }

        unsigned int uiGUILevel = obPrefFile.value( QString::fromAscii( "LogLevels/GUILogLevel" ), cSeverity::WARNING ).toUInt();
        if( (uiGUILevel >= cSeverity::MAX) &&
            (uiGUILevel <= cSeverity::MIN) )
        {
            uiGUILevel = cSeverity::DEBUG;

            g_obLogger(cSeverity::WARNING) << "Invalid GUILogLevel in preferences file: " << m_qsFileName << EOM;
        }

        unsigned int uiFileLevel = obPrefFile.value( QString::fromAscii( "LogLevels/FileLogLevel" ), cSeverity::WARNING ).toUInt();
        if( (uiFileLevel >= cSeverity::MAX) &&
            (uiFileLevel <= cSeverity::MIN) )
        {
            uiFileLevel = cSeverity::DEBUG;

            g_obLogger(cSeverity::WARNING) << "Invalid FileLogLevel in preferences file: " << m_qsFileName << EOM;
        }

        setLogLevels( uiConsoleLevel, uiDBLevel, uiGUILevel, uiFileLevel );
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
    }
    catch( cSevException &e )
    {
        if( poQuery ) delete poQuery;
        g_obLogger(e.severity()) << e.what() << EOM;
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
    obPrefFile.setValue( QString::fromAscii( "CardProductBarcodeLengthDifferent" ), m_bBarcodeLengthDifferent );
    obPrefFile.setValue( QString::fromAscii( "CassaAutoClose" ), m_bCassaAutoClose );
    obPrefFile.setValue( QString::fromAscii( "CassaAutoWithdrawal" ), m_bCassaAutoWithdrawal );
    obPrefFile.setValue( QString::fromAscii( "DefaultCountry" ), m_qsDefaultCountry );
    obPrefFile.setValue( QString::fromAscii( "ZipLength" ), m_inZipLength );
    obPrefFile.setValue( QString::fromAscii( "DBAutoSynchronization" ), m_bDBAutoArchive );
    obPrefFile.setValue( QString::fromAscii( "DBGlobalAutoSynchronization" ), m_bDBGlobalAutoSynchronize );
    obPrefFile.setValue( QString::fromAscii( "IsSecondaryWindowVisible" ), m_bIsSecondaryWindowVisible );

    obPrefFile.setValue( QString::fromAscii( "SecondaryWindow/Left" ), m_qpSecondaryPosition.x() );
    obPrefFile.setValue( QString::fromAscii( "SecondaryWindow/Top" ), m_qpSecondaryPosition.y() );
    obPrefFile.setValue( QString::fromAscii( "SecondaryWindow/Width" ), m_qsSecondarySize.width() );
    obPrefFile.setValue( QString::fromAscii( "SecondaryWindow/Height" ), m_qsSecondarySize.height() );
    obPrefFile.setValue( QString::fromAscii( "SecondaryWindow/Background" ), m_qsSecondaryBackground );

    obPrefFile.setValue( QString::fromAscii( "PatientCard/PriceLost" ), m_nPatientCardLostPrice );
    obPrefFile.setValue( QString::fromAscii( "PatientCard/PriceLostVat" ), m_nPatientCardLostPriceVat );
    obPrefFile.setValue( QString::fromAscii( "PatientCard/PricePartner" ), m_nPatientCardPartnerPrice );
    obPrefFile.setValue( QString::fromAscii( "PatientCard/PricePartnerVat" ), m_nPatientCardPartnerPriceVat );

    unsigned int  uiConLevel, uiDBLevel, uiGUILevel, uiFileLevel;
    getLogLevels( &uiConLevel, &uiDBLevel, &uiGUILevel, &uiFileLevel );
    obPrefFile.setValue( QString::fromAscii( "LogLevels/ConsoleLogLevel" ), uiConLevel );
    obPrefFile.setValue( QString::fromAscii( "LogLevels/DBLogLevel" ), uiDBLevel );
    obPrefFile.setValue( QString::fromAscii( "LogLevels/GUILogLevel" ), uiGUILevel );
    obPrefFile.setValue( QString::fromAscii( "LogLevels/FileLogLevel" ), uiFileLevel );

    obPrefFile.setValue( QString::fromAscii( "Server/Address" ), m_qsServerAddress );
    obPrefFile.setValue( QString::fromAscii( "Server/Port" ), m_qsServerPort );

    obPrefFile.setValue( QString::fromAscii( "Hardware/ComPort" ), m_inCommunicationPort );

    obPrefFile.setValue( QString::fromAscii( "UserInterface/MainBackground" ), m_qsMainBackground );

    obPrefFile.setValue( QString::fromAscii( "Currency/Short" ), m_qsCurrencyShort );
    obPrefFile.setValue( QString::fromAscii( "Currency/Long" ), m_qsCurrencyLong );
    obPrefFile.setValue( QString::fromAscii( "Currency/Separator" ), m_qsCurrencySeparator );
    obPrefFile.setValue( QString::fromAscii( "Currency/Decimal" ), m_qsCurrencyDecimalSeparator );

    obPrefFile.setValue( QString::fromAscii( "Device/MaxTreatLength" ), m_uiMaxTreatLength );
    obPrefFile.setValue( QString::fromAscii( "Device/VAT" ), m_inDeviceUseVAT );
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

void cPreferences::setDialogSize( const QString &p_qsDialogName, const QPoint &p_qpDlgSize )
{
    QSettings  obPrefFile( m_qsFileName, QSettings::IniFormat );

    obPrefFile.setValue( QString::fromAscii( "Dialogs/%1_width" ).arg(p_qsDialogName), p_qpDlgSize.x() );
    obPrefFile.setValue( QString::fromAscii( "Dialogs/%1_height" ).arg(p_qsDialogName), p_qpDlgSize.y() );
}

QPoint cPreferences::getDialogSize( const QString &p_qsDialogName, const QPoint &p_qpDlgSizeDefault ) const
{
    QSettings   obPrefFile( m_qsFileName, QSettings::IniFormat );
    QPoint      qpDlgSize = p_qpDlgSizeDefault;

    if( obPrefFile.status() != QSettings::NoError )
    {
        g_obLogger(cSeverity::WARNING) << "Failed to load preferences from file: " << m_qsFileName << EOM;
    }
    else
    {
        qpDlgSize.setX( obPrefFile.value( QString::fromAscii( "Dialogs/%1_width" ).arg(p_qsDialogName), QString::number(p_qpDlgSizeDefault.x()) ).toInt() );
        qpDlgSize.setY( obPrefFile.value( QString::fromAscii( "Dialogs/%1_height" ).arg(p_qsDialogName), QString::number(p_qpDlgSizeDefault.y()) ).toInt() );
    }

    return qpDlgSize;
}

