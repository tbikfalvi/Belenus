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
    cPreferences( const QString &p_qsFileName );
    ~cPreferences();

    void            setFileName( const QString &p_qsFileName );
    QString         getFileName() const;
    void            setVersion( const QString &p_qsVersion, bool p_boSaveNow = false );
    QString         getVersion() const;
    void            setVersionDb(const QString &p_qsVersion);
    QString         getVersionDb() const;
    void            setComponents( const unsigned int p_uiComponent, bool p_boSaveNow = false );
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
    void            setLang( const QString &p_qsLang, bool p_boSaveNow = false );
    QString         getLang() const;
    void            setLastUser( const QString &p_qsLastUser, bool p_boSaveNow = false );
    QString         getLastUser() const;
    void            setPanelsPerRow( const unsigned int p_uiPanelsPerRow, bool p_boSaveNow = false );
    unsigned int    getPanelsPerRow() const;
    unsigned int    getPanelCount() const;
    void            setMainWindowSizePos( const unsigned int p_uiMainWindowLeft,
                                          const unsigned int p_uiMainWindowTop,
                                          const unsigned int p_uiMainWindowWidth,
                                          const unsigned int p_uiMainWindowHeight,
                                          bool p_boSaveNow = false );
    unsigned int    getMainWindowLeft() const;
    unsigned int    getMainWindowTop() const;
    unsigned int    getMainWindowWidth() const;
    unsigned int    getMainWindowHeight() const;
    void            setMainBackground( const QString &p_qsColor, bool p_boSaveNow = false );
    QString         getMainBackground() const;
    unsigned int    getLicenceId() const;
    void            setLicenceId( const int licenceId );
    void            setServerAddress( const QString &p_qsServerAddress, bool p_boSaveNow = false );
    QString         getServerAddress() const;
    void            setServerPort( const QString &p_qsServerPort, bool p_boSaveNow = false );
    QString         getServerPort() const;
    void            setCommunicationPort( const int p_inPortNumber, bool p_boSaveNow = false );
    int             getCommunicationPort() const;
    void            setBarcodeLength( const int p_inBarcodeLength, bool p_boSaveNow = false );
    int             getBarcodeLength() const;
    void            setBarcodePrefix( const QString &p_qsPrefix, bool p_boSaveNow = false );
    QString         getBarcodePrefix() const;
    void            setBarcodeLengthDifferent( const bool p_bBarcodeLengthDifferent, bool p_boSaveNow = false );
    bool            isBarcodeLengthDifferent() const;
    void            setCurrencyShort( const QString &p_qsCurrencyShort, bool p_boSaveNow = false );
    QString         getCurrencyShort() const;
    void            setCurrencyLong( const QString &p_qsCurrencyLong, bool p_boSaveNow = false );
    QString         getCurrencyLong() const;
    void            setCurrencySeparator( const QString &p_qsCurrencySeparator, bool p_boSaveNow = false );
    QString         getCurrencySeparator() const;
    void            setCurrencyDecimalSeparator( const QString &p_qsCurrencyDecimalSeparator, bool p_boSaveNow = false );
    QString         getCurrencyDecimalSeparator() const;
    void            setMaxTreatLength( const unsigned int p_uiMaxTreatLength, bool p_boSaveNow = false );
    unsigned int    getMaxTreatLength() const;
    void            setDeviceUseVAT( const int p_inVAT, bool p_boSaveNow = false );
    int             getDeviceUseVAT() const;
    void            setCassaAutoClose( const bool p_bCassaAutoClose, bool p_boSaveNow = false );
    bool            getCassaAutoClose() const;
    void            setCassaAutoWithdrawal( const bool p_bCassaAutoWithdrawal, bool p_boSaveNow = false );
    bool            getCassaAutoWithdrawal() const;
    void            setCassaAutoCreate( const bool p_bCassaAutoCreate, bool p_boSaveNow = false );
    bool            getCassaAutoCreate() const;
    void            setCassaCreateType( const int p_inCassaCreateType, bool p_boSaveNow = false );
    int             getCassaCreateType() const;
    void            setDefaultCountry( const QString &p_qsDefaultCountry, bool p_boSaveNow = false );
    QString         getDefaultCountry() const;
    void            setZipLength( const int p_inZip, bool p_boSaveNow = false );
    int             getZipLength() const;
    void            setDBAutoArchive( const bool p_bDBAutoArchive, bool p_boSaveNow = false );
    bool            getDBAutoArchive() const;
    void            setDBGlobalAutoSynchronize( const bool p_bDBGlobalAutoSynchronize, bool p_boSaveNow = false );
    bool            getDBGlobalAutoSynchronize() const;
    void            setSecondaryWindowVisibility( const bool p_bIsSecondaryWindowVisible, bool p_boSaveNow = false );
    bool            isSecondaryWindowVisible() const;
    void            setSecondaryWindowPosition( const QPoint &p_qpPosition, bool p_boSaveNow = false );
    QPoint          secondaryWindowPosition() const;
    void            setSecondaryWindowSize( const QSize &p_qsSize, bool p_boSaveNow = false );
    QSize           secondaryWindowSize() const;
    void            setSecondaryBackground( const QString &p_qsColor, bool p_boSaveNow = false );
    QString         getSecondaryBackground() const;
    void            setSecondaryFrame( const QString &p_qsColor, bool p_boSaveNow = false );
    QString         getSecondaryFrame() const;
    void            setSecondaryCaptionVisibility( const bool p_bIsSecondaryCaptionVisible, bool p_boSaveNow = false );
    bool            isSecondaryCaptionVisible() const;
    void            setActiveCaptionBackground( const QString &p_qsColor, bool p_boSaveNow = false );
    QString         getActiveCaptionBackground() const;
    void            setActiveCaptionColor( const QString &p_qsColor, bool p_boSaveNow = false );
    QString         getActiveCaptionColor() const;
    void            setInactiveCaptionBackground( const QString &p_qsColor, bool p_boSaveNow = false );
    QString         getInactiveCaptionBackground() const;
    void            setInactiveCaptionColor( const QString &p_qsColor, bool p_boSaveNow = false );
    QString         getInactiveCaptionColor() const;
    void            setSecondaryCaptionBackground( const QString &p_qsColor, bool p_boSaveNow = false );
    QString         getSecondaryCaptionBackground() const;
    void            setSecondaryCaptionColor( const QString &p_qsColor, bool p_boSaveNow = false );
    QString         getSecondaryCaptionColor() const;
    void            setPatientCardLostPrice( const int p_inPrice );
    int             getPatientCardLostPrice() const;
    void            setPatientCardLostPriceVat(const int p_inVat );
    int             getPatientCardLostPriceVat() const;
    void            setPatientCardPartnerPrice( const int p_inPrice );
    int             getPatientCardPartnerPrice() const;
    void            setPatientCardPartnerPriceVat(const int p_inVat );
    int             getPatientCardPartnerPriceVat() const;
    void            setBlnsHttpEnabled( bool p_bEnable, bool p_boSaveNow = false );
    bool            isBlnsHttpEnabled();
//    int             getBlnsHttpMessageWaitTime() const;
//    void            setBlnsHttpMessageWaitTime(const int p_inWaitTime );
//    bool            isBlnsHttpSuspended();
//    void            setBlnsHttpSuspended( bool p_bEnable );

    unsigned int    getPanelId( int p_nPanelIterator );

    void            setLogLevels( const unsigned int p_uiConLevel,
                                  const unsigned int p_uiDBLevel,
                                  const unsigned int p_uiGUILevel,
                                  const unsigned int p_uiFileLevel,
                                  bool p_boSaveNow = false );
    void            getLogLevels( unsigned int *p_poConLevel = NULL,
                                  unsigned int *p_poDBLevel = NULL,
                                  unsigned int *p_poGUILevel = NULL,
                                  unsigned int *p_uiFileLevel = NULL ) const;
    void            setDBAccess( const QString &p_qsHost, const QString &p_qsDB,
                                 const QString &p_qsUser, const QString &p_qsPwd );
    void            getDBAccess( QString *p_poHost = NULL, QString *p_poDB = NULL,
                                 QString *p_poUser = NULL, QString *p_poPwd = NULL) const;


    void            loadConfFileSettings();
    void            loadDBSettings() throw (cSevException);
    void            save() const throw (cSevException);

    unsigned int    postponedPatients() const;
    void            setPostponedPatients( const unsigned int p_uiPostponedPatients );
    unsigned int    postponedAttendances() const;
    void            setPostponedAttendances( const unsigned int p_uiPostponedAttendances );
    void            setDialogSize( const QString &p_qsDialogName, const QPoint &p_qpDlgSize );
    QPoint          getDialogSize( const QString &p_qsDialogName, const QPoint &p_qpDlgSizeDefault ) const;
    void            setDialogPosition(const QString &p_qsDialogName, const QPoint &p_qpPosition );
    QPoint          dialogPosition(const QString &p_qsDialogName) const;

    void            setDirDbBinaries( const QString &p_qsDirDbBinaries, bool p_boSaveNow = false );
    QString         getDirDbBinaries() const;
    void            setDirDbBackup( const QString &p_qsDirDbBackup, bool p_boSaveNow = false );
    QString         getDirDbBackup() const;
    void            setForceBackupDatabase( bool p_bForceBackupDatabase );
    bool            isForceBackupDatabase();
    void            setBackupDatabase( bool p_bBackupDatabase, bool p_boSaveNow = false );
    bool            isBackupDatabase();
    void            setBackupDatabaseType( const int p_nBackupDatabaseType, bool p_boSaveNow = false );
    int             getBackupDatabaseType() const;
    void            setBackupDatabaseDays( const QString &p_qsBackupDatabaseDays, bool p_boSaveNow = false );
    QString         getBackupDatabaseDays() const;
    void            setDateFormat( const QString &p_qsDateFormat, bool p_boSaveNow = false );
    QString         getDateFormat() const;
    void            setFapados( bool p_bFapados, bool p_boSaveNow = false );
    bool            isFapados();
    void            setPanelSterile( int p_nPanelId, bool p_bSterile );
    bool            isPanelSterile( int p_nPanelId );
    void            createExtendedAdminPassword( const QString &p_qsExtendedAdminPassword );
    void            setExtendedAdminPassword( const QString &p_qsExtendedAdminPassword );
    bool            checkExtendedAdminPassword( const QString &p_qsExtendedAdminPassword ) const;
    void            setStopInLine( bool p_bStopInLine, bool p_boSaveNow = false );
    bool            isStopInLine();
    void            setPanelTextSteril( const QString &p_qsPanelTextSteril, bool p_boSaveNow = false );
    QString         getPanelTextSteril() const;
    void            setPanelTextTubeReplace( const QString &p_qsPanelTextTubeReplace, bool p_boSaveNow = false );
    QString         getPanelTextTubeReplace() const;
    void            setPanelTextTubeCleanup( const QString &p_qsPanelTextTubeCleanup, bool p_boSaveNow = false );
    QString         getPanelTextTubeCleanup() const;
    void            setHWDebug( bool p_bHWDebug );
    bool            isHWDebugEnabled();
    void            setForceModuleSendTime( bool p_bForceModuleSendTime, bool p_boSaveNow = false );
    bool            isForceModuleSendTime();
    void            setForceModuleCheckButton( bool p_bForceModuleCheckButton, bool p_boSaveNow = false );
    bool            isForceModuleCheckButton();
    void            setForceTimeSendCounter( const int p_nForceTimeSendCounter );
    int             getForceTimeSendCounter() const;
    void            setTextTubeReplaceVisible( bool p_bTextTubeReplaceVisible, bool p_boSaveNow = false );
    bool            isTextTubeReplaceVisible();
    void            setTextTubeCleanupVisible( bool p_bTextTubeCleanupVisible, bool p_boSaveNow = false );
    bool            isTextTubeCleanupVisible();
    void            setTextSterilVisible( bool p_bTextSterilVisible, bool p_boSaveNow = false );
    bool            isTextSterilVisible();
//    void            setStartHttpSyncAuto( bool p_bStartHttpSyncAuto, bool p_boSaveNow = false );
//    bool            isStartHttpSyncAuto();
//    void            setStartHttpSyncAutoSeconds( const int p_nStartHttpSyncAutoSeconds );
//    int             getStartHttpSyncAutoSeconds() const;
    void            setBarcodeHidden( bool p_bBarcodeHidden, bool p_boSaveNow = false );
    bool            isBarcodeHidden();
    void            setUsageVisibleOnMain( bool p_bUsageVisibleOnMain, bool p_boSaveNow = false );
    bool            isUsageVisibleOnMain();
    void            setLicenceLastValidated( const QString &p_qsLicenceLastValidated, bool p_boSaveNow = false );
    QString         getLicenceLastValidated() const;
    void            setDACanModifyWorktime( bool p_bDACanModifyWorktime, bool p_boSaveNow = false );
    bool            isDACanModifyWorktime();
    void            setDACanModifyExpDate( bool p_bDACanModifyExpDate, bool p_boSaveNow = false );
    bool            isDACanModifyExpDate();
    void            setWebSyncAutoStart( bool p_bWebSyncAutoStart, bool p_boSaveNow = false );
    bool            isWebSyncAutoStart();
    void            setAutoMailOnPCSell( bool p_bAutoMailOnPCSell, bool p_boSaveNow = false );
    bool            isAutoMailOnPCSell();
    void            setAutoMailOnPCUse( bool p_bAutoMailOnPCUse, bool p_boSaveNow = false );
    bool            isAutoMailOnPCUse();
    void            setAutoMailOnPCExpiration( bool p_bAutoMailOnPCExpiration, bool p_boSaveNow = false );
    bool            isAutoMailOnPCExpiration();
    void            setPCExpirationDays( const int p_nPCExpirationDays, bool p_boSaveNow = false  );
    int             getPCExpirationDays() const;
    void            setAdvertisementSizeAndPos( const unsigned int p_uiId,
                                                const unsigned int p_uiLeft,
                                                const unsigned int p_uiTop,
                                                const unsigned int p_uiWidth,
                                                const unsigned int p_uiHeight,
                                                bool p_boSaveNow = false );
    void            setSecondsWaitOnSlpashScreen( const int p_nSecondsWaitOnSlpashScreen, bool p_boSaveNow = false );
    int             getSecondsWaitOnSlpashScreen() const;

    void            setValue(const QString &p_qsKey, const QString &p_qsValue);
    QString         getValue( const QString &p_qsKey ) const;

    void            setWindowMain( QWidget *p_wHandle );
    void            setWindowSecondary( QWidget *p_wHandle );
    void            moveWindowMain( int posX, int posY );
    void            resizeWindowMain( int width, int height );
    void            moveWindowSecondary( int posX, int posY );
    void            resizeWindowSecondary( int width, int height );

//void            setXXX( const QString &p_qsXXX, bool p_boSaveNow = false );
//QString         getXXX() const;
//void            setXXX( const int p_nXXX, bool p_boSaveNow = false );
//int             getXXX() const;
//void            setXXX( bool p_bXXX, bool p_boSaveNow = false );
//bool            isXXX();

private:
    QWidget        *m_wWindowMain;
    QWidget        *m_wWindowSecondary;
    QString         m_qsFileName;
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
//    int             m_nBlnsHttpWaitTime;
//    bool            m_bBlnsHttpSuspended;
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
//    bool            m_bIsStartHttpSyncAuto;
//    int             m_nStartHttpSyncAutoSeconds;
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

    void init();
};

#endif
