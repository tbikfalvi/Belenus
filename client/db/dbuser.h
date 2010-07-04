#ifndef DBUSER_H
#define DBUSER_H

#include <QSqlRecord>
#include <QString>
#include <QObject>

#include "../../framework/sevexception.h"

class cAccessGroup
{
public:
    enum teAccessGroup
    {
        MIN = 0,
        USER,
        ADMIN,
        SYSTEM,
        ROOT,
        MAX
    };

    static QString toStr( teAccessGroup p_enGroup ) {
        switch( p_enGroup ) {
            case USER:    return QObject::tr( "User" );    break;
            case ADMIN:   return QObject::tr( "Administrator" );   break;
            case SYSTEM:  return QObject::tr( "System Administrator" ); break;
            case ROOT:    return QObject::tr( "God" );    break;
            default:      return QObject::tr( "Invalid" );
        }
    }
};

class cDBUser
{
public:
    cDBUser();
    ~cDBUser();

    void            load( const unsigned int p_uiId )               throw( cSevException );
    void            load( const string &p_stName )                  throw( cSevException );
    void            save()                                          throw( cSevException );
    void            remove()                                        throw( cSevException );
    void            createNew()                                     throw();
    void            logIn( const string &p_stPassword )             throw( cSevException );
    void            logOut()                                        throw();
    bool            isLoggedIn() const                              throw();
    unsigned int    id() const                                      throw();
    string          name() const                                    throw();
    void            setName( const string &p_stName )               throw();
    string          password() const                                throw();
    void            setPassword( const string &p_stPassword )       throw();
    string          realName() const                                throw();
    void            setRealName( const string &p_stRealName )       throw();
    cAccessGroup::teAccessGroup group() const                       throw();
    void            setGroup( const cAccessGroup::teAccessGroup p_enGroup) throw();
    bool            isInGroup( const cAccessGroup::teAccessGroup p_enGroup ) const  throw();
    bool            active() const                                  throw();
    void            setActive( const bool p_boActive )              throw();
    string          comment() const                                 throw();
    void            setComment( const string &p_stComment )         throw();

private:
    bool          m_boLoggedIn;
    unsigned int  m_uiId;
    unsigned int  m_uiLicenceId;
    string        m_stName;
    string        m_stRealName;
    string        m_stPassword;
    cAccessGroup::teAccessGroup  m_enGroup;
    bool          m_boActive;
    string        m_stComment;
    string        m_stArchive;

    void init( const unsigned int p_uiId = 0,
               const unsigned int p_uiLicenceId = 0,
               const string &p_stName = "",
               const string &p_stRealName = "",
               const string &p_stPassword = "",
               const cAccessGroup::teAccessGroup p_enGroup = cAccessGroup::USER,
               const bool p_boActive = false,
               const string &p_stComment = "",
               const string &p_stArchive = "NEW" )     throw();
    void init( const QSqlRecord &p_obRecord )          throw();
};

#endif
