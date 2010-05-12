#ifndef PREFERENCES_H
#define PREFERENCES_H

#include <QString>
#include "../framework/sevexception.h"

class cPreferences
{
public:
    cPreferences();
    cPreferences( const QString &p_qsFileName );
    ~cPreferences();

    void          setFileName( const QString &p_qsFileName );
    QString       getFileName() const;
    void          setVersion( const QString &p_qsVersion, bool p_boSaveNow = false );
    QString       getVersion() const;
    void          setLangFilePrefix( const QString &p_qsPrefix );
    QString       getLangFilePrefix() const;
    void          setLang( const QString &p_qsLang, bool p_boSaveNow = false );
    QString       getLang() const;
    void          setLastUser( const QString &p_qsLastUser, bool p_boSaveNow = false );
    QString       getLastUser() const;
    void          setPanelsPerRow( const unsigned int p_uiPanelsPerRow, bool p_boSaveNow = false );
    unsigned int  getPanelsPerRow() const;
    unsigned int  getPanelCount() const;
    void          setMainWindowSizePos( const unsigned int p_uiMainWindowLeft,
                                        const unsigned int p_uiMainWindowTop,
                                        const unsigned int p_uiMainWindowWidth,
                                        const unsigned int p_uiMainWindowHeight,
                                        bool p_boSaveNow = false );
    unsigned int  getMainWindowLeft() const;
    unsigned int  getMainWindowTop() const;
    unsigned int  getMainWindowWidth() const;
    unsigned int  getMainWindowHeight() const;
    void          setMainBackground( const QString &p_qsColor, bool p_boSaveNow = false );
    QString       getMainBackground() const;
    unsigned int  getLicenceId() const;
    QString       getClientSerial() const;
    void          setServerAddress( const QString &p_qsServerAddress, bool p_boSaveNow = false );
    QString       getServerAddress() const;
    void          setServerPort( const QString &p_qsServerPort, bool p_boSaveNow = false );
    QString       getServerPort() const;
    void          setCommunicationPort( const int p_nPortNumber );
    int           getCommunicationPort() const;
    void          setLogLevels( const unsigned int p_uiConLevel,
                                const unsigned int p_uiDBLevel,
                                const unsigned int p_uiGUILevel,
                                bool p_boSaveNow = false );
    void          getLogLevels( unsigned int *p_poConLevel = NULL,
                                unsigned int *p_poDBLevel = NULL,
                                unsigned int *p_poGUILevel = NULL ) const;
    void          setDBAccess( const QString &p_qsHost, const QString &p_qsDB,
                               const QString &p_qsUser, const QString &p_qsPwd );
    void          getDBAccess( QString *p_poHost = NULL, QString *p_poDB = NULL,
                               QString *p_poUser = NULL, QString *p_poPwd = NULL) const;


    void          loadConfFileSettings();
    void          loadDBSettings() throw (cSevException);
    void          save() const throw (cSevException);

private:
    QString         m_qsFileName;
    QString         m_qsVersion;
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
    QString         m_qsClientSerial;
    QString         m_qsServerAddress;
    QString         m_qsServerPort;
    int             m_nCommunicationPort;

    void init();
};

#endif
