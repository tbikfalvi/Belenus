#ifndef DB_XXX_H
#define DB_XXX_H

#include <QSqlRecord>

#include "../../framework/sevexception.h"

class cDBCassaHistory
{
public:
    cDBCassaHistory();
    ~cDBCassaHistory();

    void            load( const unsigned int p_uiId )                   throw( cSevException );
    void            save()                                              throw( cSevException );
    void            remove()                                            throw( cSevException );
    void            createNew()                                         throw();
    unsigned int    id() const                                          throw();
    unsigned int    licenceId() const                                   throw();
    void            setLicenceId( const unsigned int p_uiLicenceId )    throw();
    unsigned int    cassaId() const                                     throw();
    void            setCassaId( const unsigned int p_uiCassaId )        throw();
    unsigned int    userId() const                                      throw();
    void            setUserId( const unsigned int p_uiUserId )          throw();
    int             actionValue() const                                 throw();
    void            setActionValue( const int p_inActionValue )         throw();
    int             actionBalance() const                               throw();
    void            setActionBalance( const int p_inActionBalance )     throw();
    string          actionTime() const                                  throw();
    void            setActionTime( const string &p_stActionTime )       throw();
    string          comment() const                                     throw();
    void            setComment( const string &p_stComment )             throw();
    bool            active() const                                      throw();
    void            setActive( const bool p_bActive )                   throw();
    string          archive() const                                     throw();
    void            setArchive( const string &p_stArchive )             throw();

private:
    unsigned int    m_uiId;
    unsigned int    m_uiLicenceId;
    unsigned int    m_uiCassaId;
    unsigned int    m_uiUserId;
    int             m_inActionValue;
    int             m_inActionBalance;
    string          m_stActionTime;
    string          m_stComment;
    bool            m_bActive;
    string          m_stArchive;

    void init( const unsigned int p_uiId = 0,
               const unsigned int p_uiLicenceId = 0,
               const unsigned int p_uiCassaId = 0,
               const unsigned int p_uiUserId = 0,
               const int p_inActionValue = 0,
               const int p_inActionBalance = 0,
               const string &p_stActionTime = "",
               const string &p_stComment = "",
               const bool p_bActive = false,
               const string &p_stArchive = "NEW" )                  throw();
    void init( const QSqlRecord &p_obRecord )                       throw();
};

#endif
