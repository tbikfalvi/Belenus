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

#include <QMessageBox>

#include "preferences.h"
#include "belenus.h"

cPreferences::cPreferences()
{
    init();
}

cPreferences::~cPreferences()
{
}

void cPreferences::init()
{
    m_qsVersion             = "";
    m_qsVersionDb           = "";
    m_qsApplicationPath     = "";
    m_qsLangFilePrefix      = "";
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
    m_bCassaAutoCreateNew   = false;
    m_inCassaCreateType     = 0;

    m_nPatientCardLostPrice         = 0;
    m_nPatientCardLostPriceVat      = 0;
    m_nPatientCardPartnerPrice      = 0;
    m_nPatientCardPartnerPriceVat   = 0;

    m_qsDirDbBinaries               = "";
    m_qsDirDbBackup                 = "";
    m_bForceBackupDatabase          = false;
    m_bBackupDatabase               = false;
    m_nBackupDatabaseType           = 0;
    m_qsBackupDatabaseDays          = "";

    m_qsDateFormat                  = "yyyy-MM-dd";

    m_bFapados                      = false;
    m_nFapados                      = 0;

    m_bIsStopInLine                 = true;

    m_qsPanelTextSteril             = "";
    m_qsPanelTextTubeReplace        = "";
    m_qsPanelTextTubeCleanup        = "";

    m_bEnableHWDebug                = false;

    m_bBlnsHttpEnabled              = false;
    m_bWebSyncAutoStart             = false;

    m_bIsTextSterilVisible          = false;
    m_bIsTextTubeReplaceVisible     = false;
    m_bIsTextTubeCleanupVisible     = false;

    m_bBarcodeHidden                = false;

    m_qsLicenceLastValidated        = "";

    m_bAutoMailOnPCSell             = false;
    m_bAutoMailOnPCUse              = false;
    m_bAutoMailOnPCExpiration       = false;
    m_bAutoMailOnPCUnitChange       = false;

    m_bShowPatientInfoOnStart       = false;
    m_nShowInfoOnWindow             = 1;
    m_nCloseInfoWindowAfterSecs     = 5;

    m_bIsRFIDEnabled                = false;
    m_nRFIDComPort                  = 1;

    m_nLicenceCheck                 = 56;
    m_nLicenceCheckCounter          = 56;
    m_nWorkTime                     = 336;
    m_nWorkTimeCounter              = 336;

    m_bCardyGoSync                  = false;

    m_bAllowDeleteObsoleteUnits     = false;
    m_nObsolateUnitsDays            = 30;

    m_bLogFilesDeleteAllowed        = false;
    m_nDeleteLogFilesMonths         = 3;

    m_bSaveAdhocGuest               = true;
}

void cPreferences::loadSettings() throw (cSevException)
{
    QSqlQuery *poQuery = NULL;

    try
    {
        m_nLicenceCheck                 = loadSettingI( "LICENCE_CHECK", 56 );
        m_nLicenceCheckCounter          = loadSettingI( "LICENCE_CHECK_COUNTER", 56 );
        m_nWorkTime                     = loadSettingI( "LICENCE_WORKTIME", 336 );
        m_nWorkTimeCounter              = loadSettingI( "LICENCE_WORKTIME_COUNTER", 336 );
        m_qsLastUser                    = loadSettingS( "GEN_LastUser", "" );
        m_uiPanelsPerRow                = loadSettingU( "GEN_PanelsPerRow", 3 );
        m_inBarcodeLength               = loadSettingI( "GEN_BarcodeLength", 8 );
        m_qsBarcodePrefix               = loadSettingS( "GEN_BarcodePrefix", "" );
        m_bBarcodeLengthDifferent       = loadSettingB( "GEN_CardProductBarcodeLengthDifferent", true );
        m_bCassaAutoClose               = loadSettingB( "GEN_CassaAutoClose", true );
        m_bCassaAutoWithdrawal          = loadSettingB( "GEN_CassaAutoWithdrawal", true );
        m_bCassaAutoCreateNew           = loadSettingB( "GEN_CassaAutoCreate", true );
        m_inCassaCreateType             = loadSettingI( "GEN_CassaCreateType", 4 );
        m_qsDefaultCountry              = loadSettingS( "GEN_DefaultCountry", "" );
        m_inZipLength                   = loadSettingI( "GEN_ZipLength", 4 );
        m_bDBAutoArchive                = loadSettingB( "GEN_DBAutoSynchronization", false );
        m_bDBGlobalAutoSynchronize      = loadSettingB( "GEN_DBGlobalAutoSynchronization", false );
        m_uiComponent                   = loadSettingU( "GEN_PanelSystemID", 0 );
        m_bIsSecondaryWindowVisible     = loadSettingB( "GEN_IsSecondaryWindowVisible", false );
        m_nSecondsWaitOnSlpashScreen    = loadSettingI( "GEN_SecondsWaitOnSlpashScreen", 3 );

        m_bAllowDeleteObsoleteUnits     = loadSettingB( "DB_AllowDeleteObsoleteUnits", false );
        m_nObsolateUnitsDays            = loadSettingI( "DB_ObsolateUnitsDays", 30 );

        m_bLogFilesDeleteAllowed        = loadSettingB( "FILE_AllowDeleteLogFiles", false );
        m_nDeleteLogFilesMonths         = loadSettingI( "FILE_DeleteLogFilesMonts", 3 );

        int nLeft                       = loadSettingI( "EXTWIN_Left", 10 );
        int nTop                        = loadSettingI( "EXTWIN_Top", 10 );
        int nWidth                      = loadSettingI( "EXTWIN_Width", 600 );
        int nHeight                     = loadSettingI( "EXTWIN_Height", 400 );
        m_qsSecondaryBackground         = loadSettingS( "EXTWIN_Background", "#000000" );
        m_qsSecondaryFrame              = loadSettingS( "EXTWIN_FrameColor", "#555555" );
        m_bIsSecondaryCaptionVisible    = loadSettingB( "EXTWIN_IsSecondaryCaptionVisible", true );
        m_bIsTextTubeReplaceVisible     = loadSettingB( "EXTWIN_IsTextTubeReplaceVisible", true );
        m_bIsTextSterilVisible          = loadSettingB( "EXTWIN_IsTextSterilVisible", true );
        m_bIsTextTubeCleanupVisible     = loadSettingB( "EXTWIN_IsTextTubeCleanupVisible", false );

        m_qsActiveCaptionBackground     = loadSettingS( "PANEL_ActiveCaptionBackground", "#000099" );
        m_qsActiveCaptionColor          = loadSettingS( "PANEL_ActiveCaptionColor", "#FFFFFF" );
        m_qsInactiveCaptionBackground   = loadSettingS( "PANEL_InactiveCaptionBackground", "#000022" );
        m_qsInactiveCaptionColor        = loadSettingS( "PANEL_InactiveCaptionColor", "#FFFFFF" );
        m_qsSecondaryCaptionBackground  = loadSettingS( "PANEL_SecondaryCaptionBackground", "#000099" );
        m_qsSecondaryCaptionColor       = loadSettingS( "PANEL_SecondaryCaptionColor", "#FFFFFF" );
        m_bIsStopInLine                 = loadSettingB( "PANEL_IsStopInLine", true );
        m_qsPanelTextSteril             = loadSettingS( "PANEL_TextSterile", QObject::tr( " NOT STERILE " ) );
        m_qsPanelTextTubeReplace        = loadSettingS( "PANEL_TextTubeReplace", QObject::tr( " TUBE REPLACEMENT NEEDED " ) );
        m_qsPanelTextTubeCleanup        = loadSettingS( "PANEL_TextTubeCleanup", QObject::tr( " TUBE CLEAN-UP NEEDED " ) );

        m_uiMainWindowLeft              = loadSettingU( "UI_MainWindowLeft", 0 );
        m_uiMainWindowTop               = loadSettingU( "UI_MainWindowTop", 0 );
        m_uiMainWindowWidth             = loadSettingU( "UI_MainWindowWidth", 1024 );
        m_uiMainWindowHeight            = loadSettingU( "UI_MainWindowHeight" , 768 );
        m_qsMainBackground              = loadSettingS( "UI_MainBackground", "#000000" );

        m_qsServerAddress               = loadSettingS( "SERVER_Address", "http://www.kiwisun.hu" );
        m_qsServerPort                  = loadSettingS( "SERVER_Port", "1000" );

        m_inCommunicationPort           = loadSettingI( "HW_ComPort", 1 );
        m_bForceModuleSendTime          = loadSettingB( "HW_ForceModuleSendTime", false );
        m_bForceModuleCheckButton       = loadSettingB( "HW_ForceModuleCheckButton", false );
        m_nForceTimeSendCounter         = loadSettingB( "HW_ForceTimeSendCounter", 0);
        m_bIsRFIDEnabled                = loadSettingB( "HW_RFIDEnabled", false );
        m_nRFIDComPort                  = loadSettingI( "HW_RFIDComPort", 1);

        m_qsCurrencyShort               = loadSettingS( "CURR_Short", "Ft." );
        m_qsCurrencyLong                = loadSettingS( "CURR_Long", "Forint" );
        m_qsCurrencySeparator           = loadSettingS( "CURR_Separator", "," );
        m_qsCurrencyDecimalSeparator    = loadSettingS( "CURR_Decimal", "." );

        m_uiMaxTreatLength              = loadSettingU( "DEVICE_MaxTreatLength", 100 );
        m_inDeviceUseVAT                = loadSettingI( "DEVICE_VAT", 25 );

        m_nPatientCardLostPrice         = loadSettingI( "CARD_PriceLost", 500 );
        m_nPatientCardLostPriceVat      = loadSettingI( "CARD_PriceLostVat", 0 );
        m_nPatientCardPartnerPrice      = loadSettingI( "CARD_PricePartner", 0 );
        m_nPatientCardPartnerPriceVat   = loadSettingI( "CARD_PricePartnerVat", 0 );
        m_bBarcodeHidden                = loadSettingB( "CARD_Hidden", false );
        m_bShowPatientInfoOnStart       = loadSettingB( "CARD_ShowPatientLastVisitInfo", false );
        m_nShowInfoOnWindow             = loadSettingI( "CARD_ShowInfoOnWindow", 1 );
        m_nCloseInfoWindowAfterSecs     = loadSettingI( "CARD_CloseInfoWindowAfterSecs", 5 );

        unsigned int uiFileLevel        = loadSettingU( "LOG_FileLogLevel", cSeverity::WARNING );

        m_bBlnsHttpEnabled              = loadSettingB( "SYNC_Enabled", false );
        m_bWebSyncAutoStart             = loadSettingB( "SYNC_WebSyncAutoStart", false );

        m_qsDirDbBinaries               = loadSettingS( "BACKUP_DirDbBinaries", "C:/wamp/bin/mysql/mysql5.5.24/bin" );
        m_qsDirDbBackup                 = loadSettingS( "BACKUP_DirDbBackup", "" );
        m_bBackupDatabase               = loadSettingB( "BACKUP_BackupDb", false );
        m_nBackupDatabaseType           = loadSettingI( "BACKUP_DbBackupType", 0 );
        m_qsBackupDatabaseDays          = loadSettingS( "BACKUP_DbBackupDays", "" );

        m_qsDateFormat                  = loadSettingS( "GEN_DateFormat", "yyyy-MM-dd" );

        m_bFapados                      = loadSettingB( "GEN_Component", false );

        m_bUsageVisibleOnMain           = loadSettingB( "PANEL_UsageVisibleOnMain", true );

        m_bAutoMailOnPCSell             = loadSettingB( "AUTOMAIL_OnSell", false );
        m_bAutoMailOnPCUse              = loadSettingB( "AUTOMAIL_OnUse", false );
        m_bAutoMailOnPCExpiration       = loadSettingB( "AUTOMAIL_OnExpiration", false );
        m_nPCExpirationDays             = loadSettingI( "AUTOMAIL_ExpirationDays", 7 );
        m_bAutoMailOnPCUnitChange       = loadSettingB( "AUTOMAIL_OnUnitChange", false );
        m_bCardyGoSync                  = loadSettingB( "AUTOMAIL_OnCardyGo", false );

        m_bSaveAdhocGuest               = loadSettingB( "GEN_SaveAdhocGuest", true );

        m_qpSecondaryPosition = QPoint( nLeft, nTop );
        m_qsSecondarySize = QSize( nWidth, nHeight );

        bool boIsANumber = false;
        m_qsBarcodePrefix.toInt( &boIsANumber );
        if( !boIsANumber ) m_qsBarcodePrefix = "";
        m_qsBarcodePrefix.truncate( m_inBarcodeLength - 1 );  //Make sure there is at least one free character to be filled in a Barcode

        if( (uiFileLevel >= cSeverity::MAX) &&
            (uiFileLevel <= cSeverity::MIN) )
        {
            uiFileLevel = cSeverity::DEBUG;

            g_obLogger(cSeverity::WARNING) << "Invalid FileLogLevel in setting" << EOM;
        }
        setLogLevel( uiFileLevel );

        delete poQuery;
    }
    catch( cSevException &e )
    {
        if( poQuery ) delete poQuery;
        g_obLogger(e.severity()) << e.what() << EOM;
        g_obGen.showTrayError( e.what() );
    }
}

void cPreferences::saveSettings() throw (cSevException)
{
    QSqlQuery *poQuery = NULL;

    try
    {
        if( m_qsLang != "" )        g_obLanguage.saveCurrentLanguage( m_qsLang );
        if( m_qsLastUser != "" )    saveSettingS( "GEN_LastUser", m_qsLastUser );

        saveSettingI( "LICENCE_CHECK", m_nLicenceCheck );
        saveSettingI( "LICENCE_CHECK_COUNTER", m_nLicenceCheckCounter );
        saveSettingI( "LICENCE_WORKTIME", m_nWorkTime );
        saveSettingI( "LICENCE_WORKTIME_COUNTER", m_nWorkTimeCounter );

        saveSettingU( "GEN_PanelsPerRow", m_uiPanelsPerRow );
        saveSettingI( "GEN_BarcodeLength", m_inBarcodeLength );
        saveSettingS( "GEN_BarcodePrefix", m_qsBarcodePrefix );
        saveSettingB( "GEN_CardProductBarcodeLengthDifferent", m_bBarcodeLengthDifferent );
        saveSettingB( "GEN_CassaAutoClose", m_bCassaAutoClose );
        saveSettingB( "GEN_CassaAutoWithdrawal", m_bCassaAutoWithdrawal );
        saveSettingB( "GEN_CassaAutoCreate", m_bCassaAutoCreateNew );
        saveSettingI( "GEN_CassaCreateType", m_inCassaCreateType );
        saveSettingS( "GEN_DefaultCountry", m_qsDefaultCountry );
        saveSettingI( "GEN_ZipLength", m_inZipLength );
        saveSettingB( "GEN_DBAutoSynchronization", m_bDBAutoArchive );
        saveSettingB( "GEN_DBGlobalAutoSynchronization", m_bDBGlobalAutoSynchronize );
        saveSettingB( "GEN_IsSecondaryWindowVisible", m_bIsSecondaryWindowVisible );
        saveSettingI( "GEN_SecondsWaitOnSlpashScreen", m_nSecondsWaitOnSlpashScreen );

        saveSettingB( "DB_AllowDeleteObsoleteUnits", m_bAllowDeleteObsoleteUnits );
        saveSettingI( "DB_ObsolateUnitsDays", m_nObsolateUnitsDays );

        saveSettingB( "FILE_AllowDeleteLogFiles", m_bLogFilesDeleteAllowed );
        saveSettingI( "FILE_DeleteLogFilesMonts", m_nDeleteLogFilesMonths );

        saveSettingI( "EXTWIN_Left", m_qpSecondaryPosition.x() );
        saveSettingI( "EXTWIN_Top", m_qpSecondaryPosition.y() );
        saveSettingI( "EXTWIN_Width", m_qsSecondarySize.width() );
        saveSettingI( "EXTWIN_Height", m_qsSecondarySize.height() );
        saveSettingS( "EXTWIN_Background", m_qsSecondaryBackground );
        saveSettingB( "EXTWIN_IsTextTubeReplaceVisible", m_bIsTextTubeReplaceVisible );
        saveSettingB( "EXTWIN_IsTextSterilVisible", m_bIsTextSterilVisible );
        saveSettingB( "EXTWIN_IsTextTubeCleanupVisible", m_bIsTextTubeCleanupVisible );

        saveSettingS( "PANEL_ActiveCaptionBackground", m_qsActiveCaptionBackground );
        saveSettingS( "PANEL_ActiveCaptionColor", m_qsActiveCaptionColor );
        saveSettingS( "PANEL_InactiveCaptionBackground", m_qsInactiveCaptionBackground );
        saveSettingS( "PANEL_InactiveCaptionColor", m_qsInactiveCaptionColor );
        saveSettingS( "PANEL_SecondaryCaptionBackground", m_qsSecondaryCaptionBackground );
        saveSettingS( "PANEL_SecondaryCaptionColor", m_qsSecondaryCaptionColor );
        saveSettingB( "PANEL_IsStopInLine", m_bIsStopInLine );
        saveSettingS( "PANEL_TextSterile", m_qsPanelTextSteril );
        saveSettingS( "PANEL_TextTubeReplace", m_qsPanelTextTubeReplace );
        saveSettingS( "PANEL_TextTubeCleanup", m_qsPanelTextTubeCleanup );

        saveSettingS( "EXTWIN_FrameColor", m_qsSecondaryFrame );
        saveSettingB( "EXTWIN_IsSecondaryCaptionVisible", m_bIsSecondaryCaptionVisible );

        saveSettingI( "CARD_PriceLost", m_nPatientCardLostPrice );
        saveSettingI( "CARD_PriceLostVat", m_nPatientCardLostPriceVat );
        saveSettingI( "CARD_PricePartner", m_nPatientCardPartnerPrice );
        saveSettingI( "CARD_PricePartnerVat", m_nPatientCardPartnerPriceVat );
        saveSettingB( "CARD_Hidden", m_bBarcodeHidden );
        saveSettingB( "CARD_ShowPatientLastVisitInfo", m_bShowPatientInfoOnStart );
        saveSettingI( "CARD_ShowInfoOnWindow", m_nShowInfoOnWindow );
        saveSettingI( "CARD_CloseInfoWindowAfterSecs", m_nCloseInfoWindowAfterSecs );

        saveSettingU( "LOG_FileLogLevel", getLogLevel() );

        saveSettingS( "SERVER_Address", m_qsServerAddress );
        saveSettingS( "SERVER_Port", m_qsServerPort );

        saveSettingI( "HW_ComPort", m_inCommunicationPort );
        saveSettingB( "HW_ForceModuleSendTime", m_bForceModuleSendTime );
        saveSettingB( "HW_ForceModuleCheckButton", m_bForceModuleCheckButton );
        saveSettingB( "HW_ForceTimeSendCounter", m_nForceTimeSendCounter );
        saveSettingB( "HW_RFIDEnabled", m_bIsRFIDEnabled );
        saveSettingI( "HW_RFIDComPort", m_nRFIDComPort );

        saveSettingS( "UI_MainBackground", m_qsMainBackground );

        saveSettingS( "CURR_Short", m_qsCurrencyShort );
        saveSettingS( "CURR_Long", m_qsCurrencyLong );
        saveSettingS( "CURR_Separator", m_qsCurrencySeparator );
        saveSettingS( "CURR_Decimal", m_qsCurrencyDecimalSeparator );

        saveSettingU( "DEVICE_MaxTreatLength", m_uiMaxTreatLength );
        saveSettingI( "DEVICE_VAT", m_inDeviceUseVAT );

        saveSettingB( "SYNC_Enabled", m_bBlnsHttpEnabled );
        saveSettingB( "SYNC_WebSyncAutoStart", m_bWebSyncAutoStart );

        saveSettingS( "BACKUP_DirDbBinaries", m_qsDirDbBinaries );
        saveSettingS( "BACKUP_DirDbBackup", m_qsDirDbBackup );
        saveSettingB( "BACKUP_BackupDb", m_bBackupDatabase );
        saveSettingI( "BACKUP_DbBackupType", m_nBackupDatabaseType );
        saveSettingS( "BACKUP_DbBackupDays", m_qsBackupDatabaseDays );

        saveSettingS( "GEN_DateFormat", m_qsDateFormat );

        saveSettingB( "GEN_Component", m_bFapados );

        saveSettingB( "PANEL_UsageVisibleOnMain", m_bUsageVisibleOnMain );

        if( m_bFapados )
        {
            saveSettingI( "COMPONENT_ID", 66 );
        }
        else
        {
            saveSettingI( "COMPONENT_ID", 42 );
        }

        saveSettingB( "AUTOMAIL_OnSell", m_bAutoMailOnPCSell );
        saveSettingB( "AUTOMAIL_OnUse", m_bAutoMailOnPCUse );
        saveSettingB( "AUTOMAIL_OnExpiration", m_bAutoMailOnPCExpiration );
        saveSettingI( "AUTOMAIL_ExpirationDays", m_nPCExpirationDays );
        saveSettingB( "AUTOMAIL_OnUnitChange", m_bAutoMailOnPCUnitChange );
        saveSettingB( "AUTOMAIL_OnCardyGo", m_bCardyGoSync );

        saveSettingB( "GEN_SaveAdhocGuest", m_bSaveAdhocGuest );

        delete poQuery;
    }
    catch( cSevException &e )
    {
        if( poQuery ) delete poQuery;
        g_obLogger(e.severity()) << e.what() << EOM;
        g_obGen.showTrayError( e.what() );
    }
}

unsigned int cPreferences::loadSettingU( QString p_Identifier, unsigned int p_Default ) throw (cSevException)
{
    unsigned int value = 0;
    QSqlQuery *poQuery = NULL;

    try
    {
        poQuery = g_poDB->executeQTQuery( QString( "SELECT value FROM settings WHERE identifier=\"%1\" " ).arg( p_Identifier ) );
        if( poQuery->first() )
        {
            value = poQuery->value( 0 ).toUInt();
        }
        else
        {
            g_poDB->executeQTQuery( QString("INSERT INTO `settings` (`settingId`, `identifier`, `value`) VALUES (NULL, '%1', '%2') " )
                                            .arg( p_Identifier )
                                            .arg( p_Default ) );
            value = p_Default;
        }
        delete poQuery;
    }
    catch( cSevException &e )
    {
        if( poQuery ) delete poQuery;
        g_obLogger(e.severity()) << e.what() << EOM;
        g_obGen.showTrayError( e.what() );
    }

    return value;
}

QString cPreferences::loadSettingS( QString p_Identifier, QString p_Default ) throw (cSevException)
{
    QString value = "";
    QSqlQuery *poQuery = NULL;

    try
    {
        poQuery = g_poDB->executeQTQuery( QString( "SELECT value FROM settings WHERE identifier=\"%1\" " ).arg( p_Identifier ) );
        if( poQuery->first() )
        {
            value = poQuery->value( 0 ).toString();
        }
        else
        {
            g_poDB->executeQTQuery( QString("INSERT INTO `settings` (`settingId`, `identifier`, `value`) VALUES (NULL, '%1', '%2') " )
                                            .arg( p_Identifier )
                                            .arg( p_Default ) );
            value = p_Default;
        }
        delete poQuery;
    }
    catch( cSevException &e )
    {
        if( poQuery ) delete poQuery;
        g_obLogger(e.severity()) << e.what() << EOM;
        g_obGen.showTrayError( e.what() );
    }

    return value;
}

bool cPreferences::loadSettingB( QString p_Identifier, bool p_Default ) throw (cSevException)
{
    bool value = 0;
    QSqlQuery *poQuery = NULL;

    try
    {
        poQuery = g_poDB->executeQTQuery( QString( "SELECT value FROM settings WHERE identifier=\"%1\" " ).arg( p_Identifier ) );
        if( poQuery->first() )
        {
            value = poQuery->value( 0 ).toBool();
        }
        else
        {
            g_poDB->executeQTQuery( QString("INSERT INTO `settings` (`settingId`, `identifier`, `value`) VALUES (NULL, '%1', '%2') " )
                                            .arg( p_Identifier )
                                            .arg( p_Default ) );
            value = p_Default;
        }
        delete poQuery;
    }
    catch( cSevException &e )
    {
        if( poQuery ) delete poQuery;
        g_obLogger(e.severity()) << e.what() << EOM;
        g_obGen.showTrayError( e.what() );
    }

    return value;
}

int cPreferences::loadSettingI( QString p_Identifier, int p_Default ) throw (cSevException)
{
    int value = 0;
    QSqlQuery *poQuery = NULL;

    try
    {
        poQuery = g_poDB->executeQTQuery( QString( "SELECT value FROM settings WHERE identifier=\"%1\" " ).arg( p_Identifier ) );
        if( poQuery->first() )
        {
            value = poQuery->value( 0 ).toInt();
        }
        else
        {
            g_poDB->executeQTQuery( QString("INSERT INTO `settings` (`settingId`, `identifier`, `value`) VALUES (NULL, '%1', '%2') " )
                                            .arg( p_Identifier )
                                            .arg( p_Default ) );
            value = p_Default;
        }
        delete poQuery;
    }
    catch( cSevException &e )
    {
        if( poQuery ) delete poQuery;
        g_obLogger(e.severity()) << e.what() << EOM;
        g_obGen.showTrayError( e.what() );
    }

    return value;
}

void cPreferences::saveSettingU( QString p_Identifier, unsigned int p_Value ) throw (cSevException)
{
    try
    {
        g_poDB->executeQTQuery( QString( "UPDATE settings SET value='%1' WHERE identifier=\"%2\" " ).arg( p_Value ).arg( p_Identifier ) );
    }
    catch( cSevException &e )
    {
        g_obLogger(e.severity()) << e.what() << EOM;
        g_obGen.showTrayError( e.what() );
    }
}

void cPreferences::saveSettingS( QString p_Identifier, QString p_Value ) throw (cSevException)
{
    try
    {
        g_poDB->executeQTQuery( QString( "UPDATE settings SET value=\"%1\" WHERE identifier=\"%2\" " ).arg( p_Value ).arg( p_Identifier ) );
    }
    catch( cSevException &e )
    {
        g_obLogger(e.severity()) << e.what() << EOM;
        g_obGen.showTrayError( e.what() );
    }
}

void cPreferences::saveSettingB( QString p_Identifier, bool p_Value ) throw (cSevException)
{
    try
    {
        g_poDB->executeQTQuery( QString( "UPDATE settings SET value='%1' WHERE identifier=\"%2\" " ).arg( p_Value ).arg( p_Identifier ) );
    }
    catch( cSevException &e )
    {
        g_obLogger(e.severity()) << e.what() << EOM;
        g_obGen.showTrayError( e.what() );
    }
}

void cPreferences::saveSettingI( QString p_Identifier, int p_Value ) throw (cSevException)
{
    try
    {
        g_poDB->executeQTQuery( QString( "UPDATE settings SET value='%1' WHERE identifier=\"%2\" " ).arg( p_Value ).arg( p_Identifier ) );
    }
    catch( cSevException &e )
    {
        g_obLogger(e.severity()) << e.what() << EOM;
        g_obGen.showTrayError( e.what() );
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

        m_qslPanelIds.clear();

        poQuery = g_poDB->executeQTQuery( QString( "SELECT * FROM panels WHERE active=1" ) );
        while( poQuery->next() )
        {
            m_qslPanelIds << poQuery->value(0).toString();
        }

        poQuery = g_poDB->executeQTQuery( QString( "SELECT value FROM settings WHERE identifier=\"COMPONENT_ID\" " ) );
        if( poQuery->first() )
        {
            m_nFapados = poQuery->value( 0 ).toInt();
        }

        if( m_nFapados == 0 )
        {
            g_poDB->executeQTQuery( QString( "INSERT INTO settings ( settingId, identifier, value ) VALUES ( NULL , 'COMPONENT_ID', '13' ) " ) );
        }
        else
        {
            if( (m_bFapados && m_nFapados != 66) || (!m_bFapados && m_nFapados != 42 && m_nFapados != 13) )
            {
                g_poDB->executeQTQuery( QString( "UPDATE settings SET value='66' WHERE identifier=\"COMPONENT_ID\" " ) );
            }
        }

        poQuery = g_poDB->executeQTQuery( QString( "SELECT value FROM settings WHERE identifier=\"LICENCE_LAST_VALIDATED\" " ) );
        if( poQuery->first() )
        {
            m_qsLicenceLastValidated = poQuery->value( 0 ).toString();
        }

        if( m_qsLicenceLastValidated.length() == 0 )
        {
            g_poDB->executeQTQuery( QString( "INSERT INTO settings ( settingId, identifier, value ) VALUES ( NULL , 'LICENCE_LAST_VALIDATED', '2000-01-01 12:00:00' ) " ) );
            m_qsLicenceLastValidated = "2000-01-01 12:00:00";
        }

        poQuery = g_poDB->executeQTQuery( QString( "SELECT value FROM settings WHERE identifier=\"DA_WORKTIME\" " ) );
        if( poQuery->size() == 0 )
        {
            g_poDB->executeQTQuery( QString( "INSERT INTO settings ( settingId, identifier, value ) VALUES ( NULL , 'DA_WORKTIME', '1' ) " ) );
            m_bDACanModifyWorktime = true;
        }
        else if( poQuery->first() )
        {
            m_bDACanModifyWorktime = poQuery->value( 0 ).toBool();
        }

        poQuery = g_poDB->executeQTQuery( QString( "SELECT value FROM settings WHERE identifier=\"DA_EXPDATE\" " ) );
        if( poQuery->size() == 0 )
        {
            g_poDB->executeQTQuery( QString( "INSERT INTO settings ( settingId, identifier, value ) VALUES ( NULL , 'DA_EXPDATE', '0' ) " ) );
            m_bDACanModifyExpDate = false;
        }
        else if( poQuery->first() )
        {
            m_bDACanModifyExpDate = poQuery->value( 0 ).toBool();
        }

        delete poQuery;
    }
    catch( cSevException &e )
    {
        if( poQuery ) delete poQuery;
        g_obLogger(e.severity()) << e.what() << EOM;
        g_obGen.showTrayError( e.what() );
    }

}

void cPreferences::setApplicationPath( const QString &p_qsPath )
{
    m_qsApplicationPath = p_qsPath;
}

QString cPreferences::getApplicationPath() const
{
    return m_qsApplicationPath;
}

void cPreferences::setVersion( const QString &p_qsVersion )
{
    m_qsVersion = p_qsVersion;
}

QString cPreferences::getVersion() const
{
    return m_qsVersion;
}

void cPreferences::setVersionDb( const QString &p_qsVersion )
{
    m_qsVersionDb = p_qsVersion;
}

QString cPreferences::getVersionDb() const
{
    return m_qsVersionDb;
}

void cPreferences::setComponents( const unsigned int p_uiComponent )
{
    m_uiComponent = p_uiComponent;
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

void cPreferences::setLang( const QString &p_qsLang )
{
    m_qsLang = p_qsLang;
}

QString cPreferences::getLang() const
{
    return m_qsLang;
}

void cPreferences::setLastUser( const QString &p_qsLastUser )
{
    m_qsLastUser = p_qsLastUser;
    saveSettingS( "GEN_LastUser", m_qsLastUser );
}

QString cPreferences::getLastUser() const
{
    return m_qsLastUser;
}

void cPreferences::setPanelsPerRow( const unsigned int p_uiPanelsPerRow )
{
    m_uiPanelsPerRow = p_uiPanelsPerRow;
}

unsigned int cPreferences::getPanelsPerRow() const
{
    return m_uiPanelsPerRow;
}

unsigned int cPreferences::getPanelCount() const
{
    return m_uiPanelCount;
}

unsigned int cPreferences::getPanelId(int p_nPanelIterator)
{
    if( p_nPanelIterator > -1 && p_nPanelIterator < m_qslPanelIds.count() )
        return m_qslPanelIds.at( p_nPanelIterator ).toInt();
    else
        return 0;
}

void cPreferences::setMainWindowSizePos( const unsigned int p_uiMainWindowLeft,
                                         const unsigned int p_uiMainWindowTop,
                                         const unsigned int p_uiMainWindowWidth,
                                         const unsigned int p_uiMainWindowHeight )
{
    m_uiMainWindowLeft   = p_uiMainWindowLeft;
    m_uiMainWindowTop    = p_uiMainWindowTop;
    m_uiMainWindowWidth  = p_uiMainWindowWidth;
    m_uiMainWindowHeight = p_uiMainWindowHeight;

    saveSettingU( "UI_MainWindowLeft", m_uiMainWindowLeft );
    saveSettingU( "UI_MainWindowTop", m_uiMainWindowTop );
    saveSettingU( "UI_MainWindowWidth", m_uiMainWindowWidth );
    saveSettingU( "UI_MainWindowHeight", m_uiMainWindowHeight );
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

void cPreferences::setMainBackground( const QString &p_qsColor )
{
    m_qsMainBackground = p_qsColor;
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

void cPreferences::setServerAddress( const QString &p_qsServerAddress )
{
    m_qsServerAddress = p_qsServerAddress;
}

QString cPreferences::getServerAddress() const
{
    return m_qsServerAddress;
}

void cPreferences::setServerPort( const QString &p_qsServerPort )
{
    m_qsServerPort = p_qsServerPort;
}

QString cPreferences::getServerPort() const
{
    return m_qsServerPort;
}

void cPreferences::setCommunicationPort( const int p_inPortNumber )
{
    m_inCommunicationPort = p_inPortNumber;
}

int cPreferences::getCommunicationPort() const
{
    return m_inCommunicationPort;
}

void cPreferences::setBarcodeLength( const int p_inBarcodeLength )
{
    m_inBarcodeLength = p_inBarcodeLength;
    saveSettingI( "GEN_BarcodeLength", m_inBarcodeLength );
}

int cPreferences::getBarcodeLength() const
{
    return m_inBarcodeLength;
}

void cPreferences::setBarcodePrefix( const QString &p_qsPrefix )
{
    m_qsBarcodePrefix = p_qsPrefix;
    m_qsBarcodePrefix.truncate( m_inBarcodeLength - 1 );
}

QString cPreferences::getBarcodePrefix() const
{
    return m_qsBarcodePrefix;
}

void cPreferences::setBarcodeLengthDifferent( const bool p_bBarcodeLengthDifferent )
{
    m_bBarcodeLengthDifferent = p_bBarcodeLengthDifferent;
}

bool cPreferences::isBarcodeLengthDifferent() const
{
    return m_bBarcodeLengthDifferent;
}

void cPreferences::setCurrencyShort( const QString &p_qsCurrencyShort )
{
    m_qsCurrencyShort = p_qsCurrencyShort;
}

QString cPreferences::getCurrencyShort() const
{
    return m_qsCurrencyShort;
}

void cPreferences::setCurrencyLong( const QString &p_qsCurrencyLong )
{
    m_qsCurrencyLong = p_qsCurrencyLong;
}

QString cPreferences::getCurrencyLong() const
{
    return m_qsCurrencyLong;
}

void cPreferences::setCurrencySeparator( const QString &p_qsCurrencySeparator )
{
    m_qsCurrencySeparator = p_qsCurrencySeparator;
}

QString cPreferences::getCurrencySeparator() const
{
    return m_qsCurrencySeparator;
}

void cPreferences::setCurrencyDecimalSeparator( const QString &p_qsCurrencyDecimalSeparator )
{
    m_qsCurrencyDecimalSeparator = p_qsCurrencyDecimalSeparator;
}

QString cPreferences::getCurrencyDecimalSeparator() const
{
    return m_qsCurrencyDecimalSeparator;
}

void cPreferences::setMaxTreatLength( const unsigned int p_uiMaxTreatLength )
{
    m_uiMaxTreatLength = p_uiMaxTreatLength;
}

unsigned int cPreferences::getMaxTreatLength() const
{
    return m_uiMaxTreatLength;
}

void cPreferences::setDeviceUseVAT( const int p_inVAT )
{
    m_inDeviceUseVAT = p_inVAT;
}

int cPreferences::getDeviceUseVAT() const
{
    return m_inDeviceUseVAT;
}

void cPreferences::setZipLength( const int p_inZip )
{
    m_inZipLength = p_inZip;
}

int cPreferences::getZipLength() const
{
    return m_inZipLength;
}

void cPreferences::setCassaAutoClose( const bool p_bCassaAutoClose )
{
    m_bCassaAutoClose = p_bCassaAutoClose;
}

bool cPreferences::getCassaAutoClose() const
{
    return m_bCassaAutoClose;
}

void cPreferences::setCassaAutoWithdrawal( const bool p_bCassaAutoWithdrawal )
{
    m_bCassaAutoWithdrawal = p_bCassaAutoWithdrawal;
}

bool cPreferences::getCassaAutoWithdrawal() const
{
    return m_bCassaAutoWithdrawal;
}

void cPreferences::setCassaAutoCreate( const bool p_bCassaAutoCreate )
{
    m_bCassaAutoCreateNew = p_bCassaAutoCreate;
}

bool cPreferences::getCassaAutoCreate() const
{
    return m_bCassaAutoCreateNew;
}

void cPreferences::setCassaCreateType( const int p_inCassaCreateType )
{
    m_inCassaCreateType = p_inCassaCreateType;
}

int cPreferences::getCassaCreateType() const
{
    return m_inCassaCreateType;
}

void cPreferences::setDefaultCountry( const QString &p_qsDefaultCountry )
{
    m_qsDefaultCountry = p_qsDefaultCountry;
}

QString cPreferences::getDefaultCountry() const
{
    return m_qsDefaultCountry;
}

void cPreferences::setDBAutoArchive( const bool p_bDBAutoArchive )
{
    m_bDBAutoArchive = p_bDBAutoArchive;
}

bool cPreferences::getDBAutoArchive() const
{
    return m_bDBAutoArchive;
}
void cPreferences::setDBGlobalAutoSynchronize( const bool p_bDBGlobalAutoSynchronize )
{
    m_bDBGlobalAutoSynchronize = p_bDBGlobalAutoSynchronize;
}

bool cPreferences::getDBGlobalAutoSynchronize() const
{
    return m_bDBGlobalAutoSynchronize;
}

void cPreferences::setSecondaryWindowVisibility( const bool p_bIsSecondaryWindowVisible )
{
    m_bIsSecondaryWindowVisible = p_bIsSecondaryWindowVisible;
}

bool cPreferences::isSecondaryWindowVisible() const
{
    return m_bIsSecondaryWindowVisible;
}

void cPreferences::setSecondaryWindowPosition( const QPoint &p_qpPosition )
{
    m_qpSecondaryPosition = p_qpPosition;
    saveSettingI( "EXTWIN_Left", m_qpSecondaryPosition.x() );
    saveSettingI( "EXTWIN_Top", m_qpSecondaryPosition.y() );
}

QPoint cPreferences::secondaryWindowPosition() const
{
    return m_qpSecondaryPosition;
}

void cPreferences::setSecondaryWindowSize( const QSize &p_qsSize )
{
    m_qsSecondarySize = p_qsSize;
    saveSettingI( "EXTWIN_Width", m_qsSecondarySize.width() );
    saveSettingI( "EXTWIN_Height", m_qsSecondarySize.height() );
}

QSize cPreferences::secondaryWindowSize() const
{
    return m_qsSecondarySize;
}

void cPreferences::setSecondaryBackground( const QString &p_qsColor )
{
    m_qsSecondaryBackground = p_qsColor;
}

QString cPreferences::getSecondaryBackground() const
{
    return m_qsSecondaryBackground;
}

void cPreferences::setSecondaryFrame( const QString &p_qsColor )
{
    m_qsSecondaryFrame = p_qsColor;
}

QString cPreferences::getSecondaryFrame() const
{
    return m_qsSecondaryFrame;
}

void cPreferences::setSecondaryCaptionVisibility( const bool p_bIsSecondaryCaptionVisible )
{
    m_bIsSecondaryCaptionVisible = p_bIsSecondaryCaptionVisible;
}

bool cPreferences::isSecondaryCaptionVisible() const
{
    return m_bIsSecondaryCaptionVisible;
}

void cPreferences::setActiveCaptionBackground( const QString &p_qsColor )
{
    m_qsActiveCaptionBackground = p_qsColor;
}

QString cPreferences::getActiveCaptionBackground() const
{
    return m_qsActiveCaptionBackground;
}

void cPreferences::setActiveCaptionColor( const QString &p_qsColor )
{
    m_qsActiveCaptionColor = p_qsColor;
}

QString cPreferences::getActiveCaptionColor() const
{
    return m_qsActiveCaptionColor;
}

void cPreferences::setInactiveCaptionBackground( const QString &p_qsColor )
{
    m_qsInactiveCaptionBackground = p_qsColor;
}

QString cPreferences::getInactiveCaptionBackground() const
{
    return m_qsInactiveCaptionBackground;
}

void cPreferences::setInactiveCaptionColor( const QString &p_qsColor )
{
    m_qsInactiveCaptionColor = p_qsColor;
}

QString cPreferences::getInactiveCaptionColor() const
{
    return m_qsInactiveCaptionColor;
}

void cPreferences::setSecondaryCaptionBackground( const QString &p_qsColor )
{
    m_qsSecondaryCaptionBackground = p_qsColor;
}

QString cPreferences::getSecondaryCaptionBackground() const
{
    return m_qsSecondaryCaptionBackground;
}

void cPreferences::setSecondaryCaptionColor( const QString &p_qsColor )
{
    m_qsSecondaryCaptionColor = p_qsColor;
}

QString cPreferences::getSecondaryCaptionColor() const
{
    return m_qsSecondaryCaptionColor;
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

void cPreferences::setBlnsHttpEnabled( bool p_bEnable )
{
    m_bBlnsHttpEnabled = p_bEnable;
}

bool cPreferences::isBlnsHttpEnabled()
{
    return m_bBlnsHttpEnabled;
}

void cPreferences::setWebSyncAutoStart( bool p_bWebSyncAutoStart )
{
    m_bWebSyncAutoStart = p_bWebSyncAutoStart;
}

bool cPreferences::isWebSyncAutoStart()
{
    return m_bWebSyncAutoStart;
}

void cPreferences::setLogLevel( const unsigned int p_uiFileLevel )
{
    g_obLogger.setMinimumSeverity("file", (cSeverity::teSeverity)p_uiFileLevel );
    saveSettingU( "LOG_FileLogLevel", p_uiFileLevel );
}

unsigned int cPreferences::getLogLevel()
{
    return g_obLogger.getMinimumSeverity( "file" );
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
    saveSettingI( QString( "DLG_%1_width"  ).arg(p_qsDialogName), p_qpDlgSize.x() );
    saveSettingI( QString( "DLG_%1_height" ).arg(p_qsDialogName), p_qpDlgSize.y() );
}

QPoint cPreferences::getDialogSize( const QString &p_qsDialogName, const QPoint &p_qpDlgSizeDefault )
{
    QPoint      qpDlgSize = p_qpDlgSizeDefault;

    qpDlgSize.setX( loadSettingI( QString( "DLG_%1_width"  ).arg(p_qsDialogName), p_qpDlgSizeDefault.x() ) );
    qpDlgSize.setY( loadSettingI( QString( "DLG_%1_height" ).arg(p_qsDialogName), p_qpDlgSizeDefault.y() ) );

    return qpDlgSize;
}

void cPreferences::setDialogPosition(const QString &p_qsDialogName, const QPoint &p_qpPosition)
{
    saveSettingI( QString( "DLG_%1_left" ).arg(p_qsDialogName), p_qpPosition.x() );
    saveSettingI( QString( "DLG_%1_top"  ).arg(p_qsDialogName), p_qpPosition.y() );
}

QPoint cPreferences::dialogPosition(const QString &p_qsDialogName)
{
    QPoint      qpDlgPos = QPoint(0,0);

    qpDlgPos.setX( loadSettingI( QString( "DLG_%1_left" ).arg(p_qsDialogName), 0 ) );
    qpDlgPos.setY( loadSettingI( QString( "DLG_%1_top"  ).arg(p_qsDialogName), 0 ) );

    return qpDlgPos;
}

void cPreferences::setDirDbBinaries( const QString &p_qsDirDbBinaries )
{
    m_qsDirDbBinaries = p_qsDirDbBinaries;
}

QString cPreferences::getDirDbBinaries() const
{
    return m_qsDirDbBinaries;
}

void cPreferences::setDirDbBackup( const QString &p_qsDirDbBackup )
{
    m_qsDirDbBackup = p_qsDirDbBackup;
}

QString cPreferences::getDirDbBackup() const
{
    return m_qsDirDbBackup;
}

void cPreferences::setForceBackupDatabase( bool p_bForceBackupDatabase )
{
    m_bForceBackupDatabase = p_bForceBackupDatabase;
}

bool cPreferences::isForceBackupDatabase()
{
    return m_bForceBackupDatabase;
}

void cPreferences::setBackupDatabase( bool p_bBackupDatabase )
{
    m_bBackupDatabase = p_bBackupDatabase;
}

bool cPreferences::isBackupDatabase()
{
    return m_bBackupDatabase;
}

void cPreferences::setBackupDatabaseType( const int p_nBackupDatabaseType )
{
    m_nBackupDatabaseType = p_nBackupDatabaseType;
}

int cPreferences::getBackupDatabaseType() const
{
    return m_nBackupDatabaseType;
}

void cPreferences::setBackupDatabaseDays( const QString &p_qsBackupDatabaseDays )
{
    m_qsBackupDatabaseDays = p_qsBackupDatabaseDays;
}

QString cPreferences::getBackupDatabaseDays() const
{
    return m_qsBackupDatabaseDays;
}

void cPreferences::setDateFormat( const QString &p_qsDateFormat )
{
    m_qsDateFormat = p_qsDateFormat;
}

QString cPreferences::getDateFormat() const
{
    return m_qsDateFormat;
}

void cPreferences::setFapados( bool p_bFapados )
{
    m_bFapados = p_bFapados;

    if( m_bFapados )
    {
        saveSettingI( "COMPONENT_ID", 66 );
    }
    else
    {
        saveSettingI( "COMPONENT_ID", 42 );
    }
}

bool cPreferences::isFapados()
{
    return m_bFapados;
}

void cPreferences::setPanelSterile(int p_nPanelId, bool p_bSterile)
{
    saveSettingB( QString("PANEL_Panel%1_Sterile").arg( p_nPanelId ), p_bSterile );
}

bool cPreferences::isPanelSterile(int p_nPanelId)
{
    return loadSettingB( QString("PANEL_Panel%1_Sterile").arg( p_nPanelId ), true );
}

void cPreferences::createExtendedAdminPassword( const QString &p_qsExtendedAdminPassword )
{
    g_poDB->executeQTQuery( QString( "INSERT INTO settings ( settingId, identifier, value ) VALUES ( NULL , 'ADMIN_EXT_PASSWORD', '%1' ) " ).arg( p_qsExtendedAdminPassword ) );
}

void cPreferences::setExtendedAdminPassword( const QString &p_qsExtendedAdminPassword )
{
    g_poDB->executeQTQuery( QString( "UPDATE settings SET value='%1' WHERE identifier='ADMIN_EXT_PASSWORD' " ).arg( p_qsExtendedAdminPassword ) );
}

bool cPreferences::checkExtendedAdminPassword(const QString &p_qsExtendedAdminPassword) const
{
    bool bRet = false;

    QSqlQuery *poQuery = g_poDB->executeQTQuery( "SELECT value FROM settings WHERE identifier='ADMIN_EXT_PASSWORD' " );

    if( p_qsExtendedAdminPassword.length() == 0 )
    {
        if( poQuery->size() > 0 )
            return true;
        else
            return false;
    }

    poQuery->first();

    QString qsPassword = poQuery->value( 0 ).toString();

    if( qsPassword.compare( p_qsExtendedAdminPassword ) == 0 )
    {
        bRet = true;
    }

    return bRet;
}

void cPreferences::setStopInLine( bool p_bStopInLine )
{
    m_bIsStopInLine = p_bStopInLine;
}

bool cPreferences::isStopInLine()
{
    return m_bIsStopInLine;
}

void cPreferences::setPanelTextSteril( const QString &p_qsPanelTextSteril )
{
    m_qsPanelTextSteril = p_qsPanelTextSteril;
}

QString cPreferences::getPanelTextSteril() const
{
    return m_qsPanelTextSteril;
}

void cPreferences::setPanelTextTubeReplace( const QString &p_qsPanelTextTubeReplace )
{
    m_qsPanelTextTubeReplace = p_qsPanelTextTubeReplace;
}

QString cPreferences::getPanelTextTubeReplace() const
{
    return m_qsPanelTextTubeReplace;
}

void cPreferences::setPanelTextTubeCleanup( const QString &p_qsPanelTextTubeCleanup )
{
    m_qsPanelTextTubeCleanup = p_qsPanelTextTubeCleanup;
}

QString cPreferences::getPanelTextTubeCleanup() const
{
    return m_qsPanelTextTubeCleanup;
}

void cPreferences::setHWDebug( bool p_bHWDebug )
{
    m_bEnableHWDebug = p_bHWDebug;
}

bool cPreferences::isHWDebugEnabled()
{
    return m_bEnableHWDebug;
}

void cPreferences::setForceModuleSendTime( bool p_bForceModuleSendTime )
{
    m_bForceModuleSendTime = p_bForceModuleSendTime;
}

bool cPreferences::isForceModuleSendTime()
{
    return m_bForceModuleSendTime;
}

void cPreferences::setForceModuleCheckButton( bool p_bForceModuleCheckButton )
{
    m_bForceModuleCheckButton = p_bForceModuleCheckButton;
}

bool cPreferences::isForceModuleCheckButton()
{
    return m_bForceModuleCheckButton;
}

void cPreferences::setForceTimeSendCounter( const int p_nForceTimeSendCounter )
{
    m_nForceTimeSendCounter = p_nForceTimeSendCounter;
}

int cPreferences::getForceTimeSendCounter() const
{
    return m_nForceTimeSendCounter;
}

void cPreferences::setTextTubeReplaceVisible( bool p_bTextTubeReplaceVisible )
{
    m_bIsTextTubeReplaceVisible = p_bTextTubeReplaceVisible;
}

bool cPreferences::isTextTubeReplaceVisible()
{
    return m_bIsTextTubeReplaceVisible;
}

void cPreferences::setTextTubeCleanupVisible( bool p_bTextTubeCleanupVisible )
{
    m_bIsTextTubeCleanupVisible = p_bTextTubeCleanupVisible;
}

bool cPreferences::isTextTubeCleanupVisible()
{
    return m_bIsTextTubeCleanupVisible;
}

void cPreferences::setTextSterilVisible( bool p_bTextSterilVisible )
{
    m_bIsTextSterilVisible = p_bTextSterilVisible;
}
bool cPreferences::isTextSterilVisible()
{
    return m_bIsTextSterilVisible;
}

void cPreferences::setBarcodeHidden( bool p_bBarcodeHidden )
{
    m_bBarcodeHidden = p_bBarcodeHidden;
}

bool cPreferences::isBarcodeHidden()
{
    return m_bBarcodeHidden;
}

void cPreferences::setShowPatientInfoOnStart( bool p_bShowPatientInfoOnStart )
{
    m_bShowPatientInfoOnStart = p_bShowPatientInfoOnStart;
}

bool cPreferences::isShowPatientInfoOnStart()
{
    return m_bShowPatientInfoOnStart;
}

void cPreferences::setShowInfoOnWindow( const int p_nShowInfoOnWindow )
{
    m_nShowInfoOnWindow = p_nShowInfoOnWindow;
}

int cPreferences::getShowInfoOnWindow() const
{
    return m_nShowInfoOnWindow;
}

void cPreferences::setCloseInfoWindowAfterSecs( const int p_nCloseInfoWindowAfterSecs )
{
    m_nCloseInfoWindowAfterSecs = p_nCloseInfoWindowAfterSecs;
}
int cPreferences::getCloseInfoWindowAfterSecs() const
{
    return m_nCloseInfoWindowAfterSecs;
}

void cPreferences::setUsageVisibleOnMain( bool p_bUsageVisibleOnMain )
{
    m_bUsageVisibleOnMain = p_bUsageVisibleOnMain;
}

bool cPreferences::isUsageVisibleOnMain()
{
    return m_bUsageVisibleOnMain;
}

void cPreferences::setLicenceLastValidated( const QString &p_qsLicenceLastValidated )
{
    m_qsLicenceLastValidated = p_qsLicenceLastValidated;
    saveSettingS( "LICENCE_LAST_VALIDATED", m_qsLicenceLastValidated );
}

QString cPreferences::getLicenceLastValidated() const
{
    return m_qsLicenceLastValidated;
}

void cPreferences::setDACanModifyWorktime( bool p_bDACanModifyWorktime )
{
    m_bDACanModifyWorktime = p_bDACanModifyWorktime;
    saveSettingB( "DA_WORKTIME", m_bDACanModifyWorktime );
}

bool cPreferences::isDACanModifyWorktime()
{
    return m_bDACanModifyWorktime;
}

void cPreferences::setDACanModifyExpDate( bool p_bDACanModifyExpDate )
{
    m_bDACanModifyExpDate = p_bDACanModifyExpDate;
    saveSettingB( "DA_EXPDATE", m_bDACanModifyExpDate );
}

bool cPreferences::isDACanModifyExpDate()
{
    return m_bDACanModifyExpDate;
}

void cPreferences::setAutoMailOnPCSell( bool p_bAutoMailOnPCSell )
{
    m_bAutoMailOnPCSell = p_bAutoMailOnPCSell;
}

bool cPreferences::isAutoMailOnPCSell()
{
    return m_bAutoMailOnPCSell;
}

void cPreferences::setAutoMailOnPCUse( bool p_bAutoMailOnPCUse )
{
    m_bAutoMailOnPCUse = p_bAutoMailOnPCUse;
}

bool cPreferences::isAutoMailOnPCUse()
{
    return m_bAutoMailOnPCUse;
}

void cPreferences::setAutoMailOnPCExpiration( bool p_bAutoMailOnPCExpiration )
{
    m_bAutoMailOnPCExpiration = p_bAutoMailOnPCExpiration;
}

bool cPreferences::isAutoMailOnPCExpiration()
{
    return m_bAutoMailOnPCExpiration;
}

void cPreferences::setPCExpirationDays( const int p_nPCExpirationDays )
{
    m_nPCExpirationDays = p_nPCExpirationDays;
}

int cPreferences::getPCExpirationDays() const
{
    return m_nPCExpirationDays;
}

void cPreferences::setAutoMailOnPCUnitChange( bool p_bAutoMailOnPCUnitChange )
{
    m_bAutoMailOnPCUnitChange = p_bAutoMailOnPCUnitChange;
}

bool cPreferences::isAutoMailOnPCUnitChange()
{
    return m_bAutoMailOnPCUnitChange;
}

void cPreferences::setAdvertisementSizeAndPos( const unsigned int p_uiId,
                                               const unsigned int p_uiLeft,
                                               const unsigned int p_uiTop,
                                               const unsigned int p_uiWidth,
                                               const unsigned int p_uiHeight )
{
    saveSettingU( QString( "DLG_Ad%1_left" ).arg(p_uiId), p_uiLeft );
    saveSettingU( QString( "DLG_Ad%1_top" ).arg(p_uiId), p_uiTop );
    saveSettingU( QString( "DLG_Ad%1_width" ).arg(p_uiId), p_uiWidth );
    saveSettingU( QString( "DLG_Ad%1_height" ).arg(p_uiId), p_uiHeight );
}

void cPreferences::setSecondsWaitOnSlpashScreen( const int p_nSecondsWaitOnSlpashScreen )
{
    m_nSecondsWaitOnSlpashScreen = p_nSecondsWaitOnSlpashScreen;
}

int cPreferences::getSecondsWaitOnSlpashScreen() const
{
    return m_nSecondsWaitOnSlpashScreen;
}

void cPreferences::setValue( const QString &p_qsKey, const QString &p_qsValue )
{
    saveSettingS( p_qsKey, p_qsValue );
}

QString cPreferences::getValue( const QString &p_qsKey )
{
    return loadSettingS( p_qsKey, "" );
}

void cPreferences::setWindowMain( QWidget *p_wHandle )
{
    m_wWindowMain = p_wHandle;
}

void cPreferences::setWindowSecondary(QWidget *p_wHandle )
{
    m_wWindowSecondary = p_wHandle;
}

void cPreferences::moveWindowMain( int posX, int posY )
{
    m_wWindowMain->move( posX, posY );

    setMainWindowSizePos( posX, posY, m_uiMainWindowWidth, m_uiMainWindowHeight );
}

void cPreferences::resizeWindowMain( int width, int height )
{
    m_wWindowMain->resize( width, height );

    setMainWindowSizePos( m_uiMainWindowLeft, m_uiMainWindowTop, width, height );
}

void cPreferences::moveWindowSecondary( int posX, int posY )
{
    m_wWindowSecondary->move( posX, posY );

    setSecondaryWindowPosition( QPoint( posX, posY ) );
}

void cPreferences::resizeWindowSecondary( int width, int height )
{
    m_wWindowSecondary->resize( width, height );

    setSecondaryWindowSize( QSize( width, height ) );
}

void cPreferences::setRFIDEnabled( bool p_bIsRFIDEnabled )
{
    m_bIsRFIDEnabled = p_bIsRFIDEnabled;
}

bool cPreferences::isRFIDEnabled()
{
    return m_bIsRFIDEnabled;
}

void cPreferences::setRFIDComPort( const int p_nRFIDComPort )
{
    m_nRFIDComPort = p_nRFIDComPort;
}

int cPreferences::getRFIDComPort() const
{
    return m_nRFIDComPort;
}

void cPreferences::setLicenceCheck( const int p_nLicenceCheck )
{
    m_nLicenceCheck = p_nLicenceCheck;

    if( m_nLicenceCheckCounter > m_nLicenceCheck ) resetLicenceCheckCounter();
}

int  cPreferences::getLicenceCheck() const
{
    return m_nLicenceCheck;
}

void cPreferences::setLicenceCheckCounter( const int p_nLicenceCheckCounter )
{
    m_nLicenceCheckCounter = p_nLicenceCheckCounter;
    saveSettingI( "LICENCE_CHECK_COUNTER", m_nLicenceCheckCounter );
}

int  cPreferences::getLicenceCheckCounter() const
{
    return m_nLicenceCheckCounter;
}

void cPreferences::resetLicenceCheckCounter()
{
    m_nLicenceCheckCounter = m_nLicenceCheck;
    saveSettingI( "LICENCE_CHECK_COUNTER", m_nLicenceCheckCounter );
}

void cPreferences::decreaseLicenceCheckCounter()
{
    m_nLicenceCheckCounter = loadSettingI( "LICENCE_CHECK_COUNTER", 56 );
    if( m_nLicenceCheckCounter > 0 )  m_nLicenceCheckCounter--;
    saveSettingI( "LICENCE_CHECK_COUNTER", m_nLicenceCheckCounter );
}

void cPreferences::setWorktime( const int p_nWorktime )
{
    m_nWorkTime = p_nWorktime;

    if( m_nWorkTimeCounter > m_nWorkTime ) resetWorktimeCounter();
}

int  cPreferences::getWorktime() const
{
    return m_nWorkTime;
}

void cPreferences::setWorktimeCounter( const int p_nWorktimeCounter )
{
    m_nWorkTimeCounter = p_nWorktimeCounter;
    saveSettingI( "LICENCE_WORKTIME_COUNTER", m_nWorkTimeCounter );
}

int  cPreferences::getWorktimeCounter() const
{
    return m_nWorkTimeCounter;
}

void cPreferences::resetWorktimeCounter()
{
    m_nWorkTimeCounter = m_nWorkTime;
    saveSettingI( "LICENCE_WORKTIME_COUNTER", m_nWorkTimeCounter );
}

void cPreferences::decreaseWorktimeCounter()
{
    m_nWorkTimeCounter = loadSettingI( "LICENCE_WORKTIME_COUNTER", 336 );
    if( m_nWorkTimeCounter > 0 ) m_nWorkTimeCounter--;
    saveSettingI( "LICENCE_WORKTIME_COUNTER", m_nWorkTimeCounter );
}

void cPreferences::setCardyGoSync( bool p_bCardyGoSync )
{
    m_bCardyGoSync = p_bCardyGoSync;
}

bool cPreferences::isCardyGoSync()
{
    return m_bCardyGoSync;
}

void cPreferences::setDBAllowDeleteObsoleteUnits(bool p_bEnable)
{
    m_bAllowDeleteObsoleteUnits = p_bEnable;
}

bool cPreferences::isDBAllowDeleteObsoleteUnits()
{
    return m_bAllowDeleteObsoleteUnits;
}

void cPreferences::setObsoleteUnitsDays( const int p_nObsolateUnitsDays )
{
    m_nObsolateUnitsDays = p_nObsolateUnitsDays;
}

int cPreferences::getObsolateUnitsDays() const
{
    return m_nObsolateUnitsDays;
}

void cPreferences::setLogFilesDeleteAllowed( bool p_bLogFilesDeleteAllowed )
{
    m_bLogFilesDeleteAllowed = p_bLogFilesDeleteAllowed;
}

bool cPreferences::isLogFilesDeleteAllowed()
{
    return m_bLogFilesDeleteAllowed;
}

void cPreferences::setDeleteLogFilesMonths( const int p_nDeleteLogFilesMonths )
{
    m_nDeleteLogFilesMonths = p_nDeleteLogFilesMonths;
}

int cPreferences::getDeleteLogFilesMonths() const
{
    return m_nDeleteLogFilesMonths;
}

void cPreferences::setSaveAdhocGuest(bool p_bSaveAdhocGuest)
{
    m_bSaveAdhocGuest = p_bSaveAdhocGuest;
}

bool cPreferences::isSaveAdhocGuest()
{
    return m_bSaveAdhocGuest;
}

