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

    void                        load( const unsigned int p_uiId )                               throw( cSevException );
    void                        load( const QString &p_qsName )                                 throw( cSevException );
    void                        save()                                                          throw( cSevException );
    void                        remove()                                                        throw( cSevException );
    void                        createNew()                                                     throw();
    void                        logIn( const QString &p_qsPassword )                            throw( cSevException );
    void                        logOut()                                                        throw();
    bool                        isLoggedIn() const                                              throw();
    unsigned int                id() const                                                      throw();
    unsigned int                licenceId() const                                               throw();
    void                        setLicenceId( const unsigned int p_nLicenceId )                 throw();
    QString                     name() const                                                    throw();
    void                        setName( const QString &p_qsName )                              throw();
    QString                     password() const                                                throw();
    void                        setPassword( const QString &p_qsPassword )                      throw();
    QString                     realName() const                                                throw();
    void                        setRealName( const QString &p_qsRealName )                      throw();
    cAccessGroup::teAccessGroup group() const                                                   throw();
    void                        setGroup( const cAccessGroup::teAccessGroup p_enGroup)          throw();
    bool                        isInGroup( const cAccessGroup::teAccessGroup p_enGroup ) const  throw();
    bool                        active() const                                                  throw();
    void                        setActive( const bool p_boActive )                              throw();
    QString                     comment() const                                                 throw();
    void                        setComment( const QString &p_qsComment )                        throw();
    QString                     archive() const                                                 throw();
    void                        setArchive( const QString &p_qsArchive )                        throw();

private:
    bool                        m_boLoggedIn;
    unsigned int                m_uiId;
    unsigned int                m_uiLicenceId;
    QString                     m_qsName;
    QString                     m_qsRealName;
    QString                     m_qsPassword;
    cAccessGroup::teAccessGroup m_enGroup;
    bool                        m_boActive;
    QString                     m_qsComment;
    QString                     m_qsArchive;

    void init( const unsigned int p_uiId = 0,
               const unsigned int p_uiLicenceId = 0,
               const QString &p_qsName = "",
               const QString &p_qsRealName = "",
               const QString &p_qsPassword = "",
               const cAccessGroup::teAccessGroup p_enGroup = cAccessGroup::USER,
               const bool p_boActive = true,
               const QString &p_qsComment = "",
               const QString &p_qsArchive = "NEW" )     throw();
    void init( const QSqlRecord &p_obRecord )          throw();
};

#endif
