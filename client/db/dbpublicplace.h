#ifndef DB_PUBLIC_PLACE_H
#define DB_PUBLIC_PLACE_H

#include <QSqlRecord>

#include "../../framework/sevexception.h"

class cDBPublicPlace
{
public:
    cDBPublicPlace();
    ~cDBPublicPlace();

    void            load( const unsigned int p_uiId )               throw( cSevException );
    void            load( const QString &p_qsName )                 throw( cSevException );
    void            save()                                          throw( cSevException );
    void            remove()                                        throw( cSevException );
    void            createNew()                                     throw();
    unsigned int    id() const                                      throw();
    unsigned int    licenceId() const                               throw();
    void            setLicenceId( const unsigned int p_nLicenceId ) throw();
    QString         name() const                                    throw();
    void            setName( const QString &p_qsName )              throw();
    bool            active() const                                  throw();
    void            setActive( const bool p_bActive )               throw();
    QString         archive() const                                 throw();
    void            setArchive( const QString &p_qsArchive )        throw();

private:
    unsigned int    m_uiId;
    unsigned int    m_uiLicenceId;
    QString         m_qsName;
    bool            m_bActive;
    QString         m_qsArchive;

    void init( const unsigned int p_uiId = 0,
               const unsigned int p_uiLicenceId = 0,
               const QString &p_qsName = "",
               const bool p_bActive = true,
               const QString &p_qsArchive = "NEW" )                  throw();
    void init( const QSqlRecord &p_obRecord )                       throw();
};

#endif
