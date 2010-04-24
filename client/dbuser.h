#ifndef DBUSER_H
#define DBUSER_H

#include <QSqlRecord>

#include "../framework/sevexception.h"

class cDBUser
{
public:
    cDBUser();
    ~cDBUser();

    void          load( const unsigned int p_uiId )             throw( cSevException );
    void          load( const string &p_stName )                throw( cSevException );
    void          save()                                        throw( cSevException );
    void          createNew()                                   throw();
    void          logIn( const string &p_stPassword )           throw( cSevException );
    void          logOut()                                      throw();
    bool          isLoggedIn() const                            throw();
    unsigned int  id() const                                    throw();
    string        name() const                                  throw();
    void          setName( const string &p_stName )             throw();
    string        password() const                              throw();
    void          setPassword( const string &p_stPassword )     throw();
    string        realName() const                              throw();
    void          setRealName( const string &p_stRealName )     throw();
    string        groups() const                                throw();
    void          setGroups( const string &p_stGroups)          throw();
    bool          isInGroup( const string &p_stGroup ) const    throw();
    bool          active() const                                throw();
    void          setActive( const bool p_boActive )            throw();
    string        comment() const                               throw();
    void          setComment( const string &p_stComment )       throw();

private:
    bool          m_boLoggedIn;
    unsigned int  m_uiId;
    string        m_stName;
    string        m_stRealName;
    string        m_stPassword;
    string        m_stGroups;
    bool          m_boActive;
    string        m_stComment;

    void init( const unsigned int p_uiId = 0,
               const string &p_stName = "",
               const string &p_stRealName = "",
               const string &p_stPassword = "",
               const string &p_stGroups = "",
               const bool p_boActive = false,
               const string &p_stComment = "" )        throw();
    void init( const QSqlRecord &p_obRecord )          throw();
};

#endif
