#ifndef DB_DENOMINATION_H
#define DB_DENOMINATION_H

#include <QSqlRecord>

#include "../../framework/sevexception.h"

class cDBDenomination
{
public:
    cDBDenomination();
    ~cDBDenomination();

    void            load( const unsigned int p_uiId )                       throw( cSevException );
    void            load( const string &p_stName )                          throw( cSevException );
    void            save()                                                  throw( cSevException );
    void            remove()                                                throw( cSevException );
    void            createNew()                                             throw();
    unsigned int    id() const                                              throw();
    unsigned int    licenceId() const                                       throw();
    void            setLicenceId( const unsigned int p_nLicenceId )         throw();
    unsigned int    denomination() const                                    throw();
    void            setDenomination( const unsigned int p_uiDenomination )  throw();
    bool            active() const                                          throw();
    void            setActive( const bool p_bActive )                       throw();
    string          archive() const                                         throw();
    void            setArchive( const string &p_stArchive )                 throw();

private:
    unsigned int    m_uiId;
    unsigned int    m_uiLicenceId;
    unsigned int    m_uiDenomination;
    bool            m_bActive;
    string          m_stArchive;

    void init( const unsigned int p_uiId = 0,
               const unsigned int p_uiLicenceId = 0,
               const unsigned int p_uiDenomination = 0,
               const bool p_bActive = true,
               const string &p_stArchive = "NEW" )                          throw();
    void init( const QSqlRecord &p_obRecord )                               throw();
};

#endif
