#ifndef DB_ZIPREGIONCITY_H
#define DB_ZIPREGIONCITY_H

#include <QSqlRecord>

#include "../../framework/sevexception.h"

class cDBZipRegionCity
{
public:
    cDBZipRegionCity();
    ~cDBZipRegionCity();

    void            load( const unsigned int p_uiId )               throw( cSevException );
    void            load( const QString &p_qsZip )                  throw( cSevException );
    void            loadCity( const QString &p_qsCity )             throw( cSevException );
    void            save()                                          throw( cSevException );
    void            remove()                                        throw( cSevException );
    void            createNew()                                     throw();
    unsigned int    id() const                                      throw();
    unsigned int    licenceId() const                               throw();
    void            setLicenceId( const unsigned int p_nLicenceId ) throw();
    QString         zip() const                                     throw();
    void            setZip( const QString &p_qsZip )                throw();
    QString         region() const                                  throw();
    void            setRegion( const QString &p_qsRegion )          throw();
    QString         city() const                                    throw();
    void            setCity( const QString &p_qsCity )              throw();
    bool            active() const                                  throw();
    void            setActive( const bool p_bActive )               throw();
    QString         archive() const                                 throw();
    void            setArchive( const QString &p_qsArchive )        throw();

private:
    unsigned int    m_uiId;
    unsigned int    m_uiLicenceId;
    QString         m_qsZip;
    QString         m_qsRegion;
    QString         m_qsCity;
    bool            m_bActive;
    QString         m_qsArchive;

    void init( const unsigned int p_uiId = 0,
               const unsigned int p_uiLicenceId = 0,
               const QString &p_qsZip = "",
               const QString &p_qsRegion = "",
               const QString &p_qsCity = "",
               const bool p_bActive = false,
               const QString &p_qsArchive = "NEW" )                  throw();
    void init( const QSqlRecord &p_obRecord )                       throw();
};

#endif
