#ifndef PREFERENCES_H
#define PREFERENCES_H

#include <QString>

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
    void          setMainWindowSize( const int p_nWindowWidth,
                                     const int p_nWindowHeight,
                                     bool p_boSaveNow = false );
    void          getMainWindowSize( int *p_nWindowWidth,
                                     int *p_nWindowHeight );
    void          setServerAddress( const QString &p_qsAddress, bool p_boSaveNow = false );
    QString       getServerAddress() const;
    void          setServerPort( const int p_nPort,
                                 bool p_boSaveNow = false );
    int           getServerPort() const;
    void          setMaxCorruptMessageCount( const int p_nMaxCorruptMessageCount,
                                             bool p_boSaveNow = false );
    int           getMaxCorruptMessageCount() const;


    void          loadConfFileSettings();
    void          loadDBSettings();
    void          save() const;

private:
    QString       m_qsFileName;
    QString       m_qsVersion;
    QString       m_qsLangFilePrefix;
    QString       m_qsLang;
    int           m_nWindowWidth;
    int           m_nWindowHeight;
    QString       m_qsAddress;
    int           m_nPort;
    int           m_nMaxCorruptMessageCount;

    void init();
};

#endif
