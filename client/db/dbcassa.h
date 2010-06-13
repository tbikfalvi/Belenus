#ifndef DB_CASSA_H
#define DB_CASSA_H

#include <QSqlRecord>

#include "../../framework/sevexception.h"

class cDBCassa
{
public:
    cDBCassa();
    ~cDBCassa();

    void            load( const unsigned int p_uiId )                   throw( cSevException );
    void            save()                                              throw( cSevException );
    void            remove()                                            throw( cSevException );
    void            createNew()                                         throw();
    unsigned int    id() const                                          throw();
    unsigned int    licenceId() const                                   throw();
    void            setLicenceId( const unsigned int p_nLicenceId )     throw();
    unsigned int    userId() const                                      throw();
    void            setUserId( const unsigned int p_nUserId )           throw();
    int             currentBalance() const                              throw();
    void            setCurrentBalance( const int p_nCurrentBalance )    throw();
    string          startDateTime() const                               throw();
    void            setStartDateTime( const string &p_stStartDateTime ) throw();
    string          stopDateTime() const                                throw();
    void            setStopDateTime( const string &p_stStopDateTime )   throw();
    bool            active() const                                      throw();
    void            setActive( const bool p_bActive )                   throw();
    string          archive() const                                     throw();
    void            setArchive( const string &p_stArchive )             throw();

private:
    unsigned int    m_uiId;
    unsigned int    m_uiLicenceId;
    unsigned int    m_uiUserId;
    int             m_nCurrentBalance;
    string          m_stStartDateTime;
    string          m_stStopDateTime;
    bool            m_bActive;
    string          m_stArchive;

    void init( const unsigned int p_uiId = 0,
               const unsigned int p_uiLicenceId = 0,
               const unsigned int p_uiUserId = 0,
               const int p_nCurrentBalance = 0,
               const string p_stStartDateTime = "",
               const string p_stStopDateTime = "",
               const bool p_bActive = false,
               const string &p_stArchive = "NEW" )                      throw();
    void init( const QSqlRecord &p_obRecord )                           throw();
};

#endif
