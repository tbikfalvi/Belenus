#ifndef DB_PANELUSES_H
#define DB_PANELUSES_H

#include <QSqlRecord>

#include "../../framework/sevexception.h"

class cDBPanelUses
{
public:
    cDBPanelUses();
    ~cDBPanelUses();

    void            load( const unsigned int p_uiId )                               throw( cSevException );
    void            save()                                                          throw( cSevException );
    void            remove()                                                        throw( cSevException );
    void            createNew()                                                     throw();
    bool            isPanelUseExists()                                              throw();
    unsigned int    id() const                                                      throw();
    unsigned int    licenceId() const                                               throw();
    void            setLicenceId( const unsigned int p_nLicenceId )                 throw();
    QString         panelIds() const                                                throw();
    void            setPanelIds( const QString &p_qsPanelIds )                      throw();
    QString         name() const                                                    throw();
    void            setName( const QString &p_qsName )                              throw();
    unsigned int    useTime() const                                                 throw();
    void            setUseTime( const unsigned int p_uiUseTime )                    throw();
    unsigned int    usePrice() const                                                throw();
    void            setUsePrice( const unsigned int p_uiUsePrice )                  throw();
    QString         modified() const                                                throw();
    bool            active() const                                                  throw();
    void            setActive( const bool p_bActive )                               throw();
    QString         archive() const                                                 throw();
    void            setArchive( const QString &p_qsArchive )                        throw();

private:
    unsigned int    m_uiId;
    unsigned int    m_uiLicenceId;
    QString         m_qsPanelIds;
    QString         m_qsName;
    unsigned int    m_uiUseTime;
    unsigned int    m_uiUsePrice;
    QString         m_qsModified;
    bool            m_bActive;
    QString         m_qsArchive;

    void init( const unsigned int p_uiId = 0,
               const unsigned int p_uiLicenceId = 0,
               const QString &p_qsPanelIds = "",
               const QString &p_qsName = "",
               const unsigned int p_uiUseTime = 0,
               const unsigned int p_uiUsePrice = 0,
               const QString &p_qsModified = "",
               const bool p_bActive = true,
               const QString &p_qsArchive = "NEW" )                                 throw();
    void init( const QSqlRecord &p_obRecord )                                       throw();
};

#endif
