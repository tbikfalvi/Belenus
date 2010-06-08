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
    unsigned int    id() const                                                      throw();
    unsigned int    licenceId() const                                               throw();
    void            setLicenceId( const unsigned int p_nLicenceId )                 throw();
    unsigned int    panelId() const                                                 throw();
    void            setPanelId( const unsigned int p_uiPanelId )                    throw();
    unsigned int    useTime() const                                                 throw();
    void            setUseTime( const unsigned int p_uiUseTime )                    throw();
    unsigned int    useCurrency() const                                             throw();
    void            setUseCurrency( const unsigned int p_uiUseCurrency )            throw();
    bool            active() const                                                  throw();
    void            setActive( const bool p_bActive )                               throw();
    string          archive() const                                                 throw();
    void            setArchive( const string &p_stArchive )                         throw();

private:
    unsigned int    m_uiId;
    unsigned int    m_uiLicenceId;
    unsigned int    m_uiPanelId;
    unsigned int    m_uiUseTime;
    unsigned int    m_uiUseCurrency;
    bool            m_bActive;
    string          m_stArchive;

    void init( const unsigned int p_uiId = 0,
               const unsigned int p_uiLicenceId = 0,
               const unsigned int p_uiPanelId = 0,
               const unsigned int p_uiUseTime = 0,
               const unsigned int p_uiUseCurrency = 0,
               const bool p_bActive = true,
               const string &p_stArchive = "NEW" )                      throw();
    void init( const QSqlRecord &p_obRecord )                           throw();
};

#endif
