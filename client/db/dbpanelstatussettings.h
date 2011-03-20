#ifndef DB_PANELSTATUS_SETTINGS_H
#define DB_PANELSTATUS_SETTINGS_H

#include <QSqlRecord>

#include "../../framework/sevexception.h"

class cDBPanelStatusSettings
{
public:
    cDBPanelStatusSettings();
    ~cDBPanelStatusSettings();

    void            load( const unsigned int p_uiId )                           throw( cSevException );
    void            loadStatus( const unsigned int p_uiId )                     throw( cSevException );
    void            save()                                                      throw( cSevException );
    void            remove()                                                    throw( cSevException );
    void            createNew()                                                 throw();
    unsigned int    id() const                                                  throw();
    unsigned int    licenceId() const                                           throw();
    void            setLicenceId( const unsigned int p_uiLicenceId )            throw();
    unsigned int    panelStatusId() const                                       throw();
    void            setPanelStatusId( const unsigned int p_uiPanelStatusId )    throw();
    QString         backgroundColor() const                                     throw();
    void            setBackgroundColor( const QString &p_qsBackgroundColor )    throw();
    QString         statusFontName() const                                      throw();
    void            setStatusFontName( const QString &p_qsStatusFontName )      throw();
    int             statusFontSize() const                                      throw();
    void            setStatusFontSize( const int p_inStatusFontSize )           throw();
    QString         statusFontColor() const                                     throw();
    void            setStatusFontColor( const QString &p_qsStatusFontColor )    throw();
    QString         timerFontName() const                                       throw();
    void            setTimerFontName( const QString &p_qsTimerFontName )        throw();
    int             timerFontSize() const                                       throw();
    void            setTimerFontSize( const int p_inTimerFontSize )             throw();
    QString         timerFontColor() const                                      throw();
    void            setTimerFontColor( const QString &p_qsTimerFontColor )      throw();
    QString         nextFontName() const                                        throw();
    void            setNextFontName( const QString &p_qsNextFontName )          throw();
    int             nextFontSize() const                                        throw();
    void            setNextFontSize( const int p_inNextFontSize )               throw();
    QString         nextFontColor() const                                       throw();
    void            setNextFontColor( const QString &p_qsNextFontColor )        throw();
    QString         infoFontName() const                                        throw();
    void            setInfoFontName( const QString &p_qsInfoFontName )          throw();
    int             infoFontSize() const                                        throw();
    void            setInfoFontSize( const int p_inInfoFontSize )               throw();
    QString         infoFontColor() const                                       throw();
    void            setInfoFontColor( const QString &p_qsInfoFontColor )        throw();
    QString         modified() const                                            throw();
    bool            active() const                                              throw();
    void            setActive( const bool p_bActive )                           throw();
    QString         archive() const                                             throw();
    void            setArchive( const QString &p_qsArchive )                    throw();

private:
    unsigned int    m_uiId;
    unsigned int    m_uiLicenceId;
    unsigned int    m_uiPanelStatusId;
    QString         m_qsBackgroundColor;
    QString         m_qsStatusFontName;
    int             m_inStatusFontSize;
    QString         m_qsStatusFontColor;
    QString         m_qsTimerFontName;
    int             m_inTimerFontSize;
    QString         m_qsTimerFontColor;
    QString         m_qsNextFontName;
    int             m_inNextFontSize;
    QString         m_qsNextFontColor;
    QString         m_qsInfoFontName;
    int             m_inInfoFontSize;
    QString         m_qsInfoFontColor;
    QString         m_qsModified;
    bool            m_bActive;
    QString         m_qsArchive;

    void init( const unsigned int p_uiId = 0,
               const unsigned int p_uiLicenceId = 0,
               const unsigned int p_uiPanelStatusId = 0,
               const QString &p_qsBackgroundColor = "",
               const QString &p_qsStatusFontName = "",
               const int p_inStatusFontSize = 0,
               const QString &p_qsStatusFontColor = "",
               const QString &p_qsTimerFontName = "",
               const int p_inTimerFontSize = 0,
               const QString &p_qsTimerFontColor = "",
               const QString &p_qsNextFontName = "",
               const int p_inNextFontSize = 0,
               const QString &p_qsNextFontColor = "",
               const QString &p_qsInfoFontName = "",
               const int p_inInfoFontSize = 0,
               const QString &p_qsInfoFontColor = "",
               const QString &p_qsModified = "",
               const bool p_bActive = true,
               const QString &p_qsArchive = "NEW" )                             throw();
    void init( const QSqlRecord &p_obRecord )                                   throw();
};

#endif
