#ifndef DB_PANELTYPES_H
#define DB_PANELTYPES_H

#include <QSqlRecord>

#include "../../framework/sevexception.h"

class cDBPanelTypes
{
public:
    cDBPanelTypes();
    ~cDBPanelTypes();

    void            load( const unsigned int p_uiId )                   throw( cSevException );
    void            load( const string &p_stName )                      throw( cSevException );
    void            save()                                              throw( cSevException );
    void            remove()                                            throw( cSevException );
    void            createNew()                                         throw();
    unsigned int    id() const                                          throw();
    unsigned int    licenceId() const                                   throw();
    void            setLicenceId( const unsigned int p_nLicenceId )     throw();
    string          name() const                                        throw();
    void            setName( const string &p_stName )                   throw();
    bool            active() const                                      throw();
    void            setActive( const bool p_bActive )                   throw();
    string          archive() const                                     throw();
    void            setArchive( const string &p_stArchive )             throw();

private:
    unsigned int    m_uiId;
    unsigned int    m_uiLicenceId;
    string          m_stName;
    bool            m_bActive;
    string          m_stArchive;

    void init( const unsigned int p_uiId = 0,
               const unsigned int p_uiLicenceId = 0,
               const string &p_stName = "",
               const bool p_bActive = true,
               const string &p_stArchive = "NEW" )                   throw();
    void init( const QSqlRecord &p_obRecord )                       throw();
};

#endif
