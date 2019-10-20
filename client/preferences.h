//====================================================================================
//
// Belenus Kliens alkalmazas (c) Pagony Multimedia Studio Bt - 2010
//
//====================================================================================
//
// Filename    : preferences.h
// AppVersion  : 1.0
// FileVersion : 1.0
// Author      : Ballok Peter, Bikfalvi Tamas, Kovacs Gabor
//
//====================================================================================
// Alkalmazas beallitasok allomanya.
//====================================================================================

#ifndef PREFERENCES_H
#define PREFERENCES_H

#include <QString>
#include <QStringList>
#include <QPoint>
#include <QSize>
#include "../framework/sevexception.h"

class cPreferences
{
public:
    cPreferences();
    ~cPreferences();

    void            setApplicationPath( const QString &p_qsPath );
    QString         getApplicationPath() const;
    void            setVersion( const QString &p_qsVersion );
    QString         getVersion() const;
    void            setVersionDb(const QString &p_qsVersion);
    QString         getVersionDb() const;
    void            setComponents( const unsigned int p_uiComponent );
    unsigned int    getComponents() const;
    void            processComponentID();
    bool            isComponentSensoliteInstalled();
    bool            isComponentKiwiSunInstalled();
    bool            isComponentDatabaseInstalled();
    bool            isComponentHardwareInstalled();
    bool            isComponentInternetInstalled();
    bool            isComponentClientInstalled();
    bool            isComponentViewerInstalled();
    void            setLangFilePrefix( const QString &p_qsPrefix );
    QString         getLangFilePrefix() const;
    void            setLang( const QString &p_qsLang );
    QString         getLang() const;
    void            setLastUser( const QString &p_qsLastUser );
    QString         getLastUser() const;
    void            setPanelsPerRow( const unsigned int p_uiPanelsPerRow );
    unsigned int    getPanelsPerRow() const;
    unsigned int    getPanelCount() const;
    void            setMainWindowSizePos(const unsigned int p_uiMainWindowLeft,
                                          const unsigned int p_uiMainWindowTop,
                                          const unsigned int p_uiMainWindowWidth,
                                          const unsigned int p_uiMainWindowHeight);
    unsigned int    getMainWindowLeft() const;
    unsigned int    getMainWindowTop() const;
    unsigned int    getMainWindowWidth() const;
    unsigned int    getMainWindowHeight() const;
    void            setMainBackground( const QString &p_qsColor );
    QString         getMainBackground() const;
    unsigned int    getLicenceId() const;
    void            setLicenceId( const int licenceId );
    void            setServerAddress( const QString &p_qsServerAddress );
    QString         getServerAddress() const;
    void            setServerPort( const QString &p_qsServerPort );
    QString         getServerPort() const;
    void            setCommunicationPort( const int p_inPortNumber );
    int             getCommunicationPort() const;
    void            setBarcodeLength( const int p_inBarcodeLength );
    int             getBarcodeLength() const;
    void            setBarcodePrefix( const QString &p_qsPrefix );
    QString         getBarcodePrefix() const;
    void            setBarcodeLengthDifferent( const bool p_bBarcodeLengthDifferent );
    bool            isBarcodeLengthDifferent() const;
    void            setCurrencyShort( const QString &p_qsCurrencyShort );
    QString         getCurrencyShort() const;
    void            setCurrencyLong( const QString &p_qsCurrencyLong );
    QString         getCurrencyLong() const;
    void            setCurrencySeparator( const QString &p_qsCurrencySeparator );
    QString         getCurrencySeparator() const;
    void            setCurrencyDecimalSeparator( const QString &p_qsCurrencyDecimalSeparator );
    QString         getCurrencyDecimalSeparator() const;
    void            setMaxTreatLength( const unsigned int p_uiMaxTreatLength );
    unsigned int    getMaxTreatLength() const;
    void            setDeviceUseVAT( const int p_inVAT );
    int             getDeviceUseVAT() const;
    void            setCassaAutoClose( const bool p_bCassaAutoClose );
    bool            getCassaAutoClose() const;
    void            setCassaAutoWithdrawal( const bool p_bCassaAutoWithdrawal );
    bool            getCassaAutoWithdrawal() const;
    void            setCassaAutoCreate( const bool p_bCassaAutoCreate );
    bool            getCassaAutoCreate() const;
    void            setCassaCreateType( const int p_inCassaCreateType );
    int             getCassaCreateType() const;
    void            setDefaultCountry( const QString &p_qsDefaultCountry );
    QString         getDefaultCountry() const;
    void            setZipLength( const int p_inZip );
    int             getZipLength() const;
    void            setDBAutoArchive( const bool p_bDBAutoArchive );
    bool            getDBAutoArchive() const;
    void            setDBGlobalAutoSynchronize( const bool p_bDBGlobalAutoSynchronize );
    bool            getDBGlobalAutoSynchronize() const;
    void            setSecondaryWindowVisibility( const bool p_bIsSecondaryWindowVisible );
    bool            isSecondaryWindowVisible() const;
    void            setSecondaryWindowPosition( const QPoint &p_qpPosition );
    QPoint          secondaryWindowPosition() const;
    void            setSecondaryWindowSize( const QSize &p_qsSize );
    QSize           secondaryWindowSize() const;
    void            setSecondaryBackground( const QString &p_qsColor );
    QString         getSecondaryBackground() const;
    void            setSecondaryFrame( const QString &p_qsColor );
    QString         getSecondaryFrame() const;
    void            setSecondaryCaptionVisibility( const bool p_bIsSecondaryCaptionVisible );
    bool            isSecondaryCaptionVisible() const;
    void            setActiveCaptionBackground( const QString &p_qsColor );
    QString         getActiveCaptionBackground() const;
    void            setActiveCaptionColor( const QString &p_qsColor );
    QString         getActiveCaptionColor() const;
    void            setInactiveCaptionBackground( const QString &p_qsColor );
    QString         getInactiveCaptionBackground() const;
    void            setInactiveCaptionColor( const QString &p_qsColor );
    QString         getInactiveCaptionColor() const;
    void            setSecondaryCaptionBackground( const QString &p_qsColor );
    QString         getSecondaryCaptionBackground() const;
    void            setSecondaryCaptionColor( const QString &p_qsColor );
    QString         getSecondaryCaptionColor() const;
    void            setPatientCardLostPrice( const int p_inPrice );
    int             getPatientCardLostPrice() const;
    void            setPatientCardLostPriceVat(const int p_inVat );
    int             getPatientCardLostPriceVat() const;
    void            setPatientCardPartnerPrice( const int p_inPrice );
    int             getPatientCardPartnerPrice() const;
    void            setPatientCardPartnerPriceVat(const int p_inVat );
    int             getPatientCardPartnerPriceVat() const;
    void            setBlnsHttpEnabled( bool p_bEnable );
    bool            isBlnsHttpEnabled();

    unsigned int    getPanelId( int p_nPanelIterator );

    void            setLogLevel(const unsigned int p_uiFileLevel);
    unsigned int    getLogLevel();
    void            setDBAccess( const QString &p_qsHost, const QString &p_qsDB,
                                 const QString &p_qsUser, const QString &p_qsPwd );
    void            getDBAccess( QString *p_poHost = NULL, QString *p_poDB = NULL,
                                 QString *p_poUser = NULL, QString *p_poPwd = NULL) const;


    unsigned int    loadSettingU( QString p_Identifier, unsigned int p_Default ) throw (cSevException);
    QString         loadSettingS( QString p_Identifier, QString p_Default ) throw (cSevException);
    bool            loadSettingB( QString p_Identifier, bool p_Default ) throw (cSevException);
    int             loadSettingI( QString p_Identifier, int p_Default ) throw (cSevException);

    void            saveSettingU( QString p_Identifier, unsigned int p_Value ) throw (cSevException);
    void            saveSettingS( QString p_Identifier, QString p_Value ) throw (cSevException);
    void            saveSettingB( QString p_Identifier, bool p_Value ) throw (cSevException);
    void            saveSettingI( QString p_Identifier, int p_Value ) throw (cSevException);

    void            loadSettings() throw (cSevException);
    void            saveSettings() throw (cSevException);
    void            loadDBSettings() throw (cSevException);

    unsigned int    postponedPatients() const;
    void            setPostponedPatients( const unsigned int p_uiPostponedPatients );
    unsigned int    postponedAttendances() const;
    void            setPostponedAttendances( const unsigned int p_uiPostponedAttendances );
    void            setDialogSize( const QString &p_qsDialogName, const QPoint &p_qpDlgSize );
    QPoint          getDialogSize( const QString &p_qsDialogName, const QPoint &p_qpDlgSizeDefault );
    void            setDialogPosition(const QString &p_qsDialogName, const QPoint &p_qpPosition );
    QPoint          dialogPosition(const QString &p_qsDialogName);

    void            setDirDbBinaries( const QString &p_qsDirDbBinaries );
    QString         getDirDbBinaries() const;
    void            setDirDbBackup( const QString &p_qsDirDbBackup );
    QString         getDirDbBackup() const;
    void            setForceBackupDatabase( bool p_bForceBackupDatabase );
    bool            isForceBackupDatabase();
    void            setBackupDatabase( bool p_bBackupDatabase );
    bool            isBackupDatabase();
    void            setBackupDatabaseType( const int p_nBackupDatabaseType );
    int             getBackupDatabaseType() const;
    void            setBackupDatabaseDays( const QString &p_qsBackupDatabaseDays );
    QString         getBackupDatabaseDays() const;
    void            setDateFormat( const QString &p_qsDateFormat );
    QString         getDateFormat() const;
    void            setFapados( bool p_bFapados );
    bool            isFapados();
    void            setPanelSterile( int p_nPanelId, bool p_bSterile );
    bool            isPanelSterile( int p_nPanelId );
    void            createExtendedAdminPassword( const QString &p_qsExtendedAdminPassword );
    void            setExtendedAdminPassword( const QString &p_qsExtendedAdminPassword );
    bool            checkExtendedAdminPassword( const QString &p_qsExtendedAdminPassword ) const;
    void            setStopInLine( bool p_bStopInLine );
    bool            isStopInLine();
    void            setPanelTextSteril( const QString &p_qsPanelTextSteril );
    QString         getPanelTextSteril() const;
    void            setPanelTextTubeReplace( const QString &p_qsPanelTextTubeReplace );
    QString         getPanelTextTubeReplace() const;
    void            setPanelTextTubeCleanup( const QString &p_qsPanelTextTubeCleanup );
    QString         getPanelTextTubeCleanup() const;
    void            setHWDebug( bool p_bHWDebug );
    bool            isHWDebugEnabled();
    void            setForceModuleSendTime( bool p_bForceModuleSendTime );
    bool            isForceModuleSendTime();
    void            setForceModuleCheckButton( bool p_bForceModuleCheckButton );
    bool            isForceModuleCheckButton();
    void            setForceTimeSendCounter( const int p_nForceTimeSendCounter );
    int             getForceTimeSendCounter() const;
    void            setTextTubeReplaceVisible( bool p_bTextTubeReplaceVisible );
    bool            isTextTubeReplaceVisible();
    void            setTextTubeCleanupVisible( bool p_bTextTubeCleanupVisible );
    bool            isTextTubeCleanupVisible();
    void            setTextSterilVisible( bool p_bTextSterilVisible );
    bool            isTextSterilVisible();
    void            setBarcodeHidden( bool p_bBarcodeHidden );
    bool            isBarcodeHidden();
    void            setUsageVisibleOnMain( bool p_bUsageVisibleOnMain );
    bool            isUsageVisibleOnMain();
    void            setLicenceLastValidated( const QString &p_qsLicenceLastValidated );
    QString         getLicenceLastValidated() const;
    void            setDACanModifyWorktime( bool p_bDACanModifyWorktime );
    bool            isDACanModifyWorktime();
    void            setDACanModifyExpDate( bool p_bDACanModifyExpDate );
    bool            isDACanModifyExpDate();
    void            setWebSyncAutoStart( bool p_bWebSyncAutoStart );
    bool            isWebSyncAutoStart();
    void            setAutoMailOnPCSell( bool p_bAutoMailOnPCSell );
    bool            isAutoMailOnPCSell();
    void            setAutoMailOnPCUse( bool p_bAutoMailOnPCUse );
    bool            isAutoMailOnPCUse();
    void            setAutoMailOnPCExpiration( bool p_bAutoMailOnPCExpiration );
    bool            isAutoMailOnPCExpiration();
    void            setPCExpirationDays( const int p_nPCExpirationDays  );
    int             getPCExpirationDays() const;
    void            setAdvertisementSizeAndPos(const unsigned int p_uiId,
                                                const unsigned int p_uiLeft,
                                                const unsigned int p_uiTop,
                                                const unsigned int p_uiWidth,
                                                const unsigned int p_uiHeight);
    void            setSecondsWaitOnSlpashScreen( const int p_nSecondsWaitOnSlpashScreen );
    int             getSecondsWaitOnSlpashScreen() const;

    void            setValue(const QString &p_qsKey, const QString &p_qsValue);
    QString         getValue( const QString &p_qsKey );

    void            setWindowMain( QWidget *p_wHandle );
    void            setWindowSecondary( QWidget *p_wHandle );
    void            moveWindowMain( int posX, int posY );
    void            resizeWindowMain( int width, int height );
    void            moveWindowSecondary( int posX, int posY );
    void            resizeWindowSecondary( int width, int height );

    void            setShowPatientInfoOnStart( bool p_bShowPatientInfoOnStart );
    bool            isShowPatientInfoOnStart();
    void            setShowInfoOnWindow( const int p_nShowInfoOnWindow );
    int             getShowInfoOnWindow() const;
    void            setCloseInfoWindowAfterSecs( const int p_nCloseInfoWindowAfterSecs );
    int             getCloseInfoWindowAfterSecs() const;

    void            setRFIDEnabled( bool p_bIsRFIDEnabled );
    bool            isRFIDEnabled();
    void            setRFIDComPort( const int p_nRFIDComPort );
    int             getRFIDComPort() const;

//void            setXXX( const QString &p_qsXXX );
//QString         getXXX() const;
//void            setXXX( const int p_nXXX );
//int             getXXX() const;
//void            setXXX( bool p_bXXX );
//bool            isXXX();

private:
    QWidget        *m_wWindowMain;
    QWidget        *m_wWindowSecondary;
    QString         m_qsApplicationPath;
    QString         m_qsVersion;
    QString         m_qsVersionDb;
    unsigned int    m_uiComponent;
    bool            m_bComponentSensolite;
    bool            m_bComponentKiwiSun;
    bool            m_bComponentDatabase;
    bool            m_bComponentHardware;
    bool            m_bComponentInternet;
    bool            m_bComponentClient;
    bool            m_bComponentViewer;
    QString         m_qsLangFilePrefix;
    QString         m_qsLang;
    QString         m_qsLastUser;
    unsigned int    m_uiPanelsPerRow;
    unsigned int    m_uiPanelCount;
    unsigned int    m_uiMainWindowLeft;
    unsigned int    m_uiMainWindowTop;
    unsigned int    m_uiMainWindowWidth;
    unsigned int    m_uiMainWindowHeight;
    QString         m_qsMainBackground;
    unsigned int    m_uiLicenceId;
    QString         m_qsServerAddress;
    QString         m_qsServerPort;
    int             m_inCommunicationPort;
    int             m_inBarcodeLength;
    QString         m_qsBarcodePrefix;
    unsigned int    m_uiPostponedPatients;
    unsigned int    m_uiPostponedAttendances;
    QString         m_qsCurrencyShort;
    QString         m_qsCurrencyLong;
    QString         m_qsCurrencySeparator;
    QString         m_qsCurrencyDecimalSeparator;
    unsigned int    m_uiMaxTreatLength;
    int             m_inDeviceUseVAT;
    bool            m_bCassaAutoClose;
    bool            m_bCassaAutoWithdrawal;
    bool            m_bCassaAutoCreateNew;
    int             m_inCassaCreateType;
    QString         m_qsDefaultCountry;
    int             m_inZipLength;
    bool            m_bDBAutoArchive;
    bool            m_bDBGlobalAutoSynchronize;
    bool            m_bIsSecondaryWindowVisible;
    QPoint          m_qpSecondaryPosition;
    QSize           m_qsSecondarySize;
    QString         m_qsSecondaryBackground;
    QString         m_qsSecondaryFrame;
    bool            m_bIsSecondaryCaptionVisible;
    bool            m_bBarcodeLengthDifferent;
    int             m_nPatientCardLostPrice;
    int             m_nPatientCardLostPriceVat;
    int             m_nPatientCardPartnerPrice;
    int             m_nPatientCardPartnerPriceVat;
    QStringList     m_qslPanelIds;
    bool            m_bBlnsHttpEnabled;
    QString         m_qsDirDbBinaries;
    QString         m_qsDirDbBackup;
    bool            m_bForceBackupDatabase;
    bool            m_bBackupDatabase;
    int             m_nBackupDatabaseType;
    QString         m_qsBackupDatabaseDays;
    QString         m_qsDateFormat;
    bool            m_bFapados;
    int             m_nFapados;
    QString         m_qsActiveCaptionBackground;
    QString         m_qsActiveCaptionColor;
    QString         m_qsInactiveCaptionBackground;
    QString         m_qsInactiveCaptionColor;
    QString         m_qsSecondaryCaptionBackground;
    QString         m_qsSecondaryCaptionColor;
    bool            m_bIsStopInLine;
    QString         m_qsPanelTextSteril;
    QString         m_qsPanelTextTubeReplace;
    QString         m_qsPanelTextTubeCleanup;
    bool            m_bEnableHWDebug;
    bool            m_bForceModuleSendTime;
    bool            m_bForceModuleCheckButton;
    int             m_nForceTimeSendCounter;
    bool            m_bIsTextTubeReplaceVisible;
    bool            m_bIsTextTubeCleanupVisible;
    bool            m_bIsTextSterilVisible;
    bool            m_bBarcodeHidden;
    bool            m_bUsageVisibleOnMain;
    QString         m_qsLicenceLastValidated;
    bool            m_bDACanModifyWorktime;
    bool            m_bDACanModifyExpDate;
    bool            m_bWebSyncAutoStart;
    bool            m_bAutoMailOnPCSell;
    bool            m_bAutoMailOnPCUse;
    bool            m_bAutoMailOnPCExpiration;
    int             m_nPCExpirationDays;
    int             m_nSecondsWaitOnSlpashScreen;
    bool            m_bShowPatientInfoOnStart;
    int             m_nShowInfoOnWindow;
    int             m_nCloseInfoWindowAfterSecs;
    bool            m_bIsRFIDEnabled;
    int             m_nRFIDComPort;

    void init();
};

#endif
