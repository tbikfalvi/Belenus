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
    void            setPatientCardLostPrice( const int p_inPrice );
    int             getPatientCardLostPrice() const;
    void            setPatientCardLostPriceVat(const int p_inVat );
    int             getPatientCardLostPriceVat() const;
    void            setPatientCardPartnerPrice( const int p_inPrice );
    int             getPatientCardPartnerPrice() const;
    void            setPatientCardPartnerPriceVat(const int p_inVat );
    int             getPatientCardPartnerPriceVat() const;
    void            setGibbigName( const QString &p_qsGibbigName, bool p_boSaveNow = false );
    QString         getGibbigName() const;
    void            setGibbigPassword( const QString &p_qsGibbigPassword, bool p_boSaveNow = false );
    QString         getGibbigPassword() const;
    void            setGibbigEnabled( bool p_bEnable, bool p_boSaveNow = false );
    bool            isGibbigEnabled();

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

private:
    QString         m_qsFileName;
    QString         m_qsVersion;
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
    QString         m_qsDefaultCountry;
    int             m_inZipLength;
    bool            m_bDBAutoArchive;
    bool            m_bDBGlobalAutoSynchronize;
    bool            m_bIsSecondaryWindowVisible;
    QPoint          m_qpSecondaryPosition;
    QSize           m_qsSecondarySize;
    QString         m_qsSecondaryBackground;
    bool            m_bBarcodeLengthDifferent;
    int             m_nPatientCardLostPrice;
    int             m_nPatientCardLostPriceVat;
    int             m_nPatientCardPartnerPrice;
    int             m_nPatientCardPartnerPriceVat;
    QStringList     m_qslPanelIds;
    QString         m_qsGibbigName;
    QString         m_qsGibbigPassword;
    bool            m_bGibbigEnabled;

    void init();
};

#endif
